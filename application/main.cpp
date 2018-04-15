#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include "module_info.h"

#include "expat.h"

/* ����� #TWorkflowFileContent ����� ������ �������� ���������� �
 * ��������� ���������� workflow �����.
 */
class TWorkflowFileContent
{
public:
  /* ������ ���������� ����������� workflow ����� � ������.
   *
   */
  int bufferSize;

  /* ��������� ���������� workflow �����.
   *
   */
  char* buffer;

  /* �����������.
   * \param[in] buffer ���������� workflow �����.
   * \param[in] bufferSize ������ ����������� workflow ����� � ������.
   */
  TWorkflowFileContent(char* buffer = NULL, int bufferSize = 0);

  /* ����������.
   *
   */
  ~TWorkflowFileContent();
private:
  /* ����������� �����������. ����������� ���������.
   *
   */
  TWorkflowFileContent(const TWorkflowFileContent&);
};

class TWrapperParserFactory
{
public:
  static TWrapperParser* CreateInstance(const std::string& parserType);
};

class TWrapperParser
{
public:
  virtual ~TWrapperParser() = 0;
  virtual std::vector<TModuleInfo>
    Parse(const std::string& pathToWorkflowFile) = 0;
};

struct TTagHandlers
{
  void(*StartTagHandler)(TXMLTagInfo*, const XML_Char**);
  void(*TagDataHandler)(char*, int);
  void(*EndTagHandler)();

  TTagHandlers(void(*StartTagHandler)(TXMLTagInfo*, const XML_Char**) = NULL,
    void(*TagDataHandler)(char*, int) = NULL,
    void(*EndTagHandler)() = NULL);
  ~TTagHandlers();
};

struct TXMLTagInfo
{
  struct ETagType
  {
    enum Type
    {
      _undefined = 0,

      Workflow,
      Modules,
      ModuleName,
      ExecutionType,
      TransportType,
      ExecutablePath,
      StartCommandLineArgs,
      Argument,
      StopCommandLine,
      ModuleParameters,
      Parameter,
      Name,
      Value,
      EnvironmentVariables,
      Variable,
      InputFileName,
      OutputFileName,
      HasState,
      StateFileName,
      IsTransferable,
      InputBatches,
      InputBatch,
      InputBatchType,
      DistributorName,
      SourceChannels,
      ChannelName,
      InputBatchChannels,
      OutputBatches,
      OutputBatch,
      OutputBatchType,
      CollectorName,
      OutputChannels,
      OutputChannel,
      ChannelConvertedName,
      ReceiverName,
      IsStarting,
      IsFinishing
    };
  };

  ETagType::Type tagType;

  TXMLTagInfo* parentTag;

  std::list<TXMLTagInfo*> childTags;

  std::list<std::pair<std::string, std::string>> tagAttributes;

  TTagHandlers handlers;

  int tagDataLength;

  XML_Char* tagData;

  TXMLTagInfo(ETagType::Type tagType, const TTagHandlers& handlers);

  ~TXMLTagInfo();
};

struct TXMLWorkflowTree
{
  TXMLTagInfo* rootTag;

  TXMLTagInfo* currentTag;

  typedef std::pair<TXMLTagInfo::ETagType::Type, TTagHandlers>
    TTagHandlersInfo;
  typedef std::map<std::string, TTagHandlersInfo> TTagHandlersMap;

  TTagHandlersMap TagName2TagHandlers;

  static const std::string WorkflowTagName;

  static const std::string ModulesTagName;

  static const std::string ModuleTagName;

  static const std::string ExecutionTypeTagName;

  static const std::string TransportTypeTagName;

  static const std::string ExecutablePathTagName;

  static const std::string StartCommandLineArgsTagName;

  static const std::string ArgumentTagName;

  static const std::string StopCommandLineTagName;

  static const std::string ModuleParametersTagName;

  static const std::string ParameterTagName;

  static const std::string NameTagName;

  static const std::string ValueTagName;

  static const std::string EnvironmentVariablesTagName;

  static const std::string VariableTagName;

  static const std::string InputFileNameTagName;

  static const std::string OutputFileNameTagName;

  static const std::string HasStateTagName;

  static const std::string StateFileNameTagName;

  static const std::string IsTransferableTagName;

  static const std::string InputBatchesTagName;

  static const std::string InputBatchTagName;

  static const std::string InputBatchTypeTagName;

  static const std::string DistributorNameTagName;

  static const std::string SourceChannelsTagName;

  static const std::string ChannelNameTagName;

  static const std::string InputBatchChannelsTagName;

  static const std::string OutputBatchesTagName;

  static const std::string OutputBatchTagName;

  static const std::string OutputBatchTypeTagName;

  static const std::string CollectorNameTagName;

  static const std::string OutputChannelsTagName;

  static const std::string OutputChannelTagName;

  static const std::string ChannelConvertedNameTagName;

  static const std::string ReceiverNameTagName;

  static const std::string IsStartingTagName;

  static const std::string IsFinishingTagName;

  TXMLWorkflowTree();

  ~TXMLWorkflowTree();

  void DefaultStartTagHandler(TXMLTagInfo* newTagInfo, const XML_Char** tagAttributes);

  void DefaultTagDataHandler(char* tagData, int tagDataLength);

  void DefaultEndTagHandler();
};

class TWrapperXMLParser : public TWrapperParser
{
public:
  TWrapperXMLParser();

  virtual ~TWrapperXMLParser();

  virtual std::vector<TModuleInfo>
    Parse(const std::string& pathToWorkflowFile);
private:
  XML_Parser parser;

  /* �������, ����������� ������ ����������� workflow ����� � �����.
   * \param[in] pathToWorkflowFile ���� �� workflow �����.
   * \param[in/out] ���������� � ��������� ���������� workflow ����� � ����
   * ������. ���������� ������� ������������ �� ������������ ������, ����������
   * ��� �����.
   */
  void ReadWorkflowFile(const std::string& pathToWorkflowFile,
    TWorkflowFileContent& workflowFileContent);

  static void XMLCALL StartElementHandler(void* workflowXMLTree,
    const XML_Char* tagName, const XML_Char** tagAttributes);

  static void XMLCALL CharacterDataHandler(void* workflowXMLTree,
    const XML_Char* tagData, int tagDataLength);

  static void XMLCALL EndElementHandler(void* workflowXMLTree,
    const XML_Char* tagName);
};

int main(int argc, char* argv[])
{
  std::string pathToWorkflowFile =
    "D:/git-projects/libexpat-example-build/bin/pingpong3_internal_windows_workflow.xml";
  TWrapperParser* parser = TWrapperParserFactory::CreateInstance("XML");
  std::vector<TModuleInfo> modules = parser->Parse(pathToWorkflowFile);

  return 0;
}

TWorkflowFileContent::TWorkflowFileContent(char* buffer, int bufferSize)
{
  this->bufferSize = bufferSize;
  this->buffer = buffer;
}

TWorkflowFileContent::~TWorkflowFileContent()
{
}

TWrapperXMLParser::TWrapperXMLParser()
{
  parser = XML_ParserCreate(NULL);
}

TWrapperXMLParser::~TWrapperXMLParser()
{
  XML_ParserFree(parser);
}

std::vector<TModuleInfo>
  TWrapperXMLParser::Parse(const std::string& pathToWorkflowFile)
{
  TWorkflowFileContent workflowFileContent;
  ReadWorkflowFile(pathToWorkflowFile, workflowFileContent);

  XML_SetElementHandler(parser, &StartElementHandler, EndElementHandler);
  XML_SetCharacterDataHandler(parser, &CharacterDataHandler);

  TXMLWorkflowTree* workflowXMLTree = new TXMLWorkflowTree();
  XML_SetUserData(parser, static_cast<void*>(workflowXMLTree));

  if (XML_Parse(parser, workflowFileContent.buffer,
    workflowFileContent.bufferSize, XML_TRUE) == XML_STATUS_ERROR)
  {
    std::stringstream info;
    info << "Parsing error. Error :" << XML_ErrorString(XML_GetErrorCode(parser));
    throw std::runtime_error(info.str());
  }
  delete[] workflowFileContent.buffer;


  //int modulesCount = EjectModulesCount();
  //std::vector<TModuleInfo> modules(modulesCount);
  //EjectModulesNames(modules);
  //std::map<std::string, TModuleId::TWorkflowId> moduleName2WorkflowId;
  //for (TModuleId::TWorkflowId i = 0; i < modulesCount; ++i)
  //{
  //  moduleName2WorkflowId.insert(std::pair<std::string,
  //    TModuleId::TWorkflowId>(modules[i].name, i + 1));
  //}

  delete workflowXMLTree;
  return std::vector<TModuleInfo>();
}

void TWrapperXMLParser::ReadWorkflowFile(const std::string& pathToWorkflowFile,
  TWorkflowFileContent& workflowFileContent)
{
  std::ifstream workflowFile(pathToWorkflowFile, std::ios::binary);
  if (!workflowFile.is_open())
  {
    std::stringstream info;
    info << "Invalid path to the workflow file. Path :" << pathToWorkflowFile;
    throw std::runtime_error(info.str());
  }

  workflowFile.seekg(0, std::ios::end);
  int bufferSize = workflowFile.tellg();
  workflowFile.seekg(0, std::ios::beg);
  if (bufferSize < 0)
  {
    std::stringstream info;
    info << "tellg() failed during of calculation byte size of workflow " <<
      "file content.";
    throw std::runtime_error(info.str());
  }
  if (bufferSize == 0)
  {
    std::stringstream info;
    info << "Buffer size must be positive.";
    throw std::runtime_error(info.str());
  }
  char* buffer = NULL;
  buffer = new char[bufferSize];
  workflowFile.read(buffer, bufferSize);

  workflowFile.close();

  workflowFileContent.bufferSize = bufferSize;
  workflowFileContent.buffer = buffer;
}

void TWrapperXMLParser::StartElementHandler(void* workflowXMLTree,
  const XML_Char* tagName, const XML_Char** tagAttributes)
{
  TXMLWorkflowTree* XMLTree = static_cast<TXMLWorkflowTree*>(workflowXMLTree);
}

void TWrapperXMLParser::CharacterDataHandler(void* workflowXMLTree,
  const XML_Char* tagData, int tagDataLength)
{
  TXMLWorkflowTree* XMLTree = static_cast<TXMLWorkflowTree*>(workflowXMLTree);
}

void TWrapperXMLParser::EndElementHandler(void* workflowXMLTree,
  const XML_Char* tagName)
{
  TXMLWorkflowTree* XMLTree = static_cast<TXMLWorkflowTree*>(workflowXMLTree);
}

TWrapperParser* TWrapperParserFactory::CreateInstance(const std::string& parserType)
{
  if (parserType == "XML")
  {
    return new TWrapperXMLParser();
  }
  else
  {
    std::stringstream info;
    info << "'" <<parserType << "' parser type is not supported.";
    throw std::runtime_error(info.str());
  }
}

TXMLWorkflowTree::TXMLWorkflowTree() : rootTag(NULL), currentTag(NULL)
{
}

TXMLWorkflowTree::~TXMLWorkflowTree()
{
}

void TXMLWorkflowTree::DefaultStartTagHandler(TXMLTagInfo* newTagInfo,
  const XML_Char** tagAttributes)
{
  newTagInfo->parentTag = currentTag;
  if (currentTag != NULL) // Else it is asssumed that is tag with 'workflow' name
  {
    currentTag->childTags.push_back(newTagInfo);
  }
  currentTag = newTagInfo; // go down
  if (tagAttributes != NULL)
  {
    for (int i = 0; tagAttributes[i]; ++i)
    {
      typedef std::pair<std::string, std::string> TTagAttribute;
      currentTag->tagAttributes.push_back(
        TTagAttribute(std::string(tagAttributes[i]),
          std::string(tagAttributes[i + 1])));
    }
  }
}

void TXMLWorkflowTree::DefaultTagDataHandler(char* tagData, int tagDataLength)
{
  if ((tagData == NULL) || (tagDataLength <= 0))
  {
    std::stringstream info;
    info << "Incorrect input data. Expected not NULL tag data and positive tag " <<
      "data length. Data: " << (tagData == NULL ? "NULL" : "not NULL") << " , " <<
      "length: " << tagDataLength;
    throw std::runtime_error(info.str());
  }

  currentTag->tagDataLength = tagDataLength;
  currentTag->tagData;
}

void TXMLWorkflowTree::DefaultEndTagHandler()
{
  if (currentTag == NULL)
  {
    std::stringstream info;
    info << "NOTICE. Parsing workflow file is finished.";
    return;
  }

  currentTag = currentTag->parentTag; // go up
}

TTagHandlers::TTagHandlers(void(*StartTagHandler)(TXMLTagInfo*, const XML_Char**),
  void(*TagDataHandler)(char*, int), void(*EndTagHandler)()) :
  StartTagHandler(StartTagHandler), TagDataHandler(TagDataHandler),
  EndTagHandler(EndTagHandler)
{
}

TTagHandlers::~TTagHandlers()
{
}

TXMLTagInfo::TXMLTagInfo(ETagType::Type tagType, const TTagHandlers& handlers) :
  tagType(tagType), handlers(handlers)
{
}

TXMLTagInfo::~TXMLTagInfo()
{
}

const std::string TXMLWorkflowTree::WorkflowTagName = "workflow";

const std::string TXMLWorkflowTree::ModulesTagName = "modules";

const std::string TXMLWorkflowTree::ModuleTagName = "module";

const std::string TXMLWorkflowTree::ExecutionTypeTagName = "executionType";

const std::string TXMLWorkflowTree::TransportTypeTagName = "transportType";

const std::string TXMLWorkflowTree::ExecutablePathTagName = "executablePath";

const std::string TXMLWorkflowTree::StartCommandLineArgsTagName =
"startCommandLineArgs";

const std::string TXMLWorkflowTree::ArgumentTagName = "argument";

const std::string TXMLWorkflowTree::StopCommandLineTagName = "stopCommandLine";

const std::string TXMLWorkflowTree::ModuleParametersTagName =
  "moduleParameters";

const std::string TXMLWorkflowTree::ParameterTagName = "parameter";

const std::string TXMLWorkflowTree::NameTagName = "name";

const std::string TXMLWorkflowTree::ValueTagName = "value";

const std::string TXMLWorkflowTree::EnvironmentVariablesTagName =
  "environmentVariables";

const std::string TXMLWorkflowTree::VariableTagName = "variable";

const std::string TXMLWorkflowTree::InputFileNameTagName = "inputFileName";

const std::string TXMLWorkflowTree::OutputFileNameTagName = "outputFileName";

const std::string TXMLWorkflowTree::HasStateTagName = "hasState";

const std::string TXMLWorkflowTree::StateFileNameTagName = "stateFileName";

const std::string TXMLWorkflowTree::IsTransferableTagName = "isTransferable";

const std::string TXMLWorkflowTree::InputBatchesTagName = "inputBatches";

const std::string TXMLWorkflowTree::InputBatchTagName = "inputBatch";

const std::string TXMLWorkflowTree::InputBatchTypeTagName = "inputBatchType";

const std::string TXMLWorkflowTree::DistributorNameTagName = "distributorName";

const std::string TXMLWorkflowTree::SourceChannelsTagName = "sourceChannels";

const std::string TXMLWorkflowTree::ChannelNameTagName = "channelName";

const std::string TXMLWorkflowTree::InputBatchChannelsTagName =
  "inputBatchChannels";

const std::string TXMLWorkflowTree::OutputBatchesTagName = "outputBatches";

const std::string TXMLWorkflowTree::OutputBatchTagName = "outputBatch";

const std::string TXMLWorkflowTree::OutputBatchTypeTagName = "outputBatchType";

const std::string TXMLWorkflowTree::CollectorNameTagName = "collectorName";

const std::string TXMLWorkflowTree::OutputChannelsTagName = "outputChannels";

const std::string TXMLWorkflowTree::OutputChannelTagName = "outputChannel";

const std::string TXMLWorkflowTree::ChannelConvertedNameTagName =
"channelConvertedName";

const std::string TXMLWorkflowTree::ReceiverNameTagName = "receiverName";

const std::string TXMLWorkflowTree::IsStartingTagName = "isStarting";

const std::string TXMLWorkflowTree::IsFinishingTagName = "isFinishing";
