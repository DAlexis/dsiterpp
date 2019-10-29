#include "include/runge-error-estimator.hpp"
#include <cmath>

RungeErrorEstimator::RungeErrorEstimator(IIntegrator* integration_method, int method_order, ErrorEstimatorParameters* parameters) :
    m_integration_method(integration_method), m_parameters(parameters)
{
    m_error_multiplier = 1.0 / (pow(2, method_order) - 1);
}

void RungeErrorEstimator::set_variable(IVariable* variable)
{
    m_variable = variable;
    m_integration_method->set_variable(m_variable);
}

void RungeErrorEstimator::set_logic(IRHS* rhs)
{
    m_rhs = rhs;
    m_integration_method->set_logic(m_rhs);
}


const IntegratorMetrics& RungeErrorEstimator::metrics()
{
    return m_metrics;
}

double RungeErrorEstimator::calculate_delta(double t, double dt)
{
    for (;;)
    {
        make_test_steps(t, dt);
        estimate_error();
        if (m_rel_error < m_parameters->min_precision)
        {
            dt *= 1.5;
            break;
        }

        if (m_rel_error < m_parameters->max_precision)
        {
            break;
        }

        dt *= 0.6;
    }
    return dt;
}

void RungeErrorEstimator::make_test_steps(double t, double dt)
{
    m_values.clear();
    m_deltas_h.clear();
    m_deltas_h_2.clear();

    m_variable->collect_values(m_values);
    m_integration_method->calculate_delta(t, 2*dt);
    m_variable->collect_deltas(m_deltas_h);
    m_variable->clear_subiteration();

    m_integration_method->calculate_delta(t, dt);
    m_variable->collect_deltas(m_deltas_h);
}

void RungeErrorEstimator::estimate_error()
{
    double max_abs_error = 0.0;
    double max_rel_error = 0.0;
    for (size_t i = 0; i < m_values.size(); i++)
    {
        double abs_error = fabs(m_deltas_h[i] - m_deltas_h_2[i]) * m_error_multiplier;
        double base_value = fabs(m_values[i]) + fabs(m_deltas_h[i]);
        double rel_error = abs_error / base_value;

        if (rel_error > max_rel_error)
            max_rel_error = rel_error;

        if (abs_error > max_abs_error)
            max_abs_error = abs_error;
    }
    m_rel_error = max_rel_error;
    m_abs_error = max_abs_error;
}
