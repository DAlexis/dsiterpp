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
    RungeKuttaIterator rk;
    ExponentProblem exp_problem(&rk, 1.0);
    double time_limit = 3.0;
    exp_problem.iterate(time_limit);

    ASSERT_NEAR(exp(time_limit), exp_problem.value(), 0.01);
}

TEST(RungeKutta, ComparisionWithEuler)
{
    double time_limit = 3.0;
	double deltaRunge, deltaEuler;
	{
        RungeKuttaIterator rk;
        ExponentProblem exp_problem(&rk, 1.0);

        exp_problem.iterate(time_limit);
        deltaRunge = fabs(exp(time_limit)- exp_problem.value());
	}
	{

		EulerExplicitIterator euler;
        ExponentProblem exp_problem(&euler, 1.0);

        exp_problem.iterate(time_limit);
        deltaEuler = fabs(exp(time_limit)- exp_problem.value());
	}
	ASSERT_LE(deltaRunge, deltaEuler);
	ASSERT_LE(deltaRunge / deltaEuler, 1e-4);
}

