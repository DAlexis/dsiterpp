/*
 * runge-kutta.cpp
 *
 *  Created on: 12 янв. 2017 г.
 *      Author: dalexies
 */

#include "dsiterpp/runge-kutta.hpp"
#include <iostream>

using namespace std;
using namespace dsiterpp;

void RungeKuttaIterator::calculate_delta(IVariable* variable, IRHS* rhs, double t, double dt) const
{
    rhs->pre_iteration_job(t);

    // k1 = f(tn, xn)
    //m_target->calculate_secondary_values(m_time);
    rhs->pre_sub_iteration_job(t);
    rhs->calculate_rhs(t);
    variable->add_rhs_to_delta(dt / 6.0);

    // k2 = f(tn + dt/2, xn + dt/2*k1)
    variable->make_sub_iteration(dt / 2.0);
    rhs->pre_sub_iteration_job(t + dt / 2.0);
    rhs->calculate_rhs(t + dt / 2.0);
    variable->add_rhs_to_delta(dt / 3.0);

    // k3 = f(tn + dt/2, xn + dt/2*k2)
    variable->make_sub_iteration(dt / 2.0);
    rhs->pre_sub_iteration_job(t + dt / 2.0);
    rhs->calculate_rhs(t + dt / 2.0);
    variable->add_rhs_to_delta(dt / 3.0);

    // k3 = f(tn + dt, xn + dt*k3)
    variable->make_sub_iteration(dt);
    rhs->pre_sub_iteration_job(t + dt);
    rhs->calculate_rhs(t + dt);
    variable->add_rhs_to_delta(dt / 6.0);
}

int RungeKuttaIterator::method_order() const
{
    return 4;
}
