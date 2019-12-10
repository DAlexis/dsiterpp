/*
 * exponent-time-iterable.cpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#include "exponent-time-iterable.hpp"

using namespace dsiterpp;

ExponentProblem::ExponentProblem(IIntegrator* continious_iterator, double initial_value) :
    continious_iterator(continious_iterator), variable(initial_value),
    exp_rhs(variable, [](double t, double x) { return x; } )
{
    time_iterator.set_variable(&variable);
    time_iterator.set_rhs(&exp_rhs);
    time_iterator.set_continious_iterator(continious_iterator);
}

void ExponentProblem::iterate(double t)
{
    time_iterator.set_time(0.0);
    time_iterator.set_step(0.0001);
    time_iterator.set_stop_time(t);
    time_iterator.run();
}

double ExponentProblem::value()
{
    return variable.current_value();
}
