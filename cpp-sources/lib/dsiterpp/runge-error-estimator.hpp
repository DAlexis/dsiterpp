#ifndef RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED
#define RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED

#include "dsiterpp/error-estimator.hpp"
#include <vector>

namespace dsiterpp {

class RungeErrorEstimator : public ErrorEstimatorBase
{
public:
    RungeErrorEstimator();
    void calculate_delta_and_estimate(IVariable* variable, IRHS* rhs, double t, double dt) override;
    void set_integrator(IIntegrator* integrator) override;

private:
    void make_test_steps(IVariable* variable, IRHS* rhs, double t, double dt);
    void estimate_error();

    double m_error_multiplier;

    std::vector<double> m_values;
    std::vector<double> m_deltas_h;
    std::vector<double> m_deltas_h_2_part_1;
    std::vector<double> m_deltas_h_2_part_2;
};

}

#endif // RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED
