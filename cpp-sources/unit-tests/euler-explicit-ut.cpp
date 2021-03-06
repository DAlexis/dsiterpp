#include "exponent-time-iterable.hpp"
#include "dsiterpp/euler-explicit.hpp"
#include <cmath>

#include "gtest/gtest.h"

using namespace dsiterpp;

TEST(EulerExplicit, ExponentDiffEq)
{
    EulerExplicitIterator euler;
    ExponentProblem exp_problem(&euler, 1.0);
    double time_limit = 3.0;
    exp_problem.iterate(time_limit);

    ASSERT_NEAR(exp(time_limit), exp_problem.value(), 0.01);
}
