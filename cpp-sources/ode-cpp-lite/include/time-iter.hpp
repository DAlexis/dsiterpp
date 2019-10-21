#ifndef TIME_ITER_HPP
#define TIME_ITER_HPP

#include <vector>
#include <limits>
#include <cstddef>
#include <cmath>

struct ContiniousIteratorParameters;
struct ContiniousIteratorMetrics;

/**
 * This class represent some lines from system x'=f(time, x)
 * This interface suppose that users implementation contains those vectors (or may be named values):
 * xPrevious
 * xCurrent
 * rhs
 * xDelta
 *
 * All operations over this interface are system structure-safe
 */
class IContinuousTimeIterable
{
public:
    constexpr static double stepsCountNotMatter = std::numeric_limits<double>::max();
    virtual ~IContinuousTimeIterable() { }


    virtual void clear_subiteration() = 0;

    /** Calculate secondary values thats are trivial function of quantities and does not
     * contain any differential operators. This function should be called for all objects
     * before calling calculateRHS
     */
    virtual void calculate_secondary_values(double time) = 0;

    /** Calculate right hand side with values of
     * f(time, xCurrent, xCurrent of other objects, secondaryValues of other objects)
     */
    virtual void calculate_rhs(double time) = 0;

    /**
     * Adds rhs multiplied by m to xDelta
     */
    virtual void add_rhs_to_delta(double m) = 0;

    /**
     * Makes xCurrent = xPrevious + rhs*dt
     */
    virtual void make_sub_iteration(double dt) = 0;

    /*
     * Makes xCurrent = xPrevious = xPrevious + xDelta; xDelta = 0;
     */
    virtual void step() = 0;

    /**
     * Function used for precision control.
     *
     * Get square of norm of difference between delta and last delta
     */
    virtual double get_delta_difference_norm_sqr() { return 0.0; }

    /**
     * Function used for precision control.
     *
     * Get square of norm of difference between delta and last delta
     */
    virtual double get_delta_norm_sqr() { return 0.0; }

    /**
     * Function used for precision control.
     *
     * Get minimal steps count needed to reach zero from maximal value
     * for any variable
     */
    virtual double get_minimal_steps_count() { return stepsCountNotMatter; }
};

template<typename VectorType>
class VectorVariable : public IContinuousTimeIterable
{
public:

private:

};

class IBifurcationTimeIterable
{
public:
    virtual ~IBifurcationTimeIterable() {}

    virtual void prepare_bifurcation(double time, double dt) = 0;
    virtual void do_bifurcation(double time, double dt) = 0;
};

class IContinuousTimeIterator
{
public:
    virtual ~IContinuousTimeIterator() {}
    virtual void set_iterable(IContinuousTimeIterable* target) = 0;
    /**
     * Make one iteration. If timestep adaptation enabled, timestep may be changed
     * @return new timestep
     */
    virtual double iterate(double dt) = 0;
    virtual void set_time(double time) = 0;
    virtual double time() = 0;
    virtual void set_step_bounds(double min, double max) = 0;
    virtual double get_min_step() = 0;
    virtual double get_max_step() = 0;
    virtual void set_parameters(ContiniousIteratorParameters* parameters) = 0;
    virtual const ContiniousIteratorMetrics& metrics() = 0;
};

class ITimeHook
{
public:
    virtual ~ITimeHook() {}
    virtual void run_hook(double time) = 0;
    virtual double get_next_time() = 0;
};

class Variable
{
public:
    Variable(double initValue = 0.0) :
        previous(initValue), current(initValue)
    {
        update_max_abs();
    }

    void clear_sub_iteration() { current = previous; delta = 0.0; }
    void make_sub_iteration(double dt) { current = previous + rhs * dt; }
    void add_rhs_to_delta(double m) { delta += rhs * m; }
    void step()
    {
        current = previous = previous + delta;
        delta = 0.0;
        update_max_abs();
    }
    void set_initial(double value) { previous = current = value; }

    double get_current_steps_count()
    {
        if (maxAbs == 0.0 || delta*previous > 0)
            return IContinuousTimeIterable::stepsCountNotMatter;
        return maxAbs / fabs(delta);
    }

    void set(double value)
    {
        previous = current = value;
        delta = 0;
        rhs = 0;
    }

    double previous;
    double current;
    double delta = 0.0;
    double rhs = 0.0;

private:
    double maxAbs = 0.0;

     void update_max_abs()
    {
        double abs = fabs(previous);
        if (abs > maxAbs)
            maxAbs = abs;
    }
};

struct ContiniousIteratorParameters
{
    bool autoStepAdjustment = false;
    double iterationsPerAmplitudeMin = 3;
    double iterationsPerAmplitudeMax = 20;
    enum class VerboseLevel {
        none = 0, less, more
    };
    VerboseLevel outputVerboseLevel = VerboseLevel::none;
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

class ContinuousTimeIteratorBase : public IContinuousTimeIterator
{
public:
    void set_iterable(IContinuousTimeIterable* target) override;
    void set_time(double time) override;
    double time() override;
    void set_step_bounds(double min, double max) override;
    double get_min_step() override;
    double get_max_step() override;
    void set_parameters(ContiniousIteratorParameters* parameters) override;
    const ContiniousIteratorMetrics& metrics() override;

protected:
    double m_time = 0;
    IContinuousTimeIterable* m_target = nullptr;
    double m_stepMin = 0.0, m_stepMax = 0.0;
    static const ContiniousIteratorParameters defaultParameters;
    const ContiniousIteratorParameters *m_parameters = &defaultParameters;
    ContiniousIteratorMetrics m_metrics;
};

class TimeIterator
{
public:
    TimeIterator(
        IContinuousTimeIterable* continiousIterable,
        IContinuousTimeIterator* continiousIterator,
        IBifurcationTimeIterable* bifurcationIterable = nullptr
    );

    void set_time(double time);
    void set_bifurcation_run_period(double bifurcationPeriod);
    void set_step(double dt);
    void set_step_bounds(double min, double max);
    void set_stop_time(double stopTime);
    void add_hook(ITimeHook* hook);

    double get_step();
    double get_timestep_min();
    double get_timestep_max();
    double get_stop_time();
    double get_time();

    bool is_done();

    void iterate();
    void run();

    /**
     * Stop after current iteration. May be called from another thread
     */
    void stop();

    ContiniousIteratorParameters& continiousIterParameters();

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

    ContiniousIteratorParameters m_contIteratorParameters;

    IContinuousTimeIterable* m_continiousIterable;
    IContinuousTimeIterator* m_continiousIterator;
    IBifurcationTimeIterable* m_bifurcationIterable;

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
