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

class RungeKuttaIterator : public ContinuousTimeIteratorBase
{
public:
	double iterate(double dt) override final;

private:
    void make_subiterations(double dt);
};

}  // namespace sotm

#endif // RUNGE_KUTTA_HPP_INCLUDED
