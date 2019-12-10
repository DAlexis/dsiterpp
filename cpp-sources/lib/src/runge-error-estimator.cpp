#include "dsiterpp/runge-error-estimator.hpp"
#include "dsiterpp/integration.hpp"

#include <cmath>

using namespace dsiterpp;

RungeErrorEstimator::RungeErrorEstimator()
{
    m_error_multiplier = 1.0 / (pow(2, m_integrator->method_order()) - 1);
}

void RungeErrorEstimator::calculate_delta_and_estimate(IVariable* variable, IRHS* rhs, double t, double dt)
{
    make_test_steps(variable, rhs, t, dt);
    estimate_error();
}

void RungeErrorEstimator::make_test_steps(IVariable* variable, IRHS* rhs, double t, double dt)
{
    m_values.clear();
    m_deltas_h.clear();
    m_deltas_h_2_part_1.clear();
    m_deltas_h_2_part_2.clear();

    variable->collect_values(m_values);

    m_integrator->calculate_delta(variable, rhs, t, dt / 2.0);
    variable->collect_deltas(m_deltas_h_2_part_1);
    variable->step();
    m_integrator->calculate_delta(variable, rhs, t + dt / 2.0, dt / 2.0);
    variable->collect_deltas(m_deltas_h_2_part_2);
    auto it = m_values.cbegin();
    variable->set_values(it);

    m_integrator->calculate_delta(variable, rhs, t, dt);
    variable->collect_deltas(m_deltas_h);
}

void RungeErrorEstimator::estimate_error()
{
    m_error.max_abs_error = 0.0;
    m_error.max_rel_error = 0.0;

    for (size_t i = 0; i < m_values.size(); i++)
    {
        double abs_error = fabs(m_deltas_h[i] - (m_deltas_h_2_part_1[i] + m_deltas_h_2_part_2[i]) ) * m_error_multiplier;
        double base_value = fabs(m_values[i]) + fabs(m_deltas_h[i]);
        double rel_error = abs_error / base_value;

        if (rel_error > m_error.max_rel_error)
            m_error.max_rel_error = rel_error;

        if (abs_error > m_error.max_abs_error)
            m_error.max_abs_error = abs_error;
    }
}
