#include "exponent-time-iterable.hpp"
#include "include/runge-kutta.hpp"
#include "include/euler-explicit.hpp"
#include <cmath>

#include "gtest/gtest.h"

#include <iostream>

using namespace sotm;
using namespace std;

TEST(RungeKutta, ExponentDiffEq)
{
	Exponent e;
	RungeKuttaIterator euler;
	TimeIterator iter(&e, &euler);

	double timeLimit = 3.0;
    iter.set_time(0.0);
    iter.set_step(0.0001);
    iter.set_stop_time(timeLimit);
	iter.run();
	ASSERT_NEAR(exp(timeLimit), e.getValue(), 0.01);
}

TEST(RungeKutta, ComparisionWithEuler)
{
	double deltaRunge, deltaEuler;
	{
		Exponent e;
		RungeKuttaIterator euler;
		TimeIterator iter(&e, &euler);

		double timeLimit = 3.0;
        iter.set_time(0.0);
        iter.set_step(0.0001);
        iter.set_stop_time(timeLimit);
		iter.run();
		deltaRunge = fabs(exp(timeLimit)- e.getValue());
	}
	{
		Exponent e;
		EulerExplicitIterator euler;
		TimeIterator iter(&e, &euler);

		double timeLimit = 3.0;
        iter.set_time(0.0);
        iter.set_step(0.0001);
        iter.set_stop_time(timeLimit);
		iter.run();
		deltaEuler = fabs(exp(timeLimit)- e.getValue());
	}
	ASSERT_LE(deltaRunge, deltaEuler);
	ASSERT_LE(deltaRunge / deltaEuler, 1e-4);
}
