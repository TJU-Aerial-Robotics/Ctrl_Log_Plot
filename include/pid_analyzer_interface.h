#ifndef PID_ANA_H
#define PID_ANA_H

#include <Python.h>
#include <numpy/arrayobject.h>
#include <iostream>
#include <vector>

/*
    The original source code: https://github.com/Plasmatree/PID-Analyzer by Florian Melsheimer
*/

std::vector<double> numpy_array_to_vector(PyObject *array);

int AnalyzerInterface(const std::vector<double> &time_input, const std::vector<double> &state_input, const std::vector<double> &setpoint_input,
                      std::vector<double> &response_time_vec, std::vector<double> &response_step_vec);

#endif
