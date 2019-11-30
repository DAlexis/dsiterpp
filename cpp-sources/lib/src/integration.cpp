#include "dsiterpp/integration.hpp"

using namespace dsiterpp;

/////////////////////////////////
// VariableScalar

VariableScalar::VariableScalar(double value) :
    m_previous_value(value)
{
    clear_subiteration();
}

void VariableScalar::clear_subiteration()
{
    m_current_value = m_previous_value; m_delta = 0.0;
}

void VariableScalar::add_rhs_to_delta(double m)
{
    m_delta += m_rhs * m;
}

void VariableScalar::make_sub_iteration(double dt)
{
    m_current_value = m_previous_value + m_rhs * dt;
}

void VariableScalar::step()
{
    m_current_value = m_previous_value = m_previous_value + m_delta;
    m_delta = 0.0;
}

void VariableScalar::collect_values(std::vector<double>& values) const
{
    values.push_back(m_current_value);
}

void VariableScalar::collect_deltas(std::vector<double>& deltas) const
{
    deltas.push_back(m_delta);
}

void VariableScalar::set_values(std::vector<double>::const_iterator& values)
{
    m_previous_value = *(values++); clear_subiteration();
}

double VariableScalar::current_value()
{
    return m_current_value;
}

void VariableScalar::set_rhs(double rhs)
{
    m_rhs = rhs;
}

VariableScalar::operator double&()
{
    return m_previous_value;
}

void VariablesGroup::add_variable(IVariable& var)
{
    m_variables.push_back(&var);
}

void VariablesGroup::clear_subiteration()
{
    for (auto &var : m_variables) {
        var->clear_subiteration();
    }
}

void VariablesGroup::add_rhs_to_delta(double m)
{
    for (auto &var : m_variables) {
        var->add_rhs_to_delta(m);
    }
}

void VariablesGroup::make_sub_iteration(double dt)
{
    for (auto &var : m_variables) {
        var->make_sub_iteration(dt);
    }
}

void VariablesGroup::step()
{
    for (auto &var : m_variables) {
        var->step();
    }
}

void VariablesGroup::collect_values(std::vector<double>& values) const
{
    for (auto &var : m_variables) {
        var->collect_values(values);
    }
}

void VariablesGroup::collect_deltas(std::vector<double>& deltas) const
{
    for (auto &var : m_variables) {
        var->collect_deltas(deltas);
    }
}

void VariablesGroup::set_values(std::vector<double>::const_iterator& values)
{
    for (auto &var : m_variables) {
        var->set_values(values);
    }
}


/////////////////////////////////
// RHSScalar

RHSScalar::RHSScalar(VariableScalar& scalar, RHSFunction rhs) :
    m_scalar(scalar), m_rhs_function(rhs)
{
}

void RHSScalar::calculate_rhs(double time)
{
    double rhs = m_rhs_function(time, m_scalar.current_value());
    m_scalar.set_rhs(rhs);
}

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
