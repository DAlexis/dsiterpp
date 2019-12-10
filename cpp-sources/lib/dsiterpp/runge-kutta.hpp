/*
 * runge-kutta.hpp
 *
 *  Created on: 12 янв. 2017 г.
 *      Author: dalexies
 */

#ifndef RUNGE_KUTTA_HPP_INCLUDED
#define RUNGE_KUTTA_HPP_INCLUDED

#include "dsiterpp/integration.hpp"

namespace dsiterpp {

class RungeKuttaIterator : public IIntegrator
{
public:
    void calculate_delta(IVariable* variable, IRHS* rhs, double t, double dt) const override final;
    int method_order() const override final;
};

}

#endif // RUNGE_KUTTA_HPP_INCLUDED
