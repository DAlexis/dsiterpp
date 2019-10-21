#include "include/time-iter.hpp"

#include <iostream>

const ContiniousIteratorParameters ContinuousTimeIteratorBase::defaultParameters;

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

////////////////////////////////
// ContinuousTimeIteratorBase

void ContinuousTimeIteratorBase::set_iterable(IContinuousTimeIterable* target)
{
	m_target = target;
}

void ContinuousTimeIteratorBase::set_time(double time)
{
	m_time = time;
}

double ContinuousTimeIteratorBase::time()
{
	return m_time;
}

void ContinuousTimeIteratorBase::set_step_bounds(double min, double max)
{
	m_stepMin = min;
	m_stepMax = max;
}

double ContinuousTimeIteratorBase::get_min_step()
{
	return m_stepMin;
}

double ContinuousTimeIteratorBase::get_max_step()
{
	return m_stepMax;
}

void ContinuousTimeIteratorBase::set_parameters(ContiniousIteratorParameters* parameters)
{
	if (parameters)
		m_parameters = parameters;
	else
		m_parameters = &defaultParameters;
}

const ContiniousIteratorMetrics& ContinuousTimeIteratorBase::metrics()
{
	return m_metrics;
}

/////////////////////////
// TimeIterator

TimeIterator::TimeIterator(
		IContinuousTimeIterable* continiousIterable,
		IContinuousTimeIterator* continiousIterator,
		IBifurcationTimeIterable* bifurcationIterable
	) :
	m_continiousIterable(continiousIterable),
	m_continiousIterator(continiousIterator),
	m_bifurcationIterable(bifurcationIterable)
{
    //ASSERT(continiousIterable != nullptr, "continiousIterable cannot be a nullptr");
    //ASSERT(continiousIterator != nullptr, "continiousIterator cannot be a nullptr");
    m_continiousIterator->set_iterable(m_continiousIterable);
    m_continiousIterator->set_parameters(&m_contIteratorParameters);
}

void TimeIterator::set_time(double time)
{
    m_continiousIterator->set_time(time);
	m_lastBifurcationTime = time;
}

void TimeIterator::set_bifurcation_run_period(double bifurcationPeriod)
{
	m_bifurcationPeriod = bifurcationPeriod;
}

void TimeIterator::set_step(double dt)
{
	m_dt = dt;
    m_continiousIterator->set_step_bounds(m_dt, m_dt);
}

void TimeIterator::set_step_bounds(double min, double max)
{
    m_continiousIterator->set_step_bounds(min, max);
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

double TimeIterator::get_step()
{
	return m_dt;
}

double TimeIterator::get_timestep_min()
{
    return m_continiousIterator->get_min_step();
}

double TimeIterator::get_timestep_max()
{
    return m_continiousIterator->get_max_step();
}

double TimeIterator::get_stop_time()
{
	return m_stopTime;
}

double TimeIterator::get_time()
{
	return m_continiousIterator->time();
}

bool TimeIterator::is_done()
{
	return m_continiousIterator->time() >= m_stopTime;
}

void TimeIterator::iterate()
{
	double time = m_continiousIterator->time();
	m_dt = m_continiousIterator->iterate(m_dt);

	if (
			(m_bifurcationPeriod == 0.0 || time - m_lastBifurcationTime >= m_bifurcationPeriod) &&
			m_bifurcationIterable != nullptr
		)
	{
		double dt = time - m_lastBifurcationTime;
        m_bifurcationIterable->prepare_bifurcation(time, dt);
        m_bifurcationIterable->do_bifurcation(time, dt);
		m_lastBifurcationTime = time;
	}
    call_hook();
}

void TimeIterator::call_hook()
{
	if (m_timeHooks.empty())
		return;
	double time = m_continiousIterator->time();
	if (time >= m_nextHookTime)
	{
        m_timeHooks[m_nextHook]->run_hook(time);
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

void TimeIterator::stop()
{
	m_needStop = true;
}

ContiniousIteratorParameters& TimeIterator::continiousIterParameters()
{
	return m_contIteratorParameters;
}
