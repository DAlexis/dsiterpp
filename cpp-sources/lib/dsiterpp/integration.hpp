#ifndef INTEGRATION_HPP_INCLUDED
#define INTEGRATION_HPP_INCLUDED

#include "utils.hpp"

#include <vector>
#include <functional>

namespace dsiterpp {

class IVariable
{
public:
    virtual ~IVariable() {}

    /**
     * Clear current value of delta and make x_current = x_previous
     */
    virtual void clear_subiteration() = 0;

    /**
     * Adds rhs multiplied by m to xDelta
     */
    virtual void add_rhs_to_delta(double m) = 0;

    /**
     * Makes xCurrent = xPrevious + rhs*dt
     */
    virtual void make_sub_iteration(double dt) = 0;

    /**
     * Makes xCurrent = xPrevious = xPrevious + xDelta; xDelta = 0;
     */
    virtual void step() = 0;

    /**
     * For precision control.
     * Add all current values to vector. Vector may already contain something, use push_back
     */
    virtual void collect_values(std::vector<double>& values) const = 0;

    /**
     * For precision control.
     * Add all current deltas to vector. Vector may already contain something, use push_back
     */
    virtual void collect_deltas(std::vector<double>& deltas) const = 0;

    /**
     * For precision control.
     * Add all current values to vector. Vector may already contain something, use push_back
     */
    virtual void set_values(std::vector<double>::const_iterator& values) = 0;
};

class IRHS
{
public:
    virtual ~IRHS() {}

    /**
     * Tasks that done before iteration (i.e. grid refinment)
     */
    virtual void pre_iteration_job(double time) { DSITERPP_UNUSED(time); }

    /**
     * Tasks that done before sub iteration calculation
     */
    virtual void pre_sub_iteration_job(double time) { DSITERPP_UNUSED(time); }

    /** Calculate right hand side with values of
     * f(time, xCurrent, xCurrent of other objects, secondaryValues of other objects)
     */
    virtual void calculate_rhs(double time) = 0;
};

class VariableScalar : public IVariable
{
public:
    VariableScalar(double value = 0);
    void clear_subiteration() override;
    void add_rhs_to_delta(double m) override;
    void make_sub_iteration(double dt) override;
    void step() override;
    void collect_values(std::vector<double>& values) const override;
    void collect_deltas(std::vector<double>& deltas) const override;
    void set_values(std::vector<double>::const_iterator& values) override;

    // API for IContinuousIterableLogic
    double current_value();
    void set_rhs(double rhs);

    // API for usage
    operator double&();

private:
    double m_previous_value;
    double m_current_value;
    double m_delta;
    double m_rhs;
};

class VariablesGroup : public IVariable
{
public:
    void add_variable(IVariable& var);

    void clear_subiteration() override;
    void add_rhs_to_delta(double m) override;
    void make_sub_iteration(double dt) override;
    void step() override;
    void collect_values(std::vector<double>& values) const override;
    void collect_deltas(std::vector<double>& deltas) const override;
    void set_values(std::vector<double>::const_iterator& values) override;

private:
    std::vector<IVariable*> m_variables;};

class RHSScalar : public IRHS
{
public:
    using RHSFunction = std::function<double(double time, double x)>;
    RHSScalar(VariableScalar& scalar, RHSFunction rhs);

    void calculate_rhs(double time) override;

private:
    VariableScalar& m_scalar;
    RHSFunction m_rhs_function;

};

class IIntegrator
{
public:
    virtual ~IIntegrator() {}
    virtual void set_variable(IVariable* variable) = 0;
    virtual void set_rhs(IRHS* variable) = 0;

    virtual IVariable* get_variable() = 0;

    /**
     * Calculate delta for specified t and dt
     */
    virtual void calculate_delta(double t, double dt) = 0;
};

class IntegrationMethodBase : public IIntegrator
{
public:
    void set_variable(IVariable* variable) override;
    void set_rhs(IRHS* logic) override;

    IVariable* get_variable() override;

protected:
    IVariable *m_variable = nullptr;
    IRHS *m_rhs = nullptr;
};

}

#endif // INTEGRATION_HPP_INCLUDED
