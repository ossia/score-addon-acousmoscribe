#pragma once
#include <Process/Execution/ProcessComponent.hpp>

#include <ossia/dataflow/node_process.hpp>

namespace Acousmoscribe
{
class Model;
class ProcessExecutorComponent final
    : public Execution::ProcessComponent_T<
          Acousmoscribe::Model, ossia::node_process>
{
  COMPONENT_METADATA("918a0fb8-1844-42db-b470-a88494441bca")
public:
  ProcessExecutorComponent(
      Model& element, const Execution::Context& ctx, QObject* parent);
};

using ProcessExecutorComponentFactory
    = Execution::ProcessComponentFactory_T<ProcessExecutorComponent>;
}
