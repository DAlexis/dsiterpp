#ifndef BIFURCATION_HPP_INCLUDED
#define BIFURCATION_HPP_INCLUDED

#include "utils.hpp"

namespace dsiterpp
{

class IBifurcator
{
public:
    virtual ~IBifurcator() {}

    virtual void prepare_bifurcation(double time, double dt) { DSITERPP_UNUSED(time); DSITERPP_UNUSED(dt); }
    virtual void do_bifurcation(double time, double dt) = 0;
};

}

#endif // BIFURCATION_HPP_INCLUDED
