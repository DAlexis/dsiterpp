/*
 * exponent-time-iterable.hpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#ifndef UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_
#define UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_

#include "dsiterpp/time-iter.hpp"
#include "dsiterpp/integration.hpp"

namespace dsiterpp {

class ExponentProblem
{
public:
    ExponentProblem(IIntegrator* continious_iterator, double initial_value = 1.0);
    void iterate(double t);
    double value();


    IIntegrator* continious_iterator;
    VariableScalar variable;
    RHSScalar exp_rhs;
    TimeIterator time_iterator;

};

}

#endif /* UNIT_TESTS_LIBSOTM_UT_TIME_ITER_EXPONENT_TIME_ITERABLE_HPP_ */
