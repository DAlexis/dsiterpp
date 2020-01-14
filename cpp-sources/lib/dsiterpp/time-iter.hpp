#ifndef TIME_ITER_HPP
#define TIME_ITER_HPP

#include "utils.hpp"

#include <vector>
#include <functional>
#include <limits>
#include <cstddef>
#include <cmath>

namespace dsiterpp {

class IErrorEstimator;
class IVariable;
class IRHS;
class IIntegrator;
class IBifurcator;

struct StepAdjustmentParameters
{
    bool is_self_consistent();
    void setup_relative_deconvergence_speed(double rel_err_per_second);

    bool autoStepAdjustment = false;
    double max_step_limit = 0.001;
    double min_step_limit = 0.00001;
    double relative_deconvergence_speed_min = 0.001;
    double relative_deconvergence_speed_max = 0.01;

    double step_refining_factor = 0.5;
    double step_coarsening_factor = 1.5;
};

struct IteratingMetrics
{
    IteratingMetrics();
    void reset();

    std::vector<double> time_steps_log;
    size_t max_step_limitations = 0;
    size_t min_step_limitations = 0;
};

class ITimeHook
{
public:
    virtual ~ITimeHook() {}
    virtual void run_hook(double time) = 0;
    virtual double get_next_time() = 0;
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
    virtual void hook(double real_time, double wanted_time) = 0;

private:
    double m_nextRun = 0;
    double m_period = 1.0;

protected:
    double m_lastRun = 0.0;
};

class TimeHookPeriodicFunc : public TimeHookPeriodic
{
public:
    using HookFunc = std::function<void(double real_time, double wanted_time)>;

    TimeHookPeriodicFunc(HookFunc tagret_func);
    void hook(double real_time, double wanted_time) override;

private:
    HookFunc m_hook_func;
};

class TimeIterator
{
public:
    TimeIterator();

    void set_variable(IVariable* variable);
    void set_rhs(IRHS* rhs);
    void set_continious_iterator(IIntegrator* continious_iterator);
    void set_error_estimator(IErrorEstimator* estimator);
    void set_bifurcator(IBifurcator* bifurcator);

    StepAdjustmentParameters& step_adj_pars();

    void set_time(double time);
    void set_bifurcation_run_period(double bifurcationPeriod);
    void set_stop_time(double stopTime);
    void add_hook(ITimeHook* hook);

    void set_step(double dt);
    double get_step();
    double get_stop_time();
    double get_time();

    bool is_done();

    void iterate();
    void run();

    const IteratingMetrics& metrics();
    void reset_metrics();

    /**
     * Stop after current iteration. May be called from another thread
     */
    void stop();

private:
    /**
     * Integrate ODE with step adjusting. Timestep (m_dt) may be changed
     * @return time step for NEXT iteration
     */
    double integrate_iteration();
    void bifurcate_iteration();
    void call_hook();
    void find_next_hook();

    void assert_pointers_are_set();

    double m_time = 0.0;

    double m_stopTime = 1.0;
    double m_dt = 0.01;
    double m_bifurcationPeriod = 0;
    double m_lastBifurcationTime = 0;

    double m_nextHookTime = 0;
    size_t m_nextHook = 0;
    bool m_needStop = false;

    StepAdjustmentParameters m_step_adj_pars;

    IVariable* m_variable = nullptr;
    IRHS* m_rhs = nullptr;
    IIntegrator* m_continiousIterator = nullptr;
    IErrorEstimator* m_estimator = nullptr;
    IBifurcator* m_bifurcationIterable = nullptr;

    std::vector<ITimeHook*> m_timeHooks;
    IteratingMetrics m_metrics;
};

class PeriodicStopHook : public TimeHookPeriodic
{
public:
    PeriodicStopHook(TimeIterator* iterator);
    void hook(double time, double wantedTime) override;

private:
    TimeIterator* m_iterator;
};

}

#endif // TIME_ITER_HPP
