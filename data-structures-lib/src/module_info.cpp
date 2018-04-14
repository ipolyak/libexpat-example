#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "module_info.h"


const TModuleId::TWorkflowId TModuleId::WorkflowIdUndefined = 0;
const TModuleId::TInstanceId TModuleId::InstanceIdUndefined = 0;

TModuleId::TModuleId(const TWorkflowId& workflowId,
  const TInstanceId& instanceId) :
  workflowId(workflowId),
  instanceId(instanceId)
{
}

bool TModuleId::operator==(const TModuleId& other) const
{
  return (workflowId == other.workflowId) &&
    (instanceId == other.instanceId);
}

bool TModuleId::operator!=(const TModuleId& other) const
{
  return !operator == (other);
}


TInputBatchInfo::TInputBatchInfo() :
  sourceChannels(),
  source(TModuleId::WorkflowIdUndefined),
  channels(),
  type(EInputBatchType::_undefined)
{
}

TOutputBatchInfo::TOutputMessageChannelInfo::TOutputMessageChannelInfo() :
  receiver(TModuleId::WorkflowIdUndefined),
  name(),
  convertedName()
{
}

TOutputBatchInfo::TOutputMessageChannelInfo::TOutputMessageChannelInfo(
  const TModuleId::TWorkflowId& receiver,
  const std::string& name, const std::string& convertedName) :
  receiver(receiver),
  name(name),
  convertedName(convertedName)
{
}

TOutputBatchInfo::TOutputBatchInfo() :
  receiver(TModuleId::WorkflowIdUndefined),
  channels(),
  type(EOutputBatchType::_undefined)
{
}


TModuleInfo::TModuleInfo() :
  name(),
  id(TModuleId::WorkflowIdUndefined, TModuleId::InstanceIdUndefined),
  executionType(EExecutionType::_undefined),
  transportType(ETransportType::_undefined),
  executablePath(), startCommandLineArgs(), stopCommandLine(),
  parameters(), environmentVariables(),
  inputFileName(), outputFileName(),
  isTransferable(false), hasState(false),
  stateFileName(), tempDirectoryPath(),
  inputBatches(), outputBatches(),
  isStarting(false), isFinishing(false)
{
  
  
}
