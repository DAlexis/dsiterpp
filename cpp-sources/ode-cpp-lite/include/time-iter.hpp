#ifndef TIME_ITER_HPP
#define TIME_ITER_HPP

#include <vector>
#include <functional>
#include <limits>
#include <cstddef>
#include <cmath>

struct IntegrationParameters;
struct ContiniousIteratorMetrics;

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

class ContinuousIterableLogicScalar : public IRHS
{
public:
    using RHSFunction = std::function<double(double time, double x)>;
    ContinuousIterableLogicScalar(VariableScalar& scalar, RHSFunction rhs) :
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

class IBifurcator
{
public:
    virtual ~IBifurcator() {}

    virtual void prepare_bifurcation(double time, double dt) = 0;
    virtual void do_bifurcation(double time, double dt) = 0;
};

class IIntegrationMethod
{
public:
    virtual ~IIntegrationMethod() {}
    virtual void set_variable(IVariable* variable) = 0;
    virtual void set_logic(IRHS* variable) = 0;

    /**
     * Make one iteration. If timestep adaptation enabled, timestep may be changed
     * @return new timestep
     */
    virtual double iterate(double dt) = 0;
    virtual void set_time(double time) = 0;
    virtual double time() = 0;
    virtual void set_parameters(IntegrationParameters* parameters) = 0;
    virtual const IntegrationParameters* parameters() = 0;
    virtual const ContiniousIteratorMetrics& metrics() = 0;
};

class ITimeHook
{
public:
    virtual ~ITimeHook() {}
    virtual void run_hook(double time) = 0;
    virtual double get_next_time() = 0;
};

struct IntegrationParameters
{
    bool autoStepAdjustment = false;
    double max_step = 0.001;
    double min_step = 0.00001;
    double target_precision = 0.001;

/*
    enum class VerboseLevel {
        none = 0, less, more
    };
    VerboseLevel outputVerboseLevel = VerboseLevel::none;
*/
};

struct ContiniousIteratorMetrics
{
    size_t totalStepCalculations = 0;
    size_t timeIterations = 0;
    size_t complexity = 0;

    double adaptationEfficiency() { return double(timeIterations) / totalStepCalculations; }
};

class TimeHookPeriodic : public ITimeHook
{
public:
    void run_hook(double time) override final;
    double get_next_time() override final;

    void set_period(double period);
    double get_period();

    /**
     * Real hook function that should be defined in derived class.
     * @param realTime Model time of calling hook
     * @param wantedTime Time hook was planned to run. Usually lesser than realTime
     */
    virtual void hook(double realTime, double wantedTime) = 0;

private:
    double m_nextRun = 0;
    double m_lastRun = 0.0;
    double m_period = 1.0;
};

class IntegrationMethodBase : public IIntegrationMethod
{
public:
    void set_variable(IVariable* variable) override;
    void set_logic(IRHS* logic) override;
    void set_time(double time) override;
    double time() override;

    void set_parameters(IntegrationParameters* parameters) override;
    const IntegrationParameters* parameters() override;

    const ContiniousIteratorMetrics& metrics() override;

protected:
    double m_time = 0;
    IVariable *m_variable = nullptr;
    IRHS *m_rhs = nullptr;

    const IntegrationParameters *m_parameters = nullptr;
    ContiniousIteratorMetrics m_metrics;
};

class TimeIterator
{
public:
    TimeIterator();

    void set_continious_iterator(IIntegrationMethod* continious_iterator);
    void set_bifurcator(IBifurcator* bifurcator);

    void set_time(double time);
    void set_bifurcation_run_period(double bifurcationPeriod);
    void set_stop_time(double stopTime);
    void add_hook(ITimeHook* hook);

    void set_step(double dt);
    double get_step();
    double get_stop_time();
    double get_time();

    IntegrationParameters& parameters();

    bool is_done();

    void iterate();
    void run();

    /**
     * Stop after current iteration. May be called from another thread
     */
    void stop();

    IntegrationParameters& continiousIterParameters();

private:
    void call_hook();
    void find_next_hook();

    double m_stopTime = 1.0;
    double m_dt = 0.01;
    double m_bifurcationPeriod = 0;
    double m_lastBifurcationTime = 0;

    double m_nextHookTime = 0;
    size_t m_nextHook = 0;
    bool m_needStop = false;

    IntegrationParameters m_integration_parameters;

    IIntegrationMethod* m_continiousIterator = nullptr;
    IBifurcator* m_bifurcationIterable = nullptr;

    std::vector<ITimeHook*> m_timeHooks;
};

class PeriodicStopHook : public TimeHookPeriodic
{
public:
    PeriodicStopHook(TimeIterator* iterator) : m_iterator(iterator) {}
    void hook(double time, double wantedTime) override { /*UNUSED_ARG(time); UNUSED_ARG(wantedTime);*/ m_iterator->stop(); }

private:
    TimeIterator* m_iterator;
};


#endif // TIME_ITER_HPP
