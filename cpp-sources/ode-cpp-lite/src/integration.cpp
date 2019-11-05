#include "include/integration.hpp"


////////////////////////////////
// ContinuousTimeIteratorBase

void IntegrationMethodBase::set_variable(IVariable* variable)
{
    m_variable = variable;
}

void IntegrationMethodBase::set_rhs(IRHS* logic)
{
    m_rhs = logic;
}

IVariable* IntegrationMethodBase::get_variable()
{
    return m_variable;
}

const IntegratorMetrics& IntegrationMethodBase::metrics()
{
    return m_metrics;
}
