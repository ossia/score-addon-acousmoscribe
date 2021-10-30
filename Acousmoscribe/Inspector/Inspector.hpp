#pragma once
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegateFactory.hpp>

#include <score/command/Dispatchers/CommandDispatcher.hpp>

#include <Acousmoscribe/Model/Process.hpp>

namespace Acousmoscribe
{
class InspectorWidget final
    : public Process::InspectorWidgetDelegate_T<Acousmoscribe::Model>
{
public:
  explicit InspectorWidget(
      const Acousmoscribe::Model& object, const score::DocumentContext& context,
      QWidget* parent);
  ~InspectorWidget() override;

private:
  CommandDispatcher<> m_dispatcher;
};

class InspectorFactory final
    : public Process::InspectorWidgetDelegateFactory_T<Model, InspectorWidget>
{
  SCORE_CONCRETE("9230cad0-53c2-44e6-8d6b-8c495eccde5f")
};
}
