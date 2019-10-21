/*
 * exponent-time-iterable.cpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#include "exponent-time-iterable.hpp"

void Exponent::clear_subiteration()
{
	xCurrent = xPrevious;
	delta = 0.0;
}

void Exponent::calculate_secondary_values(double time)
{
	tmp = xCurrent;
}

void Exponent::calculate_rhs(double time)
{
    //UNUSED_ARG(time);
	rhs = tmp;
}

void Exponent::add_rhs_to_delta(double m)
{
	delta += rhs*m;
}

void Exponent::make_sub_iteration(double dt)
{
	xCurrent = xPrevious + rhs*dt;
}

void Exponent::step()
{
	xCurrent = xPrevious = xPrevious + delta;
	delta = 0;
}

double Exponent::getValue()
{
	return xPrevious;
}
