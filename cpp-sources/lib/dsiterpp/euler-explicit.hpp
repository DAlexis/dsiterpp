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

class EulerExplicitIterator : public IIntegrator
{
public:
    void calculate_delta(IVariable* variable, IRHS* rhs, double t, double dt) const override final;
    int method_order() const override final;
};

}

#endif // EULER_EXPLICI_HPP_INCLUDED
