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

void RungeKuttaIterator::calculate_delta(double t, double dt)
{
    m_rhs->pre_iteration_job(t);
    make_subiterations(t, dt);
}

void RungeKuttaIterator::make_subiterations(double t, double dt)
{
	// k1 = f(tn, xn)
    //m_target->calculate_secondary_values(m_time);
    m_rhs->pre_sub_iteration_job(t);
    m_rhs->calculate_rhs(t);
    m_variable->add_rhs_to_delta(dt / 6.0);

	// k2 = f(tn + dt/2, xn + dt/2*k1)
    m_variable->make_sub_iteration(dt / 2.0);
    m_rhs->pre_sub_iteration_job(t + dt / 2.0);
    m_rhs->calculate_rhs(t + dt / 2.0);
    m_variable->add_rhs_to_delta(dt / 3.0);

	// k3 = f(tn + dt/2, xn + dt/2*k2)
    m_variable->make_sub_iteration(dt / 2.0);
    m_rhs->pre_sub_iteration_job(t + dt / 2.0);
    m_rhs->calculate_rhs(t + dt / 2.0);
    m_variable->add_rhs_to_delta(dt / 3.0);

	// k3 = f(tn + dt, xn + dt*k3)
    m_variable->make_sub_iteration(dt);
    m_rhs->pre_sub_iteration_job(t + dt);
    m_rhs->calculate_rhs(t + dt);
    m_variable->add_rhs_to_delta(dt / 6.0);
}
