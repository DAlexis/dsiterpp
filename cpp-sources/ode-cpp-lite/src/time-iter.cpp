#include "include/time-iter.hpp"
#include "include/integration.hpp"
#include "include/bifurcation.hpp"
#include "include/error-estimator.hpp"

#include <iostream>

IteratingMetrics::IteratingMetrics()
{
}


void IteratingMetrics::reset()
{
    (*this) = IteratingMetrics();
}

void TimeHookPeriodic::run_hook(double time)
{
	std::cout << "Next run: " << m_nextRun << std::endl;
	//m_lastRun = time;
	hook(time, m_nextRun);
	m_nextRun += m_period;
}

double TimeHookPeriodic::get_next_time()
{
	return m_nextRun;
}

void TimeHookPeriodic::set_period(double period)
{
	m_period = period;
	m_nextRun = m_period;
}

double TimeHookPeriodic::get_period()
{
	return m_period;
}

/////////////////////////
// TimeIterator

TimeIterator::TimeIterator()
{
    //ASSERT(continiousIterable != nullptr, "continiousIterable cannot be a nullptr");
    //ASSERT(continiousIterator != nullptr, "continiousIterator cannot be a nullptr");
}

void TimeIterator::set_continious_iterator(IIntegrator* continious_iterator)
{
    m_continiousIterator = continious_iterator;
}

void TimeIterator::set_error_estimator(IErrorEstimator* estimator)
{
    m_estimator = estimator;
}

void TimeIterator::set_bifurcator(IBifurcator* bifurcator)
{
    m_bifurcationIterable = bifurcator;
}

StepAdjustmentParameters& TimeIterator::step_adj_pars()
{
    return m_step_adj_pars;
}

void TimeIterator::set_time(double time)
{
	m_lastBifurcationTime = time;
}

void TimeIterator::set_bifurcation_run_period(double bifurcationPeriod)
{
	m_bifurcationPeriod = bifurcationPeriod;
}

void TimeIterator::set_stop_time(double stopTime)
{
	m_stopTime = stopTime;
}

void TimeIterator::add_hook(ITimeHook* hook)
{
	m_timeHooks.push_back(hook);
    find_next_hook();
}

void TimeIterator::set_step(double dt)
{
    m_dt = dt;
}

double TimeIterator::get_step()
{
    return m_dt;
}

double TimeIterator::get_stop_time()
{
	return m_stopTime;
}

double TimeIterator::get_time()
{
    return m_time;
}

bool TimeIterator::is_done()
{
    return m_time >= m_stopTime;
}

void TimeIterator::iterate()
{
    double next_dt = integrate_iteration();
    bifurcate_iteration();
    call_hook();

    m_metrics.time_steps_log.push_back(m_dt);

    m_time += m_dt;
    m_dt = next_dt;
}


double TimeIterator::integrate_iteration()
{
    if (m_step_adj_pars.autoStepAdjustment && m_estimator == nullptr)
        throw std::logic_error("Error estimator must be set to use auto step ajustment");

    double next_dt = m_dt;

    if (m_step_adj_pars.autoStepAdjustment)
    {
        m_estimator->set_integrator(m_continiousIterator);
        bool error_is_ok = false;
        do {
            m_estimator->calculate_delta_and_estimate(m_time, m_dt);
            auto error = m_estimator->get_error();
            error_is_ok = error.max_rel_error < m_step_adj_pars.rel_error_per_step_refining_treshold;
            if (!error_is_ok)
            {
                m_dt *= m_step_adj_pars.step_refining_factor;
                next_dt = m_dt;
                m_continiousIterator->get_variable()->clear_subiteration();
                if (m_dt < m_step_adj_pars.min_step_limit)
                {
                    // throttling
                    // @todo Add metrtic increment
                    m_dt = m_step_adj_pars.min_step_limit;
                    next_dt = m_dt;
                    m_metrics.min_step_limitations++;
                    break;
                }
            }

            if (error.max_rel_error < m_step_adj_pars.rel_error_per_step_coarsening_treshold)
            {
                next_dt = m_dt * m_step_adj_pars.step_coarsening_factor;
                if (next_dt > m_step_adj_pars.max_step_limit)
                {
                    next_dt = m_step_adj_pars.max_step_limit;
                    m_metrics.max_step_limitations++;
                }
            }
        } while (!error_is_ok);
        m_continiousIterator->get_variable()->step();
    } else {
        m_continiousIterator->calculate_delta(m_time, m_dt);
        m_continiousIterator->get_variable()->step();
    }
    return next_dt;
}

void TimeIterator::bifurcate_iteration()
{
    if (
            (m_bifurcationPeriod == 0.0 || m_time - m_lastBifurcationTime >= m_bifurcationPeriod) &&
            m_bifurcationIterable != nullptr
        )
    {
        double dt = m_time - m_lastBifurcationTime;
        m_bifurcationIterable->prepare_bifurcation(m_time, dt);
        m_bifurcationIterable->do_bifurcation(m_time, dt);
        m_lastBifurcationTime = m_time;
    }
}

void TimeIterator::call_hook()
{
	if (m_timeHooks.empty())
		return;

    if (m_time >= m_nextHookTime)
	{
        m_timeHooks[m_nextHook]->run_hook(m_time);
        find_next_hook();
	}
}

void TimeIterator::find_next_hook()
{
	if (m_timeHooks.empty())
		return;
    m_nextHookTime = m_timeHooks[0]->get_next_time();
	m_nextHook = 0;
	for (size_t i=1; i<m_timeHooks.size(); i++)
	{
        double candidatTime = m_timeHooks[i]->get_next_time();
		if (candidatTime < m_nextHookTime) {
			m_nextHook = i;
			m_nextHookTime = candidatTime;
		}
	}
}

void TimeIterator::run()
{
	m_needStop = false;
    find_next_hook();
	// std::cout << "Next hook: " << m_nextHookTime << std::endl;
    while (!is_done() && !m_needStop)
		iterate();
}

const IteratingMetrics& TimeIterator::metrics()
{
    return m_metrics;
}

void TimeIterator::reset_metrics()
{
   m_metrics.reset();
}


void TimeIterator::stop()
{
	m_needStop = true;
}
