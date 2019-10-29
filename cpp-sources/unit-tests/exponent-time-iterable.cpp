/*
 * exponent-time-iterable.cpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#include "exponent-time-iterable.hpp"

ExponentProblem::ExponentProblem(IIntegrator* continious_iterator, double initial_value) :
    m_continious_iterator(continious_iterator), m_variable(initial_value),
    m_exp_logic(m_variable, [](double t, double x) { return x; } )
{
    m_continious_iterator->set_variable(&m_variable);
    m_continious_iterator->set_logic(&m_exp_logic);
    m_time_iterator.set_continious_iterator(m_continious_iterator);
}

void ExponentProblem::iterate(double t)
{
    m_time_iterator.set_time(0.0);
    m_time_iterator.set_step(0.0001);
    m_time_iterator.set_stop_time(t);
    m_time_iterator.run();
}

double ExponentProblem::value()
{
    return m_variable.current_value();
}
