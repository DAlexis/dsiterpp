#include "dsiterpp/euler-explicit.hpp"

using namespace dsiterpp;

void EulerExplicitIterator::calculate_delta(IVariable* variable, IRHS* rhs, double t, double dt) const
{
    rhs->pre_iteration_job(t);
    rhs->pre_sub_iteration_job(t);

    rhs->calculate_rhs(t);
    variable->add_rhs_to_delta(dt);
}

int EulerExplicitIterator::method_order() const
{
    return 1;
}
