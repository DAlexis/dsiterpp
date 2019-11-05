#ifndef INTEGRATION_HPP_INCLUDED
#define INTEGRATION_HPP_INCLUDED

#include <vector>
#include <functional>

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
    virtual void collect_values(std::vector<double>& values) = 0;

    /**
     * For precision control.
     * Add all current deltas to vector. Vector may already contain something, use push_back
     */
    virtual void collect_deltas(std::vector<double>& deltas) = 0;
};


class IRHS
{
public:
    virtual ~IRHS() {}

    /**
     * Tasks that done before iteration (i.e. grid refinment)
     */
    virtual void pre_iteration_job(double time) {}

    /**
     * Tasks that done before sub iteration calculation
     */
    virtual void pre_sub_iteration_job(double time) {}

    /** Calculate right hand side with values of
     * f(time, xCurrent, xCurrent of other objects, secondaryValues of other objects)
     */
    virtual void calculate_rhs(double time) = 0;
};

class VariableScalar : public IVariable
{
public:
    VariableScalar(double value = 0) : m_previous_value(value) { clear_subiteration(); }
    void clear_subiteration() override { m_current_value = m_previous_value; m_delta = 0.0;}
    void add_rhs_to_delta(double m) override { m_delta += m_rhs * m; }
    void make_sub_iteration(double dt) override { m_current_value = m_previous_value + m_rhs * dt; }
    void step() override { m_current_value = m_previous_value = m_previous_value + m_delta; m_delta = 0.0;}
    void collect_values(std::vector<double>& values) override { values.push_back(m_current_value); }
    void collect_deltas(std::vector<double>& deltas) override { deltas.push_back(m_delta); }

    // API for IContinuousIterableLogic
    double current_value() { return m_current_value; }
    void set_rhs(double rhs) { m_rhs = rhs; }

    // API for usage
    operator double&()
    {
        return m_previous_value;
    }
private:
    double m_previous_value;
    double m_current_value;
    double m_delta;
    double m_rhs;
};

class RHSScalar : public IRHS
{
public:
    using RHSFunction = std::function<double(double time, double x)>;
    RHSScalar(VariableScalar& scalar, RHSFunction rhs) :
        m_scalar(scalar), m_rhs_function(rhs)
    {
    }

    void calculate_rhs(double time) override
    {
        double rhs = m_rhs_function(time, m_scalar.current_value());
        m_scalar.set_rhs(rhs);
    }

private:
    VariableScalar& m_scalar;
    RHSFunction m_rhs_function;

};

struct IntegratorMetrics
{
    size_t totalStepCalculations = 0;
    size_t timeIterations = 0;
    size_t complexity = 0;

    double adaptationEfficiency() { return double(timeIterations) / totalStepCalculations; }
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
    virtual const IntegratorMetrics& metrics() = 0;
};


class IntegrationMethodBase : public IIntegrator
{
public:
    void set_variable(IVariable* variable) override;
    void set_rhs(IRHS* logic) override;

    IVariable* get_variable() override;

    const IntegratorMetrics& metrics() override;

protected:
    IVariable *m_variable = nullptr;
    IRHS *m_rhs = nullptr;
    IntegratorMetrics m_metrics;
};

#endif // INTEGRATION_HPP_INCLUDED
