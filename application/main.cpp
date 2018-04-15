#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include "module_info.h"

#include "expat.h"

/* Класс #TWorkflowFileContent задаёт способ хранения информации о
 * считанном содержимом workflow файла.
 */
class TWorkflowFileContent
{
public:
  /* Размер считанного содержимого workflow файла в байтах.
   *
   */
  int bufferSize;

  /* Считанное содержимое workflow файла.
   *
   */
  char* buffer;

  /* Конструктор.
   * \param[in] buffer Содержимое workflow файла.
   * \param[in] bufferSize Размер содержимого workflow файла в байтах.
   */
  TWorkflowFileContent(char* buffer = NULL, int bufferSize = 0);

  /* Деструктор.
   *
   */
  ~TWorkflowFileContent();
private:
  /* Конструктор копирования. Копирование запрещено.
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
  void(*StartTagHandler)(TXMLWorkflowTree*, TXMLTagInfo*, const XML_Char**);
  void(*TagDataHandler)(TXMLWorkflowTree*, const XML_Char*, int);
  void(*EndTagHandler)(TXMLWorkflowTree*);

  TTagHandlers(
    void(*StartTagHandler)(TXMLWorkflowTree*, TXMLTagInfo*,
      const XML_Char**) = NULL,
    void(*TagDataHandler)(TXMLWorkflowTree*, const XML_Char*, int) = NULL,
    void(*EndTagHandler)(TXMLWorkflowTree*) = NULL);
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
      Module,
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

  TTagHandlersMap tagName2TagHandlers;

  static const std::string WorkflowTagName;

  static const std::string ModulesTagName;

  static const std::string ModuleTagName;

  static const std::string ModuleNameTagName;

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

  static void DefaultStartTagHandler(TXMLWorkflowTree* workflowXMLTree,
    TXMLTagInfo* newTagInfo, const XML_Char** tagAttributes);

  static void DefaultTagDataHandler(TXMLWorkflowTree* workflowXMLTree,
    const XML_Char* tagData, int tagDataLength);

  static void DefaultEndTagHandler(TXMLWorkflowTree* workflowXMLTree);
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

  /* Функция, выполняющая запись содержимого workflow файла в буфер.
   * \param[in] pathToWorkflowFile Путь до workflow файла.
   * \param[in/out] Информация о считанное содержимое workflow файла в виде
   * буфера. Вызывающая сторона ответственна за освобождение памяти, выделенной
   * под буфер.
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
  if (XMLTree == NULL)
  {
    std::stringstream info;
    info << "Expected not NULL workflow XML tree.";
    throw std::runtime_error(info.str());
  }

  std::string tagNameStr(tagName);
  TXMLWorkflowTree::TTagHandlersInfo& tagHandlersInfo =
    XMLTree->tagName2TagHandlers[tagNameStr];
  TXMLTagInfo* XMLTagInfo = new TXMLTagInfo(tagHandlersInfo.first,
    tagHandlersInfo.second);
  if (XMLTagInfo->handlers.StartTagHandler != NULL)
  {
    XMLTagInfo->handlers.StartTagHandler(XMLTree, XMLTagInfo, tagAttributes);
  }
}

void TWrapperXMLParser::CharacterDataHandler(void* workflowXMLTree,
  const XML_Char* tagData, int tagDataLength)
{
  TXMLWorkflowTree* XMLTree = static_cast<TXMLWorkflowTree*>(workflowXMLTree);
  if (XMLTree == NULL)
  {
    std::stringstream info;
    info << "Expected not NULL workflow XML tree.";
    throw std::runtime_error(info.str());
  }

  if (XMLTree->currentTag->handlers.TagDataHandler != NULL)
  {
    XMLTree->currentTag->handlers.TagDataHandler(XMLTree, tagData,
      tagDataLength);
  }
}

void TWrapperXMLParser::EndElementHandler(void* workflowXMLTree,
  const XML_Char* tagName)
{
  TXMLWorkflowTree* XMLTree = static_cast<TXMLWorkflowTree*>(workflowXMLTree);
  if (XMLTree == NULL)
  {
    std::stringstream info;
    info << "Expected not NULL workflow XML tree.";
    throw std::runtime_error(info.str());
  }

  if (XMLTree->currentTag->handlers.EndTagHandler != NULL)
  {
    XMLTree->currentTag->handlers.EndTagHandler(XMLTree);
  }
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

TXMLWorkflowTree::TXMLWorkflowTree() :
  rootTag(NULL), currentTag(NULL), tagName2TagHandlers()
{
  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(WorkflowTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Workflow, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ModulesTagName,
    TTagHandlersInfo(TXMLTagInfo::ETagType::Modules, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ModuleTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Module, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ModuleNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ModuleName, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ExecutionTypeTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ExecutionType, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(TransportTypeTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::TransportType, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ExecutablePathTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ExecutablePath,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(StartCommandLineArgsTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::StartCommandLineArgs,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ArgumentTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Argument, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(StopCommandLineTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::StopCommandLine,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ModuleParametersTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ModuleParameters,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ParameterTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Parameter, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(NameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Name, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ValueTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Value, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(EnvironmentVariablesTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::EnvironmentVariables,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(VariableTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::Variable, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(InputFileNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::InputFileName, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(OutputFileNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::OutputFileName,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(HasStateTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::HasState, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(StateFileNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::StateFileName, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(IsTransferableTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::IsTransferable,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(InputBatchesTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::InputBatches, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(InputBatchTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::InputBatch, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(InputBatchTypeTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::InputBatchType, 
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(DistributorNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::DistributorName,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(SourceChannelsTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::SourceChannels,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(InputBatchChannelsTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::InputBatchChannels,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ChannelNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ChannelName, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(OutputBatchesTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::OutputBatches, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(OutputBatchTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::OutputBatch, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(OutputBatchTypeTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::OutputBatchType,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(CollectorNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::CollectorName, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(OutputChannelsTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::OutputChannels,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(OutputChannelTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::OutputChannel, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ChannelConvertedNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ChannelConvertedName,
        TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(ReceiverNameTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::ReceiverName, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(IsStartingTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::IsStarting, TTagHandlers())));

  tagName2TagHandlers.insert(
    std::pair<std::string, TTagHandlersInfo>(IsFinishingTagName,
      TTagHandlersInfo(TXMLTagInfo::ETagType::IsFinishing, TTagHandlers())));

  for (TTagHandlersMap::iterator it = tagName2TagHandlers.begin();
    it != tagName2TagHandlers.end(); ++it)
  {
    it->second.second.StartTagHandler = DefaultStartTagHandler;
    it->second.second.EndTagHandler = DefaultEndTagHandler;
    it->second.second.TagDataHandler = NULL;
  }

  tagName2TagHandlers[ModuleNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ExecutionTypeTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[TransportTypeTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ExecutablePathTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ArgumentTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[StopCommandLineTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[NameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ValueTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[InputFileNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[OutputFileNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[HasStateTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[StateFileNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[IsTransferableTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[InputBatchTypeTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[DistributorNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ChannelNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[OutputBatchTypeTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[CollectorNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ChannelConvertedNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ReceiverNameTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[IsStartingTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[IsFinishingTagName].second.TagDataHandler =
    &DefaultTagDataHandler;
}

TXMLWorkflowTree::~TXMLWorkflowTree()
{
}

void TXMLWorkflowTree::DefaultStartTagHandler(
  TXMLWorkflowTree* workflowXMLTree, TXMLTagInfo* newTagInfo,
  const XML_Char** tagAttributes)
{
  newTagInfo->parentTag = workflowXMLTree->currentTag;
  if (workflowXMLTree->currentTag != NULL)
  {
    workflowXMLTree->currentTag->childTags.push_back(newTagInfo);
  }
  else // Else it is asssumed that is root tag with 'workflow' name
  {
    workflowXMLTree->rootTag = newTagInfo; 
  }
  workflowXMLTree->currentTag = newTagInfo; // go down
  if (tagAttributes != NULL)
  {
    for (int i = 0; tagAttributes[i]; ++i)
    {
      typedef std::pair<std::string, std::string> TTagAttribute;
      workflowXMLTree->currentTag->tagAttributes.push_back(
        TTagAttribute(std::string(tagAttributes[i]),
          std::string(tagAttributes[i + 1])));
    }
  }
}

void TXMLWorkflowTree::DefaultTagDataHandler(TXMLWorkflowTree* workflowXMLTree,
  const XML_Char* tagData, int tagDataLength)
{
  workflowXMLTree->currentTag->tagDataLength = tagDataLength;
  workflowXMLTree->currentTag->tagData;
}

void TXMLWorkflowTree::DefaultEndTagHandler(TXMLWorkflowTree* workflowXMLTree)
{
  if (workflowXMLTree->currentTag == NULL)
  {
    std::stringstream info;
    info << "NOTICE. Parsing workflow file is finished.";
    return;
  }

  workflowXMLTree->currentTag =
    workflowXMLTree->currentTag->parentTag; // go up
}

TTagHandlers::TTagHandlers(
  void(*StartTagHandler)(TXMLWorkflowTree*, TXMLTagInfo*, const XML_Char**),
  void(*TagDataHandler)(TXMLWorkflowTree*, const XML_Char*, int),
  void(*EndTagHandler)(TXMLWorkflowTree*)) :
  StartTagHandler(StartTagHandler), TagDataHandler(TagDataHandler),
  EndTagHandler(EndTagHandler)
{
}

TTagHandlers::~TTagHandlers()
{
}

TXMLTagInfo::TXMLTagInfo(ETagType::Type tagType, const TTagHandlers& handlers) :
  tagType(tagType), handlers(handlers), parentTag(NULL), childTags(),
  tagAttributes(), tagDataLength(0), tagData(NULL)
{
}

TXMLTagInfo::~TXMLTagInfo()
{
}

const std::string TXMLWorkflowTree::WorkflowTagName = "workflow";

const std::string TXMLWorkflowTree::ModulesTagName = "modules";

const std::string TXMLWorkflowTree::ModuleTagName = "module";

const std::string TXMLWorkflowTree::ModuleNameTagName = "module";

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
