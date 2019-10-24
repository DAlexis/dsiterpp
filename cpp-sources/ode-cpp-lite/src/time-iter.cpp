#include "include/time-iter.hpp"

#include <iostream>

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

void IntegrationMethodBase::set_variable(IVariable* variable)
{
    m_variable = variable;
}

void IntegrationMethodBase::set_logic(IRHS* logic)
{
    m_rhs = logic;
}

void IntegrationMethodBase::set_time(double time)
{
	m_time = time;
}

double IntegrationMethodBase::time()
{
	return m_time;
}


void IntegrationMethodBase::set_parameters(IntegrationParameters* parameters)
{
    m_parameters = parameters;
}

const IntegrationParameters* IntegrationMethodBase::parameters()
{
    return m_parameters;
}

const ContiniousIteratorMetrics& IntegrationMethodBase::metrics()
{
	return m_metrics;
}

/////////////////////////
// TimeIterator

TimeIterator::TimeIterator()
{
    //ASSERT(continiousIterable != nullptr, "continiousIterable cannot be a nullptr");
    //ASSERT(continiousIterator != nullptr, "continiousIterator cannot be a nullptr");
}

void TimeIterator::set_continious_iterator(IIntegrationMethod* continious_iterator)
{
    m_continiousIterator = continious_iterator;
    m_continiousIterator->set_parameters(&m_integration_parameters);
}

void TimeIterator::set_bifurcator(IBifurcator* bifurcator)
{
    m_bifurcationIterable = bifurcator;
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
	return m_continiousIterator->time();
}

IntegrationParameters& TimeIterator::parameters()
{
    return m_integration_parameters;
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

    if (m_integration_parameters.autoStepAdjustment)
    {
        if (m_dt < m_integration_parameters.min_step)
            m_dt = m_integration_parameters.min_step;

        if (m_dt > m_integration_parameters.max_step)
            m_dt = m_integration_parameters.max_step;
    }
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

IntegrationParameters& TimeIterator::continiousIterParameters()
{
    return m_integration_parameters;
}
