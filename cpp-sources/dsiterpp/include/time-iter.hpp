#ifndef TIME_ITER_HPP
#define TIME_ITER_HPP

#include <vector>
#include <functional>
#include <limits>
#include <cstddef>
#include <cmath>

struct IntegrationParameters;
struct IntegratorMetrics;
class IErrorEstimator;
class IVariable;
class IRHS;
class IIntegrator;
class IntegrationError;
class IBifurcator;

struct StepAdjustmentParameters
{
    bool autoStepAdjustment = false;
    double max_step_limit = 0.001;
    double min_step_limit = 0.00001;
    double relative_deconvergence_speed_min = 0.001;
    double relative_deconvergence_speed_max = 0.01;
    /*double rel_error_per_step_coarsening_treshold = 0.0001;
    double rel_error_per_step_refining_treshold = 0.001;*/

    double step_refining_factor = 0.5;
    double step_coarsening_factor = 1.5;

    bool is_self_consistent();
    void setup_relative_deconvergence_speed(double rel_err_per_second);
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
    virtual void hook(double realTime, double wantedTime) = 0;

private:
    double m_nextRun = 0;
    double m_lastRun = 0.0;
    double m_period = 1.0;
};

class TimeIterator
{
public:
    TimeIterator();

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

    double m_time = 0.0;

    double m_stopTime = 1.0;
    double m_dt = 0.01;
    double m_bifurcationPeriod = 0;
    double m_lastBifurcationTime = 0;

    double m_nextHookTime = 0;
    size_t m_nextHook = 0;
    bool m_needStop = false;

    StepAdjustmentParameters m_step_adj_pars;
    IIntegrator* m_continiousIterator = nullptr;
    IErrorEstimator* m_estimator = nullptr;
    IBifurcator* m_bifurcationIterable = nullptr;

    std::vector<ITimeHook*> m_timeHooks;
    IteratingMetrics m_metrics;
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
