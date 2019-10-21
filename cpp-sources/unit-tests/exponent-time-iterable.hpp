/*
 * exponent-time-iterable.hpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#ifndef UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_
#define UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_

#include "include/time-iter.hpp"


/**
 * Simpliest ordinary differential equation
 * x' = x
 * with
 * x = 1 at t = 0
 */
class Exponent : public IContinuousTimeIterable
{
public:
    void clear_subiteration() override;
    void calculate_secondary_values(double time) override;
    void calculate_rhs(double time) override;
    void add_rhs_to_delta(double m) override;
    void make_sub_iteration(double dt) override;
	void step() override;
	double getValue();

private:
	double xPrevious = 1;
	double xCurrent = xPrevious;
	double rhs = 0;
	double delta = 0;
	double tmp = 0; // Secondary variable that will be used as RHS
};


#endif /* UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_ */
