#include "dsiterpp/runge-error-estimator.hpp"
#include "dsiterpp/integration.hpp"

#include <cmath>

using namespace dsiterpp;

RungeErrorEstimator::RungeErrorEstimator(int method_order) :
    m_order(method_order)
{
    m_error_multiplier = 1.0 / (pow(2, method_order) - 1);
}

void RungeErrorEstimator::calculate_delta_and_estimate(double t, double dt)
{
    make_test_steps(t, dt);
    estimate_error();
}

void RungeErrorEstimator::make_test_steps(double t, double dt)
{
    m_values.clear();
    m_deltas_h.clear();
    m_deltas_h_2_part_1.clear();
    m_deltas_h_2_part_2.clear();

    m_integrator->get_variable()->collect_values(m_values);

    m_integrator->calculate_delta(t, dt / 2.0);
    m_integrator->get_variable()->collect_deltas(m_deltas_h_2_part_1);
    m_integrator->get_variable()->step();
    m_integrator->calculate_delta(t + dt / 2.0, dt / 2.0);
    m_integrator->get_variable()->collect_deltas(m_deltas_h_2_part_2);
    auto it = m_values.cbegin();
    m_integrator->get_variable()->set_values(it);

    m_integrator->calculate_delta(t, dt);
    m_integrator->get_variable()->collect_deltas(m_deltas_h);
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
