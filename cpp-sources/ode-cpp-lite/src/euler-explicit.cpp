#include "include/euler-explicit.hpp"

double EulerExplicitIterator::calculate_delta(double t, double dt)
{
    m_rhs->pre_iteration_job(t);
    m_rhs->pre_sub_iteration_job(t);

    m_rhs->calculate_rhs(t);
    m_variable->add_rhs_to_delta(dt);
	return dt;
}
