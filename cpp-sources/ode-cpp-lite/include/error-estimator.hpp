#ifndef EEROR_ESTIMATOR_HPP_INCLUDED
#define EEROR_ESTIMATOR_HPP_INCLUDED

#include "include/time-iter.hpp"

struct ErrorEstimatorParameters
{
    bool autoStepAdjustment = false;
    double max_step = 0.001;
    double min_step = 0.00001;
    double max_precision = 0.0001;
    double min_precision = 0.001;

};

#endif // EEROR_ESTIMATOR_HPP_INCLUDED
