#include "pid_analyzer_interface.h"

/*
    The original source code: https://github.com/Plasmatree/PID-Analyzer by Florian Melsheimer
*/

std::vector<double> AnalyzerInterface::numpy_array_to_vector(PyObject *array)
{
    std::vector<double> result;
    if (PyArray_Check(array))
    {
        PyArrayObject *arr = (PyArrayObject *)array;
        npy_intp size = PyArray_SIZE(arr);
        double *data = (double *)PyArray_DATA(arr);
        result.assign(data, data + size);
    }
    return result;
}

AnalyzerInterface::AnalyzerInterface()
{
    Py_Initialize();

    if (!initialize_numpy())
        std::cerr << "Failed to initialize NumPy!" << std::endl;
    
    // Define Python code to declare a class and its method
    const char *pythonCode = R"(
import numpy as np
from scipy.interpolate import interp1d
from scipy.ndimage import gaussian_filter1d

class Trace:
    """ PID response analysis based on a deconvolution using a
    setpoint and the measured process variable as inputs.
    It computes an average, stdev and a 2D histogram.
    """
    framelen = 1.           # length of each single frame over which to compute response
    resplen = 0.5           # length of respose window
    cutfreq = 25.           # cutfreqency of what is considered as input
    tuk_alpha = 1.0         # alpha of tukey window, if used
    superpos = 16           # sub windowing (superpos windows in framelen)
    threshold = 500.        # threshold for 'high input rate'
    noise_framelen = 0.3    # window width for noise analysis
    noise_superpos = 16     # subsampling for noise analysis windows

    def __init__(self, time, gyro_rate, gyro_setpoint, gyro_noise_thresh=0.0):
        # equally space samples in time
        data = {
            'gyro': gyro_rate,
            'input': gyro_setpoint,
            }
        self.time, self.data = self.equalize_data(time, data)
        self.gyro = self.data['gyro']    # The quad does
        self.input = self.data['input']  # What the quad should do
        self.dt = self.time[0]-self.time[1]
        self.data['time'] = self.time
        self.flen = self.stepcalc(self.time, Trace.framelen)        # array len corresponding to framelen in s
        self.rlen = self.stepcalc(self.time, Trace.resplen)         # array len corresponding to resplen in s
        self.time_resp = self.time[0:self.rlen]-self.time[0]
        self.stacks = self.winstacker({'time':[],'input':[],'gyro':[]}, self.flen, Trace.superpos)
        self.window = np.hanning(self.flen)
        self.spec_sm, self.avr_t, self.avr_in, self.max_in = self.stack_response(self.stacks, self.window)
        self.low_mask, self.high_mask = self.low_high_mask(self.max_in, self.threshold)       #calcs masks for high and low inputs according to threshold
        self.toolow_mask = self.low_high_mask(self.max_in, gyro_noise_thresh)[1]          #mask for ignoring noisy low input

        self.resp_low = self.weighted_mode_avr(self.spec_sm, self.low_mask*self.toolow_mask, [-1.5,3.5], 1000)
        if self.high_mask.sum()>0:
            self.resp_high = self.weighted_mode_avr(self.spec_sm, self.high_mask*self.toolow_mask, [-1.5,3.5], 1000)

    @staticmethod
    def low_high_mask(signal, threshold):
        low = np.copy(signal)

        low[low <=threshold] = 1.
        low[low > threshold] = 0.
        high = -low+1.

        if high.sum() < 10:     # ignore high pinput that is too short
            high *= 0.

        return low, high

    def to_mask(self, clipped):
        clipped-=clipped.min()
        clipped_max = clipped.max()
        if clipped_max > 1e-10: # avoid division by zero
            clipped/=clipped_max
        return clipped


    @staticmethod
    def equalize_data(time, data):
        """
        Resample & interpolate all dict elements in data for equal sampling in time
        :return: tuple of (time, data)
        """
        newtime = np.linspace(time[0], time[-1], len(time), dtype=np.float64)
        output = {}
        for key in data:
            output[key] = interp1d(time, data[key])(newtime)
        return (newtime, output)


    def stepcalc(self, time, duration):
        ### calculates frequency and resulting windowlength
        tstep = (time[-1]-time[0])/len(time)
        freq = 1./tstep
        arr_len = duration * freq
        return int(arr_len)

    def winstacker(self, stackdict, flen, superpos):
        ### makes stack of windows for deconvolution
        tlen = len(self.time)
        shift = int(flen/superpos)
        wins = int((tlen-flen)/shift)
        for i in np.arange(wins):
            for key in stackdict.keys():
                stackdict[key].append(self.data[key][i * shift:i * shift + flen])
        for k in stackdict.keys():
            stackdict[k]=np.array(stackdict[k], dtype=np.float64)
        return stackdict

    def wiener_deconvolution(self, input, output, cutfreq):      # input/output are two-dimensional
        pad = 1024 - (len(input[0]) % 1024)                     # padding to power of 2, increases transform speed
        input = np.pad(input, [[0,0],[0,pad]], mode='constant')
        output = np.pad(output, [[0, 0], [0, pad]], mode='constant')
        H = np.fft.fft(input, axis=-1)
        G = np.fft.fft(output,axis=-1)
        freq = np.abs(np.fft.fftfreq(len(input[0]), self.dt))
        sn = self.to_mask(np.clip(np.abs(freq), cutfreq-1e-9, cutfreq))
        len_lpf=np.sum(np.ones_like(sn)-sn)
        sn=self.to_mask(gaussian_filter1d(sn,len_lpf/6.))
        sn= 10.*(-sn+1.+1e-9)       # +1e-9 to prohibit 0/0 situations
        Hcon = np.conj(H)
        deconvolved_sm = np.real(np.fft.ifft(G * Hcon / (H * Hcon + 1./sn),axis=-1))
        return deconvolved_sm

    def stack_response(self, stacks, window):
        inp = stacks['input'] * window
        outp = stacks['gyro'] * window

        deconvolved_sm = self.wiener_deconvolution(inp, outp, self.cutfreq)[:, :self.rlen]
        delta_resp = deconvolved_sm.cumsum(axis=1)

        avr_in = np.abs(np.abs(inp)).mean(axis=1)
        max_in = np.max(np.abs(inp), axis=1)
        avr_t = stacks['time'].mean(axis=1)

        return delta_resp, avr_t, avr_in, max_in

    def weighted_mode_avr(self, values, weights, vertrange, vertbins):
        ### finds the most common trace and std
        threshold = 0.5  # threshold for std calculation
        filt_width = 7  # width of gaussian smoothing for hist data

        resp_y = np.linspace(vertrange[0], vertrange[-1], vertbins, dtype=np.float64)
        times = np.repeat(np.array([self.time_resp],dtype=np.float64), len(values), axis=0)
        weights = np.repeat(weights, len(values[0]))

        hist2d = np.histogram2d(times.flatten(), values.flatten(),
                                range=[[self.time_resp[0], self.time_resp[-1]], vertrange],
                                bins=[len(times[0]), vertbins], weights=weights.flatten())[0].transpose()

        if hist2d.sum():
            hist2d_sm = gaussian_filter1d(hist2d, filt_width, axis=0, mode='constant')
            hist2d_sm /= np.max(hist2d_sm, 0)


            pixelpos = np.repeat(resp_y.reshape(len(resp_y), 1), len(times[0]), axis=1)
            avr = np.average(pixelpos, 0, weights=hist2d_sm * hist2d_sm)
        else:
            hist2d_sm = hist2d
            avr = np.zeros_like(self.time_resp)
        # only used for monochrome error width
        hist2d[hist2d <= threshold] = 0.
        hist2d[hist2d > threshold] = 0.5 / (vertbins / (vertrange[-1] - vertrange[0]))
        std = np.sum(hist2d, 0)
        return avr, std, [self.time_resp, resp_y, hist2d_sm]

    def get_response(self):
        return self.time_resp, self.resp_low[0]
)";

    PyRun_SimpleString(pythonCode);

    // Get reference to the Python class
    PyObject *pName = PyUnicode_FromString("__main__");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr)
    {
        PyObject *pDict = PyModule_GetDict(pModule);
        pClass = PyDict_GetItemString(pDict, "Trace");

        if (pClass == nullptr)
            std::cerr << "Failed to get 'MyClass' from module dictionary." << std::endl;
    }
    else
    {
        std::cerr << "Failed to import module." << std::endl;
    }
}

int AnalyzerInterface::runAnalyzer(const std::vector<double> &time_input, const std::vector<double> &state_input, const std::vector<double> &setpoint_input,
                                   std::vector<double> &response_time_vec, std::vector<double> &response_step_vec)
{
    npy_intp size = time_input.size();
    double *time_data = new double[size];
    double *gyro_rate_data = new double[size];
    double *gyro_setpoint_data = new double[size];
    for (int i = 0; i < size; ++i)
    {
        time_data[i] = time_input[i];
        gyro_rate_data[i] = state_input[i];
        gyro_setpoint_data[i] = setpoint_input[i];
    }
    PyObject *time = PyArray_SimpleNewFromData(1, &size, NPY_DOUBLE, time_data);
    PyObject *gyro_rate = PyArray_SimpleNewFromData(1, &size, NPY_DOUBLE, gyro_rate_data);
    PyObject *gyro_setpoint = PyArray_SimpleNewFromData(1, &size, NPY_DOUBLE, gyro_setpoint_data);
    double gyro_noise_thresh = 1;
    PyObject *pInstance = PyObject_CallObject(pClass, Py_BuildValue("(OOOd)", time, gyro_rate, gyro_setpoint, gyro_noise_thresh));

    if (pInstance == nullptr)
    {
        PyErr_Print();
        std::cerr << "Failed to create an instance of MyClass" << std::endl;
        return 1;
    }
    PyObject *pResult = PyObject_CallMethod(pInstance, "get_response", NULL);
    if (pResult)
    {
        PyObject *response_time = PyTuple_GetItem(pResult, 0);
        PyObject *response_step = PyTuple_GetItem(pResult, 1);

        // Convert numpy arrays to std::vector
        response_time_vec = numpy_array_to_vector(response_time);
        response_step_vec = numpy_array_to_vector(response_step);

        Py_DECREF(pResult);
    }
    else
    {
        PyErr_Print();
        std::cerr << "Failed to call get_gyro_rate_and_setpoint" << std::endl;
    }
    Py_DECREF(pInstance);

    return 0;
}