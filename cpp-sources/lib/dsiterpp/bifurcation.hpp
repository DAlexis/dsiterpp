#ifndef BIFURCATION_HPP_INCLUDED
#define BIFURCATION_HPP_INCLUDED

namespace dsiterpp
{

class IBifurcator
{
public:
    virtual ~IBifurcator() {}

    virtual void prepare_bifurcation(double time, double dt) = 0;
    virtual void do_bifurcation(double time, double dt) = 0;
};

}

#endif // BIFURCATION_HPP_INCLUDED
