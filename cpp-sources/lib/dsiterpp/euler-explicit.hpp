/*
 * euler-explicit.hpp
 *
 *  Created on: 6 сент. 2016 г.
 *      Author: dalexies
 */

#ifndef EULER_EXPLICI_HPP_INCLUDED
#define EULER_EXPLICI_HPP_INCLUDED

#include "dsiterpp/integration.hpp"

namespace dsiterpp {

class EulerExplicitIterator : public IntegrationMethodBase
{
public:
    void calculate_delta(double t, double dt) override final;
};

}

#endif // EULER_EXPLICI_HPP_INCLUDED
