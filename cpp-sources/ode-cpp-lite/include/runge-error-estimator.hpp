#ifndef RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED
#define RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED

#include "include/time-iter.hpp"
#include "include/error-estimator.hpp"

class RungeErrorEstimator : public IIntegrator
{
public:
    RungeErrorEstimator(IIntegrator* integration_method, int method_order, ErrorEstimatorParameters* parameters);
    void set_variable(IVariable* variable) override;
    void set_logic(IRHS* logic) override;

    const IntegratorMetrics& metrics() override;

    double calculate_delta(double t, double dt) override;


private:
    void make_test_steps(double t, double dt);
    double estimate_abs_error();
    double estimate_rel_error();
    void estimate_error();

    double m_error_multiplier;

    double m_rel_error, m_abs_error;

    IIntegrator* m_integration_method;
    IVariable *m_variable = nullptr;
    IRHS *m_rhs = nullptr;

    IntegratorMetrics m_metrics;

    std::vector<double> m_values;
    std::vector<double> m_deltas_h;
    std::vector<double> m_deltas_h_2;

    ErrorEstimatorParameters* m_parameters;
};

#endif // RUNGE_ERROR_ESTIMATOR_HPP_INCLUDED
