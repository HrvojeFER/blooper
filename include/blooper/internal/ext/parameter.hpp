#ifndef BLOOPER_PARAMETER_HPP
#define BLOOPER_PARAMETER_HPP

#include <blooper/internal/macros/macros.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline void nudgeParameterUp(
    te::AutomatableParameter& parameter)
{
  parameter.setNormalisedParameter(
      parameter.getCurrentValue() +
          parameter.getValueRange().getLength() / 20,
      juce::sendNotificationAsync);
}

[[maybe_unused]] inline void nudgeParameterDown(
    te::AutomatableParameter& parameter)
{
  parameter.setNormalisedParameter(
      parameter.getCurrentValue() -
          parameter.getValueRange().getLength() / 20,
      juce::sendNotificationAsync);
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_PARAMETER_HPP
