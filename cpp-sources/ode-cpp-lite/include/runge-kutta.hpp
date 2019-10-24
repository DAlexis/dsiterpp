/*
 * runge-kutta.hpp
 *
 *  Created on: 12 янв. 2017 г.
 *      Author: dalexies
 */

#ifndef RUNGE_KUTTA_HPP_INCLUDED
#define RUNGE_KUTTA_HPP_INCLUDED

#include "include/time-iter.hpp"

namespace sotm {

class RungeKuttaIterator : public IntegrationMethodBase
{
public:
    RungeKuttaIterator(double step_precision = 1e-3);
	double iterate(double dt) override final;

private:
    void make_subiterations(double dt);
    double m_step_precision;
};

}  // namespace sotm

#endif // RUNGE_KUTTA_HPP_INCLUDED
