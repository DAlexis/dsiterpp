/*
 * exponent-time-iterable.hpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#ifndef UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_
#define UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_

#include "include/time-iter.hpp"

class ExponentProblem
{
public:
    ExponentProblem(IIntegrationMethod* continious_iterator, double initial_value = 1.0);
    void iterate(double t);
    double value();

private:
    IIntegrationMethod* m_continious_iterator;
    VariableScalar m_variable;
    ContinuousIterableLogicScalar m_exp_logic;
    TimeIterator m_time_iterator;

};


#endif /* UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_ */
