#include "exponent-time-iterable.hpp"
#include "include/euler-explicit.hpp"
#include <cmath>

#include "gtest/gtest.h"

TEST(EulerExplicit, ExponentDiffEq)
{
	Exponent e;
	EulerExplicitIterator euler;
	TimeIterator iter(&e, &euler);

	double timeLimit = 3.0;
    iter.set_time(0.0);
    iter.set_step(0.0001);
    iter.set_stop_time(timeLimit);
	iter.run();
	ASSERT_NEAR(exp(timeLimit), e.getValue(), 0.01);
}
