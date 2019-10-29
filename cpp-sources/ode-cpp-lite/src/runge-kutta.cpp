/*
 * runge-kutta.cpp
 *
 *  Created on: 12 янв. 2017 г.
 *      Author: dalexies
 */

#include "include/runge-kutta.hpp"
#include <iostream>

using namespace sotm;
using namespace std;


RungeKuttaIterator::RungeKuttaIterator(double step_precision) :
    m_step_precision(step_precision)
{
}

double RungeKuttaIterator::calculate_delta(double t, double dt)
{
    m_rhs->pre_iteration_job(t);

	for(;;) {
		m_metrics.totalStepCalculations++;
        make_subiterations(t, dt);

        break;
/*
		// If step adjustment disabled
		if (!m_parameters->autoStepAdjustment)
			break;

		// Now checking step to be optimal
        double iterationsCount = m_target->get_minimal_steps_count();
		if (m_parameters->outputVerboseLevel >= ContiniousIteratorParameters::VerboseLevel::more)
		{
			cout << "[R-K4] Min steps count estimated as " << iterationsCount << endl;
		}

		if (iterationsCount == IContinuousTimeIterable::stepsCountNotMatter)
			break;

		if (iterationsCount < m_parameters->iterationsPerAmplitudeMin)
		{
			// Step is too big
			if (dt == m_stepMin)
				break;

            dt *= 0.6;
            if (m_parameters->outputVerboseLevel >= ContiniousIteratorParameters::VerboseLevel::more)
			{
            	cout << "[R-K4] Deacreasing step to " << dt << endl;
			}
			if (dt < m_stepMin)
				dt = m_stepMin;

            m_target->clear_subiteration();
			continue;
		}
		if (iterationsCount > m_parameters->iterationsPerAmplitudeMax)
		{
			// Step is smaller then enough
			if (dt == m_stepMax)
				break;

            dt *= 1.5;
			if (dt > m_stepMax)
				dt = m_stepMax;
			if (m_parameters->outputVerboseLevel >= ContiniousIteratorParameters::VerboseLevel::more)
			{
				cout << "[R-K4] Increasing step to " << dt << endl;
			}

			// But we should not recompute this iteration, it is done with step precise then enough
			//m_target->clearSubiteration();
			//continue;
		}
        break;*/
	}

	m_metrics.timeIterations++;
    //m_metrics.complexity +=
	//cout << (int)m_parameters->outputVerboseLevel << endl;
    /*
	if (m_parameters->outputVerboseLevel != ContiniousIteratorParameters::VerboseLevel::none)
	{
		cout << "[R-K4] Iteration done. t = " << m_time
				<< ", dt = " << dt
				<< ", efficiency = " << m_metrics.adaptationEfficiency() << endl;
    }*/
	return dt;
}

void RungeKuttaIterator::make_subiterations(double t, double dt)
{
	// k1 = f(tn, xn)
    //m_target->calculate_secondary_values(m_time);
    m_rhs->pre_sub_iteration_job(t);
    m_rhs->calculate_rhs(t);
    m_variable->add_rhs_to_delta(dt / 6.0);

	// k2 = f(tn + dt/2, xn + dt/2*k1)
    m_variable->make_sub_iteration(dt / 2.0);
    m_rhs->pre_sub_iteration_job(t + dt / 2.0);
    m_rhs->calculate_rhs(t + dt / 2.0);
    m_variable->add_rhs_to_delta(dt / 3.0);

	// k3 = f(tn + dt/2, xn + dt/2*k2)
    m_variable->make_sub_iteration(dt / 2.0);
    m_rhs->pre_sub_iteration_job(t + dt / 2.0);
    m_rhs->calculate_rhs(t + dt / 2.0);
    m_variable->add_rhs_to_delta(dt / 3.0);

	// k3 = f(tn + dt, xn + dt*k3)
    m_variable->make_sub_iteration(dt);
    m_rhs->pre_sub_iteration_job(t + dt);
    m_rhs->calculate_rhs(t + dt);
    m_variable->add_rhs_to_delta(dt / 6.0);
}
