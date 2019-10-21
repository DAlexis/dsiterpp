#include "include/euler-explicit.hpp"

double EulerExplicitIterator::iterate(double dt)
{
    m_target->calculate_secondary_values(m_time);
    m_target->calculate_rhs(m_time);
    m_target->add_rhs_to_delta(dt);
	m_target->step();
	m_time += dt;
	return dt;
}
