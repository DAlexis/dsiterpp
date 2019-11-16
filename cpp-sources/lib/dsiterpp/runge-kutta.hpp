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

class RungeKuttaIterator : public IntegrationMethodBase
{
public:
    void calculate_delta(double t, double dt) override final;

private:
    void make_subiterations(double t, double dt);
};

}

#endif // RUNGE_KUTTA_HPP_INCLUDED
