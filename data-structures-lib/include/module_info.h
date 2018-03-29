#ifndef MODULE_INFO_H_
#define MODULE_INFO_H_

#include <cstddef>
#include <map>
#include <string>
#include <vector>


struct TModuleId
{
  typedef std::size_t TWorkflowId;
  static const TWorkflowId WorkflowIdUndefined;

  typedef std::size_t TInstanceId;
  static const TInstanceId InstanceIdUndefined;

  TWorkflowId workflowId;
  TInstanceId instanceId;


  TModuleId(const TWorkflowId& workflowId = WorkflowIdUndefined,
    const TInstanceId& instanceId = InstanceIdUndefined);
  bool operator==(const TModuleId& other) const;
  bool operator!=(const TModuleId& other) const;
};

struct EExecutionType
{
  enum Type
  {
    _undefined = 0,

    Internal,
    External
  };
};

struct ETransportType
{
  enum Type
  {
    _undefined = 0,

    Pipe,
    File
  };
};

struct EInputBatchType
{
  enum Type
  {
    _undefined = 0,

    Regular,
    Collector,
  };
};

struct TInputBatchInfo
{
  std::vector<std::string> sourceChannels;
  TModuleId::TWorkflowId source;
  std::vector<std::string> channels;
  EInputBatchType::Type type;

  TInputBatchInfo();
};

struct EOutputBatchType
{
  enum Type
  {
    _undefined = 0,

    Regular,
    Distributor
  };
};

struct TOutputBatchInfo
{
  TModuleId::TWorkflowId receiver;
  struct TOutputMessageChannelInfo
  {
    TModuleId::TWorkflowId receiver;
    std::string name;
    std::string convertedName;
    TOutputMessageChannelInfo();
    TOutputMessageChannelInfo(const TModuleId::TWorkflowId& receiver,
      const std::string& name, const std::string& convertedName);
  };
  std::vector<TOutputMessageChannelInfo> channels;
  EOutputBatchType::Type type;

  TOutputBatchInfo();
};

struct TModuleInfo
{
  std::string name;
  TModuleId id;
  EExecutionType::Type executionType;
  ETransportType::Type transportType;
  std::string executablePath;
  std::vector<std::string> startCommandLineArgs;
  std::string stopCommandLine;
  std::vector< std::pair<std::string, std::string> > parameters;
  std::map<std::string, std::string> environmentVariables;
  std::string inputFileName;
  std::string outputFileName;
  bool isTransferable;
  bool hasState;
  std::string stateFileName;
  std::string tempDirectoryPath;
  std::vector<TInputBatchInfo> inputBatches;
  std::vector<TOutputBatchInfo> outputBatches;
  bool isStarting;
  bool isFinishing;

  TModuleInfo();
};

#endif // MODULE_INFO_H_
