#ifndef PID_ANA_H
#define PID_ANA_H

#include <Python.h>
#include <numpy/arrayobject.h>
#include <iostream>
#include <vector>

class AnalyzerInterface
{
public:
    AnalyzerInterface();
    ~AnalyzerInterface()
    {
        Py_DECREF(pModule);
        Py_Finalize();
    };

    int runAnalyzer(const std::vector<double> &time_input, const std::vector<double> &state_input, const std::vector<double> &setpoint_input,
                    std::vector<double> &response_time_vec, std::vector<double> &response_step_vec);
    
    bool initialize_numpy() {
        import_array1(false);
        return true;
    }

private:
    PyObject *pModule = nullptr;
    PyObject *pClass = nullptr;

    std::vector<double> numpy_array_to_vector(PyObject *array);
};

#endif
