#ifndef RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED
#define RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED

#include "dsiterpp/error-estimator.hpp"
#include <vector>

namespace dsiterpp {

class RungeErrorEstimator : public ErrorEstimatorBase
{
public:
    RungeErrorEstimator(int method_order);
    void calculate_delta_and_estimate(double t, double dt) override;

private:
    void make_test_steps(double t, double dt);
    void estimate_error();

    int m_order;
    double m_error_multiplier;

    std::vector<double> m_values;
    std::vector<double> m_deltas_h;
    std::vector<double> m_deltas_h_2_part_1;
    std::vector<double> m_deltas_h_2_part_2;
};

}

#endif // RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED
