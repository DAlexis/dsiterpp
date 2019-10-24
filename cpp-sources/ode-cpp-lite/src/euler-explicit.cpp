#include "include/euler-explicit.hpp"

double EulerExplicitIterator::iterate(double dt)
{
    m_rhs->pre_iteration_job(m_time);
    m_rhs->pre_sub_iteration_job(m_time);

    m_rhs->calculate_rhs(m_time);
    m_variable->add_rhs_to_delta(dt);
    m_variable->step();
	m_time += dt;
	return dt;
}
