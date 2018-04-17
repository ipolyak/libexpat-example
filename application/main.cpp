#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h> // atoi
#include <map>
#include <list>
#include "module_info.h"

#include "expat.h"

struct TXMLWorkflowTree;
struct TWrapperParser;
struct TXMLTagInfo;

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

  std::list<ETagType::Type> allowedParentTagIds;

  std::list<TXMLTagInfo*> childTags;

  std::list<ETagType::Type> allowedChildTagIds;

  std::map<std::string, std::string> tagAttributes;

  TTagHandlers handlers;

  int tagDataLength;

  XML_Char* tagData;

  TXMLTagInfo(ETagType::Type tagType, const TTagHandlers& handlers,
    const std::list<TXMLTagInfo::ETagType::Type>& allowedParentTagIds,
    const std::list<TXMLTagInfo::ETagType::Type>& allowedChildTagIds);

  ~TXMLTagInfo();
};

struct TXMLWorkflowTree
{
  TXMLTagInfo* rootTag;

  TXMLTagInfo* currentTag;

  typedef std::pair<TXMLTagInfo::ETagType::Type, TTagHandlers>
    TTagHandlersInfo;

  struct TPrioriTagInfo
  {
    TTagHandlersInfo handlers;
    std::list<TXMLTagInfo::ETagType::Type> allowedParentTagIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildTagIds;

    TPrioriTagInfo();
    TPrioriTagInfo(const TTagHandlersInfo& handlers,
      const std::list<TXMLTagInfo::ETagType::Type>& allowedParentTagIds,
      std::list<TXMLTagInfo::ETagType::Type>& allowedChildTagIds);
    ~TPrioriTagInfo();
  };
  
  typedef std::map<std::string, TPrioriTagInfo> TTagHandlersMap;
  TTagHandlersMap tagName2TagHandlers;

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

  static const std::string CountAttributeName;

  TXMLWorkflowTree();

  ~TXMLWorkflowTree();

  static TXMLTagInfo* FindTagAmongChilds(const TXMLTagInfo* relativeTag,
    TXMLTagInfo::ETagType::Type searchTagType);

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

  std::map<std::string, TModuleId::TWorkflowId> moduleName2WorkflowId;

  /* Функция, выполняющая запись содержимого workflow файла в буфер.
   * \param[in] pathToWorkflowFile Путь до workflow файла.
   * \param[in/out] Информация о считанное содержимое workflow файла в виде
   * буфера. Вызывающая сторона ответственна за освобождение памяти, выделенной
   * под буфер.
   */
  void ReadWorkflowFile(const std::string& pathToWorkflowFile,
    TWorkflowFileContent& workflowFileContent);

  static const std::string BoolTrueStr;

  static const std::string BoolFalseStr;

  void Fill(const TXMLTagInfo* relativeTag,
    bool& value);

  void Fill(const TXMLTagInfo* relativeTag,
    std::string& value);

  template <class T1, class T2>
  void Fill(const TXMLTagInfo* relativeTag,
    std::pair<T1, T2>& value);

  template <class T>
  void Fill(const TXMLTagInfo* relativeTag,
    std::vector<T>& value);

  template <class T1, class T2>
  void Fill(const TXMLTagInfo* relativeTag,
    std::map<T1, T2>& value);

  static const std::string InternalExecutionTypeStr;

  static const std::string ExternalExecutionTypeStr;

  void Fill(const TXMLTagInfo* relativeTag, 
    EExecutionType::Type& value);

  static const std::string PipeTransportTypeStr;

  static const std::string FileTransportTypeStr;

  void Fill(const TXMLTagInfo* relativeTag, 
    ETransportType::Type& value);

  static const std::string RegularBatchTypeStr;

  static const std::string CollectorInputBatchTypeStr;

  void Fill(const TXMLTagInfo* relativeTag, EInputBatchType::Type& value);

  void Fill(const TXMLTagInfo* relativeTag, TInputBatchInfo& value);

  static const std::string DistributorOutputBatchTypeStr;

  void Fill(const TXMLTagInfo* relativeTag, EOutputBatchType::Type& value);

  void Fill(const TXMLTagInfo* relativeTag,
    TOutputBatchInfo::TOutputMessageChannelInfo& value);

  void Fill(const TXMLTagInfo* relativeTag, TOutputBatchInfo& value);

  void Fill(const TXMLTagInfo* relativeTag, TModuleInfo& value);

  void FillModules(const TXMLWorkflowTree* XMLTree,
    std::vector<TModuleInfo>& modules);

  void CheckCorrectnessModuleInfos(const std::vector<TModuleInfo>& modules);

  void CheckFindResult(const TXMLTagInfo* checkingTag,
    TXMLTagInfo::ETagType::Type searchTagType,
    TXMLTagInfo::ETagType::Type parentTagType);

  void CheckAttributeFindResult(
    const std::map<std::string, std::string>::const_iterator& it,
    const std::map<std::string, std::string>::const_iterator& itEnd,
    const std::string& attributeName,
    TXMLTagInfo::ETagType::Type scopeTagType);

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

  std::vector<TModuleInfo> modules;
  FillModules(workflowXMLTree, modules);

  CheckCorrectnessModuleInfos(modules);

  delete workflowXMLTree;
  return modules;
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

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  bool& value)
{
  if ((relativeTag->tagData != NULL) && (relativeTag->tagDataLength > 0))
  {
    std::string strBool;
    strBool.resize(relativeTag->tagDataLength);
    strBool.assign(relativeTag->tagData, relativeTag->tagDataLength);
    if (strBool == BoolTrueStr)
    {
      value = true;
    }
    else if (strBool == BoolFalseStr)
    {
      value = false;
    }
    else
    {
      std::stringstream info;
      info << "Unexpected value of bool variable. Should be '" <<
        BoolTrueStr << "' or '" << BoolFalseStr << "' for tag with '" <<
        relativeTag->tagType << "' type.";
      throw std::runtime_error(info.str());
    }
  }
  else
  {
    std::stringstream info;
    info << "Bool value should be set for tag with '" <<
      relativeTag->tagType << "' type.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  std::string& value)
{
  value.clear();
  if ((relativeTag->tagData != NULL) && (relativeTag->tagDataLength > 0))
  {
    value.resize(relativeTag->tagDataLength);
    value.assign(relativeTag->tagData, relativeTag->tagDataLength);
  }
}

template <class T1, class T2>
void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  std::pair<T1, T2>& value)
{
  TXMLTagInfo::ETagType::Type relativeTagType = relativeTag->tagType;
  if ((relativeTagType == TXMLTagInfo::ETagType::Parameter) ||
    (relativeTagType == TXMLTagInfo::ETagType::Variable))
  {
    /* Filling object name */
    TXMLTagInfo* childTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::Name);
    CheckFindResult(childTag, TXMLTagInfo::ETagType::Name,
      relativeTag->tagType);
    Fill(childTag, value.first);

    /* Filling object value */
    childTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::Value);
    CheckFindResult(childTag, TXMLTagInfo::ETagType::Value,
      relativeTag->tagType);
    Fill(childTag, value.second);
  }
  else
  {
    std::stringstream info;
    info << "Unsupported handling of pair for tag with '" <<
      relativeTagType << "' type.";
    throw std::runtime_error(info.str());
  }
}

template <class T>
void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  std::vector<T>& value)
{
  /* Getting count of elements in vector from tag attributes */
  std::map<std::string, std::string>::const_iterator itAttrs =
    relativeTag->tagAttributes.find(TXMLWorkflowTree::CountAttributeName);
  CheckAttributeFindResult(itAttrs, relativeTag->tagAttributes.end(),
    TXMLWorkflowTree::CountAttributeName, relativeTag->tagType);
  int count = atoi(itAttrs->second.c_str());
  if (count < 0)
  {
    std::stringstream info;
    info << "Count vector elements must be non-negative. " <<
      "Current value of count: " << count;
    throw std::runtime_error(info.str());
  }
  value.resize(count);

  /* Filling elements of vector */
  std::list<TXMLTagInfo*>::const_iterator it = relativeTag->childTags.begin();
  std::list<TXMLTagInfo*>::const_iterator itEnd = relativeTag->childTags.end();
  std::vector<T>::iterator itVector = value.begin();
  int i = 0;
  while (it != itEnd)
  {
    if (itVector == value.end())
    {
      std::stringstream info;
      info << "The actual number of elements in vector is greater than " <<
        "that specified in the tag attribute. Expected '" << count << "' " <<
        "elements in vector (in tag with '" << relativeTag->tagType <<
        "' type)";
      throw std::runtime_error(info.str());
    }
    TXMLTagInfo* childTag = *it;
    Fill(childTag, value[i]);
    ++i;
    ++it;
    ++itVector;
  }
  if (i != count)
  {
    std::stringstream info;
    info << "Unexpected count of vector elements for tag with '" <<
      relativeTag->tagType << "' type." << " Current count: " << i <<
      ". Should be " << count << ".";
    throw std::runtime_error(info.str());
  }
}

template <class T1, class T2>
void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  std::map<T1, T2>& value)
{
  /* Getting count of elements in map from tag attributes */
  std::map<std::string, std::string>::const_iterator itAttrs =
    relativeTag->tagAttributes.find(TXMLWorkflowTree::CountAttributeName);
  CheckAttributeFindResult(itAttrs, relativeTag->tagAttributes.end(),
    TXMLWorkflowTree::CountAttributeName, relativeTag->tagType);
  int count = atoi(itAttrs->second.c_str());
  if (count < 0)
  {
    std::stringstream info;
    info << "Count map elements must be non-negative. " <<
      "Current value of count: " << count;
    throw std::runtime_error(info.str());
  }

  /* Filling elements of map */
  std::list<TXMLTagInfo*>::const_iterator it = relativeTag->childTags.begin();
  std::list<TXMLTagInfo*>::const_iterator itEnd = relativeTag->childTags.end();
  int i = 0;
  while (it != itEnd)
  {
    TXMLTagInfo* childTag = *it;
    std::pair<T1, T2> mapElem;
    Fill(childTag, mapElem);
    value.insert(mapElem);
    ++i;
    ++it;
  }
  if (i != count)
  {
    std::stringstream info;
    info << "Unexpected count of map elements for tag with '" <<
      relativeTag->tagType << "' type." << " Current count: " << i <<
      ". Should be " << count << ".";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  EExecutionType::Type& value)
{
  if ((relativeTag->tagData != NULL) && (relativeTag->tagDataLength > 0))
  {
    std::string execTypeStr;
    execTypeStr.resize(relativeTag->tagDataLength);
    execTypeStr.assign(relativeTag->tagData, relativeTag->tagDataLength);
    if (execTypeStr == ExternalExecutionTypeStr)
    {
      value = EExecutionType::External;
    }
    else if (execTypeStr == InternalExecutionTypeStr)
    {
      value = EExecutionType::Internal;
    }
    else
    {
      std::stringstream info;
      info << "Unexpected value of execution type variable. Should be '" <<
        ExternalExecutionTypeStr << "' or '" << InternalExecutionTypeStr <<
        "' for tag with '" << relativeTag->tagType << "' type.";
      throw std::runtime_error(info.str());
    }
  }
  else
  {
    std::stringstream info;
    info << "Execution type value should be set for tag with '" <<
      relativeTag->tagType << "' type.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  ETransportType::Type& value)
{
  if ((relativeTag->tagData != NULL) && (relativeTag->tagDataLength > 0))
  {
    std::string transportTypeStr;
    transportTypeStr.resize(relativeTag->tagDataLength);
    transportTypeStr.assign(relativeTag->tagData, relativeTag->tagDataLength);
    if (transportTypeStr == FileTransportTypeStr)
    {
      value = ETransportType::File;
    }
    else if (transportTypeStr == PipeTransportTypeStr)
    {
      value = ETransportType::Pipe;
    }
    else
    {
      std::stringstream info;
      info << "Unexpected value of transport type variable. Should be '" <<
        FileTransportTypeStr << "' or '" << PipeTransportTypeStr <<
        "' for tag with '" << relativeTag->tagType << "' type.";
      throw std::runtime_error(info.str());
    }
  }
  else
  {
    std::stringstream info;
    info << "Transport type value should be set for tag with '" <<
      relativeTag->tagType << "' type.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  EInputBatchType::Type& value)
{
  if ((relativeTag->tagData != NULL) && (relativeTag->tagDataLength > 0))
  {
    std::string inputBatchTypeStr;
    inputBatchTypeStr.resize(relativeTag->tagDataLength);
    inputBatchTypeStr.assign(relativeTag->tagData, relativeTag->tagDataLength);
    if (inputBatchTypeStr == RegularBatchTypeStr)
    {
      value = EInputBatchType::Regular;
    }
    else if (inputBatchTypeStr == CollectorInputBatchTypeStr)
    {
      value = EInputBatchType::Collector;
    }
    else
    {
      std::stringstream info;
      info << "Unexpected value of input batch type variable. Should be '" <<
        RegularBatchTypeStr << "' or '" << CollectorInputBatchTypeStr <<
        "' for tag with '" << relativeTag->tagType << "' type.";
      throw std::runtime_error(info.str());
    }
  }
  else
  {
    std::stringstream info;
    info << "Input batch type value should be set for tag with '" <<
      relativeTag->tagType << "' type.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  TInputBatchInfo& value)
{
  /* Filling of source channels */
  TXMLTagInfo* inputBatchChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::SourceChannels);
  CheckFindResult(inputBatchChildTag, TXMLTagInfo::ETagType::SourceChannels,
    relativeTag->tagType);
  Fill(inputBatchChildTag, value.sourceChannels);
  for (std::size_t i = 0; i < value.sourceChannels.size(); ++i)
  {
    if (value.sourceChannels[i].empty())
    {
      std::stringstream info;
      info << "Empty source channel name in input batch is forbidden.";
      throw std::runtime_error(info.str());
    }
  }

  /* Filling of distributor workflow id */
  inputBatchChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::DistributorName);
  CheckFindResult(inputBatchChildTag, TXMLTagInfo::ETagType::DistributorName,
    relativeTag->tagType);
  std::string distributorName;
  Fill(inputBatchChildTag, distributorName);
  if (!distributorName.empty())
  {
    std::map<std::string, TModuleId::TWorkflowId>::iterator it =
      moduleName2WorkflowId.find(distributorName);
    if (it == moduleName2WorkflowId.end())
    {
      std::stringstream info;
      info << "Distributor with '" << distributorName << "' name unexisted " <<
        "in granted workflow.";
      throw std::runtime_error(info.str());
    }
    value.source = it->second;
  }

  /* Filling of input batch channels */
  inputBatchChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::InputBatchChannels);
  CheckFindResult(inputBatchChildTag,
    TXMLTagInfo::ETagType::InputBatchChannels, relativeTag->tagType);
  Fill(inputBatchChildTag, value.channels);
  for (std::size_t i = 0; i < value.channels.size(); ++i)
  {
    if (value.channels[i].empty())
    {
      std::stringstream info;
      info << "Empty input batch channel name in input batch is forbidden.";
      throw std::runtime_error(info.str());
    }
  }

  /* Filling of input batch type */
  inputBatchChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::InputBatchType);
  CheckFindResult(inputBatchChildTag, TXMLTagInfo::ETagType::InputBatchType,
    relativeTag->tagType);
  Fill(inputBatchChildTag, value.type);
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  EOutputBatchType::Type& value)
{
  if ((relativeTag->tagData != NULL) && (relativeTag->tagDataLength > 0))
  {
    std::string outputBatchTypeStr;
    outputBatchTypeStr.resize(relativeTag->tagDataLength);
    outputBatchTypeStr.assign(relativeTag->tagData, relativeTag->tagDataLength);
    if (outputBatchTypeStr == RegularBatchTypeStr)
    {
      value = EOutputBatchType::Regular;
    }
    else if (outputBatchTypeStr == DistributorOutputBatchTypeStr)
    {
      value = EOutputBatchType::Distributor;
    }
    else
    {
      std::stringstream info;
      info << "Unexpected value of output batch type variable. Should be '" <<
        RegularBatchTypeStr << "' or '" << DistributorOutputBatchTypeStr <<
        "' for tag with '" << relativeTag->tagType << "' type.";
      throw std::runtime_error(info.str());
    }
  }
  else
  {
    std::stringstream info;
    info << "Output batch type value should be set for tag with '" <<
      relativeTag->tagType << "' type.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  TOutputBatchInfo::TOutputMessageChannelInfo& value)
{
  /* Filling of receiver workflow id */
  TXMLTagInfo* outputMessageChannelInfoChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::ReceiverName);
  CheckFindResult(outputMessageChannelInfoChildTag,
    TXMLTagInfo::ETagType::ReceiverName, relativeTag->tagType);
  std::string receiverName;
  Fill(outputMessageChannelInfoChildTag, receiverName);
  if (!receiverName.empty())
  {
    std::map<std::string, TModuleId::TWorkflowId>::iterator it =
      moduleName2WorkflowId.find(receiverName);
    if (it == moduleName2WorkflowId.end())
    {
      std::stringstream info;
      info << "Receiver module with '" << receiverName <<
        "' name unexisted in granted workflow.";
      throw std::runtime_error(info.str());
    }
    value.receiver = it->second;
  }
  else
  {
    std::stringstream info;
    info << "Empty receiver name in output batch is forbidden.";
    throw std::runtime_error(info.str());
  }

  /* Filling of channel name */
  outputMessageChannelInfoChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::ChannelName);
  CheckFindResult(outputMessageChannelInfoChildTag,
    TXMLTagInfo::ETagType::ChannelName, relativeTag->tagType);
  Fill(outputMessageChannelInfoChildTag, value.name);
  if (value.name.empty())
  {
    std::stringstream info;
    info << "Empty channel name in output batch is forbidden.";
    throw std::runtime_error(info.str());
  }

  /* Filling of converted channel name */
  outputMessageChannelInfoChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::ChannelConvertedName);
  CheckFindResult(outputMessageChannelInfoChildTag,
    TXMLTagInfo::ETagType::ChannelConvertedName, relativeTag->tagType);
  Fill(outputMessageChannelInfoChildTag, value.convertedName);
  if (value.convertedName.empty())
  {
    std::stringstream info;
    info << "Empty converted channel name in output batch is forbidden.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  TOutputBatchInfo& value)
{
  /* Filling of collector workflow id */
  TXMLTagInfo* outputBatchChildTag =
    TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
      TXMLTagInfo::ETagType::CollectorName);
  CheckFindResult(outputBatchChildTag, TXMLTagInfo::ETagType::CollectorName,
    relativeTag->tagType);
  std::string collectorName;
  Fill(outputBatchChildTag, collectorName);
  if (!collectorName.empty())
  {
    std::map<std::string, TModuleId::TWorkflowId>::iterator it =
      moduleName2WorkflowId.find(collectorName);
    if (it == moduleName2WorkflowId.end())
    {
      std::stringstream info;
      info << "Collector with '" << collectorName << "' name unexisted " <<
        "in granted workflow.";
      throw std::runtime_error(info.str());
    }
    value.receiver = it->second;
  }

  /* Filling of information about output channels */
  outputBatchChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::OutputChannels);
  CheckFindResult(outputBatchChildTag, TXMLTagInfo::ETagType::OutputChannels,
    relativeTag->tagType);
  Fill(outputBatchChildTag, value.channels);

  /* Filling of output batch type */
  outputBatchChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::OutputBatchType);
  CheckFindResult(outputBatchChildTag, TXMLTagInfo::ETagType::OutputBatchType,
    relativeTag->tagType);
  Fill(outputBatchChildTag, value.type);
}

void TWrapperXMLParser::Fill(const TXMLTagInfo* relativeTag,
  TModuleInfo& value)
{
  /* Filling of module name */
  TXMLTagInfo* moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::Name);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::Name,
    relativeTag->tagType);
  Fill(moduleChildTag, value.name);
  if (value.name.empty())
  {
    std::stringstream info;
    info << "Empty module name is forbidden.";
    throw std::runtime_error(info.str());
  }

  /* Filling of execution type */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::ExecutionType);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::ExecutionType,
    relativeTag->tagType);
  Fill(moduleChildTag, value.executionType);

  /* Filling of transport type */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::TransportType);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::TransportType,
    relativeTag->tagType);
  Fill(moduleChildTag, value.transportType);

  /* Filling of executable path */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::ExecutablePath);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::ExecutablePath,
    relativeTag->tagType);
  Fill(moduleChildTag, value.executablePath);
  if (value.executablePath.empty())
  {
    std::stringstream info;
    info << "Empty executable path is forbidden.";
    throw std::runtime_error(info.str());
  }

  /* Filling of start command line args */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::StartCommandLineArgs);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::StartCommandLineArgs,
    relativeTag->tagType);
  Fill(moduleChildTag, value.startCommandLineArgs);

  /* Filling of stop command line */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::StopCommandLine);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::StopCommandLine,
    relativeTag->tagType);
  Fill(moduleChildTag, value.stopCommandLine);

  /* Filling of module parameters */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::ModuleParameters);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::ModuleParameters,
    relativeTag->tagType);
  Fill(moduleChildTag, value.parameters);

  /* Filling of environment variables */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::EnvironmentVariables);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::EnvironmentVariables,
    relativeTag->tagType);
  Fill(moduleChildTag, value.environmentVariables);

  /* Filling of input file name */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::InputFileName);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::InputFileName,
    relativeTag->tagType);
  Fill(moduleChildTag, value.inputFileName);
  if (value.inputFileName.empty())
  {
    //value.inputFileName = TWrapperConfiguration::InputFileDefaultPrefix;
  }

  /* Filling of output file name */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::OutputFileName);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::OutputFileName,
    relativeTag->tagType);
  Fill(moduleChildTag, value.outputFileName);
  if (value.outputFileName.empty())
  {
    //value.outputFileName = TWrapperConfiguration::OutputFileDefaultPrefix;
  }

  /* Filling of 'has state' bool variable */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::HasState);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::HasState,
    relativeTag->tagType);
  Fill(moduleChildTag, value.hasState);

  /* Filling of output file name */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::StateFileName);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::StateFileName,
    relativeTag->tagType);
  Fill(moduleChildTag, value.stateFileName);
  if (value.stateFileName.empty())
  {
    //value.stateFileName = TWrapperConfiguration::StateFileDefaultName;
  }

  /* Filling of 'is transferable' bool variable */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::IsTransferable);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::IsTransferable,
    relativeTag->tagType);
  Fill(moduleChildTag, value.isTransferable);

  /* Filling of input batches */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::InputBatches);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::InputBatches,
    relativeTag->tagType);
  Fill(moduleChildTag, value.inputBatches);

  /* Filling of output batches */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::OutputBatches);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::OutputBatches,
    relativeTag->tagType);
  Fill(moduleChildTag, value.outputBatches);

  /* Filling of 'is starting' bool variable */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::IsStarting);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::IsStarting,
    relativeTag->tagType);
  Fill(moduleChildTag, value.isStarting);

  /* Filling of 'is finishing' bool variable */
  moduleChildTag = TXMLWorkflowTree::FindTagAmongChilds(relativeTag,
    TXMLTagInfo::ETagType::IsFinishing);
  CheckFindResult(moduleChildTag, TXMLTagInfo::ETagType::IsFinishing,
    relativeTag->tagType);
  Fill(moduleChildTag, value.isFinishing);
}

void TWrapperXMLParser::FillModules(const TXMLWorkflowTree* XMLTree,
  std::vector<TModuleInfo>& modules)
{
  /* Getting modules count */
  TXMLTagInfo* currentTag = XMLTree->rootTag;
  TXMLTagInfo* modulesTag = TXMLWorkflowTree::FindTagAmongChilds(currentTag,
    TXMLTagInfo::ETagType::Modules);
  CheckFindResult(modulesTag, TXMLTagInfo::ETagType::Modules, currentTag->tagType);
  std::map<std::string, std::string>::iterator itAttrs =
    modulesTag->tagAttributes.find(TXMLWorkflowTree::CountAttributeName);
  CheckAttributeFindResult(itAttrs, modulesTag->tagAttributes.end(),
    TXMLWorkflowTree::CountAttributeName, modulesTag->tagType);
  int count = atoi(itAttrs->second.c_str());
  if (count <= 0)
  {
    std::stringstream info;
    info << "Incorrect value of count modules. Expected one less module. " <<
      "Current value of count: " << count;
    throw std::runtime_error(info.str());
  }
  modules.resize(count);

  /* Filling name for each module */
  std::list<TXMLTagInfo*>::iterator itModulesTagChilds =
    modulesTag->childTags.begin();
  std::list<TXMLTagInfo*>::iterator itModulesTagChildsEnd =
    modulesTag->childTags.end();
  int i = 0;
  while (itModulesTagChilds != itModulesTagChildsEnd)
  {
    TXMLTagInfo* moduleTag = *itModulesTagChilds;
    TXMLTagInfo* moduleNameTag = TXMLWorkflowTree::FindTagAmongChilds(
      moduleTag, TXMLTagInfo::ETagType::Name);
    CheckFindResult(moduleNameTag, TXMLTagInfo::ETagType::Name, moduleTag->tagType);
    Fill(moduleNameTag, modules[i].name);
    if (modules[i].name.empty())
    {
      std::stringstream info;
      info << "Empty module name is forbidden.";
      throw std::runtime_error(info.str());
    }
    ++i;
    ++itModulesTagChilds;
  }
  if (i != count)
  {
    std::stringstream info;
    info << "Unexpected count of modules. Current: " << i << ". Should be " <<
      count << " modules";
    throw std::runtime_error(info.str());
  }

  /* Creating map for converting module name to module workflow id */
  moduleName2WorkflowId.clear();
  for (TModuleId::TWorkflowId i = 0; i < count; ++i)
  {
    moduleName2WorkflowId.insert(std::pair<std::string,
      TModuleId::TWorkflowId>(modules[i].name, i + 1));
  }

  /* Filling module id for each module */
  for (std::size_t i = 0; i < modules.size(); ++i)
  {
    modules[i].id = TModuleId(moduleName2WorkflowId[modules[i].name]);
  }

  /* Filling full information about each module */
  itModulesTagChilds = modulesTag->childTags.begin();
  itModulesTagChildsEnd = modulesTag->childTags.end();
  i = 0;
  while (itModulesTagChilds != itModulesTagChildsEnd)
  {
    TXMLTagInfo* moduleTag = *itModulesTagChilds;
    Fill(moduleTag, modules[i]);
    ++i;
    ++itModulesTagChilds;
  }
}

void TWrapperXMLParser::CheckCorrectnessModuleInfos(
  const std::vector<TModuleInfo>& modules)
{
  // TODO: implement
}

void TWrapperXMLParser::CheckFindResult(const TXMLTagInfo* checkingTag,
  TXMLTagInfo::ETagType::Type searchTagType,
  TXMLTagInfo::ETagType::Type parentTagType)
{
  if (checkingTag == NULL)
  {
    std::stringstream info;
    info << "XML tag with '" << searchTagType << "' type " <<
      "didn't found among childs of tag with '" << parentTagType <<
      "' type.";
    throw std::runtime_error(info.str());
  }
}

void TWrapperXMLParser::CheckAttributeFindResult(
  const std::map<std::string, std::string>::const_iterator& it,
  const std::map<std::string, std::string>::const_iterator& itEnd,
  const std::string& attributeName,
  TXMLTagInfo::ETagType::Type scopeTagType)
{
  if (it == itEnd)
  {
    std::stringstream info;
    info << "Expected '" << attributeName <<
      "' attribute in tag with '" << scopeTagType << "' type.";
    throw std::runtime_error(info.str());
  }
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
  TXMLWorkflowTree::TTagHandlersMap::iterator prioriTagInfoIt =
    XMLTree->tagName2TagHandlers.find(tagNameStr);
  if (prioriTagInfoIt == XMLTree->tagName2TagHandlers.end())
  {
    std::stringstream info;
    info << "Unexpected tag with '" << tagNameStr << "' name.";
    throw std::runtime_error(info.str());
  }
  TXMLTagInfo* XMLTagInfo = new TXMLTagInfo(
    prioriTagInfoIt->second.handlers.first,
    prioriTagInfoIt->second.handlers.second,
    prioriTagInfoIt->second.allowedParentTagIds,
    prioriTagInfoIt->second.allowedChildTagIds);
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

TWrapperParser* TWrapperParserFactory::CreateInstance(
  const std::string& parserType)
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
  typedef TXMLTagInfo::ETagType XMLTagType;

  /* Filling prior info about XML tags */
  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedChildIds.push_back(XMLTagType::Modules);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Workflow,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(WorkflowTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Workflow);
    allowedChildIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Modules,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ModulesTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Modules);
    allowedChildIds.push_back(XMLTagType::Name);
    allowedChildIds.push_back(XMLTagType::ExecutionType);
    allowedChildIds.push_back(XMLTagType::TransportType);
    allowedChildIds.push_back(XMLTagType::ExecutablePath);
    allowedChildIds.push_back(XMLTagType::StartCommandLineArgs);
    allowedChildIds.push_back(XMLTagType::StopCommandLine);
    allowedChildIds.push_back(XMLTagType::ModuleParameters);
    allowedChildIds.push_back(XMLTagType::EnvironmentVariables);
    allowedChildIds.push_back(XMLTagType::InputFileName);
    allowedChildIds.push_back(XMLTagType::OutputFileName);
    allowedChildIds.push_back(XMLTagType::HasState);
    allowedChildIds.push_back(XMLTagType::StateFileName);
    allowedChildIds.push_back(XMLTagType::IsTransferable);
    allowedChildIds.push_back(XMLTagType::InputBatches);
    allowedChildIds.push_back(XMLTagType::OutputBatches);
    allowedChildIds.push_back(XMLTagType::IsStarting);
    allowedChildIds.push_back(XMLTagType::IsFinishing);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Module,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ModuleTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Workflow);
    allowedChildIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Modules,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ModulesTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    allowedParentIds.push_back(XMLTagType::Parameter);
    allowedParentIds.push_back(XMLTagType::Variable);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Name,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(NameTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::ExecutionType,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ExecutionTypeTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::TransportType,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(TransportTypeTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::ExecutablePath,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ExecutablePathTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    allowedChildIds.push_back(XMLTagType::Argument);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(
      XMLTagType::StartCommandLineArgs,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(StartCommandLineArgsTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::StopCommandLine,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(StopCommandLineTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    allowedChildIds.push_back(XMLTagType::Parameter);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::ModuleParameters,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ModuleParametersTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::ModuleParameters);
    allowedChildIds.push_back(XMLTagType::Name);
    allowedChildIds.push_back(XMLTagType::Value);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Parameter,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ParameterTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Parameter);
    allowedParentIds.push_back(XMLTagType::Variable);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Value,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ValueTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    allowedChildIds.push_back(XMLTagType::Variable);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(
      XMLTagType::EnvironmentVariables,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(EnvironmentVariablesTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::EnvironmentVariables);
    allowedChildIds.push_back(XMLTagType::Name);
    allowedChildIds.push_back(XMLTagType::Value);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::Variable,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(VariableTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::InputFileName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(InputFileNameTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::OutputFileName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(OutputFileNameTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::HasState,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(HasStateTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::StateFileName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(StateFileNameTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::IsTransferable,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(IsTransferableTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    allowedChildIds.push_back(XMLTagType::InputBatch);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::InputBatches,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(InputBatchesTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::InputBatches);
    allowedChildIds.push_back(XMLTagType::InputBatchType);
    allowedChildIds.push_back(XMLTagType::DistributorName);
    allowedChildIds.push_back(XMLTagType::SourceChannels);
    allowedChildIds.push_back(XMLTagType::InputBatchChannels);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::InputBatch,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(InputBatchTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::InputBatch);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::InputBatchType,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(InputBatchTypeTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::InputBatch);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::DistributorName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(DistributorNameTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::InputBatch);
    allowedChildIds.push_back(XMLTagType::ChannelName);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::SourceChannels,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(SourceChannelsTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::SourceChannels);
    allowedParentIds.push_back(XMLTagType::InputBatchChannels);
    allowedParentIds.push_back(XMLTagType::OutputChannel);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::ChannelName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ChannelNameTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::InputBatch);
    allowedChildIds.push_back(XMLTagType::ChannelName);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::InputBatchChannels,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(InputBatchChannelsTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    allowedChildIds.push_back(XMLTagType::OutputBatch);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::OutputBatches,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(OutputBatchesTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputBatches);
    allowedChildIds.push_back(XMLTagType::OutputBatchType);
    allowedChildIds.push_back(XMLTagType::CollectorName);
    allowedChildIds.push_back(XMLTagType::OutputChannels);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::OutputBatch,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(OutputBatchTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputBatch);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::OutputBatchType,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(OutputBatchTypeTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputBatch);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::CollectorName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(CollectorNameTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputBatch);
    allowedChildIds.push_back(XMLTagType::OutputChannel);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::OutputChannels,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(OutputChannelsTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputChannels);
    allowedChildIds.push_back(XMLTagType::ChannelName);
    allowedChildIds.push_back(XMLTagType::ChannelConvertedName);
    allowedChildIds.push_back(XMLTagType::ReceiverName);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::OutputChannel,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(OutputChannelTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputChannel);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::ChannelConvertedName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ChannelConvertedNameTagName,
        prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::OutputChannel);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::ReceiverName,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(ReceiverNameTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::IsStarting,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(IsStartingTagName, prioriInfo));
  }

  {
    std::list<TXMLTagInfo::ETagType::Type> allowedParentIds;
    std::list<TXMLTagInfo::ETagType::Type> allowedChildIds;
    allowedParentIds.push_back(XMLTagType::Module);
    TPrioriTagInfo prioriInfo(TTagHandlersInfo(XMLTagType::IsFinishing,
      TTagHandlers()), allowedParentIds, allowedChildIds);
    tagName2TagHandlers.insert(
      std::pair<std::string, TPrioriTagInfo>(IsFinishingTagName, prioriInfo));
  }

  for (TTagHandlersMap::iterator it = tagName2TagHandlers.begin();
    it != tagName2TagHandlers.end(); ++it)
  {
    it->second.handlers.second.StartTagHandler = &DefaultStartTagHandler;
    it->second.handlers.second.EndTagHandler = &DefaultEndTagHandler;
    it->second.handlers.second.TagDataHandler = NULL;
  }

  tagName2TagHandlers[ExecutionTypeTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[TransportTypeTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ExecutablePathTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ArgumentTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[StopCommandLineTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[NameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ValueTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[InputFileNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[OutputFileNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[HasStateTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[StateFileNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[IsTransferableTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[InputBatchTypeTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[DistributorNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ChannelNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[OutputBatchTypeTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[CollectorNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ChannelConvertedNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[ReceiverNameTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[IsStartingTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
  tagName2TagHandlers[IsFinishingTagName].handlers.second.TagDataHandler =
    &DefaultTagDataHandler;
}

TXMLWorkflowTree::~TXMLWorkflowTree()
{
  currentTag = rootTag;
  while (currentTag != NULL)
  {
    /* Go down to leaf tag */
    while (!(currentTag->childTags.empty()))
    {
      currentTag = currentTag->childTags.front();
    }

    /* Free tag data */
    if (currentTag->tagData != NULL)
    {
      delete[] currentTag->tagData;
    }

    TXMLTagInfo* parentTag = currentTag->parentTag;

    /* Delete leaf tag */
    delete currentTag;
    if (parentTag != NULL)
    {
      parentTag->childTags.pop_front();
    }

    /* Go up to parent tag */
    currentTag = parentTag;
  }
}

TXMLTagInfo* TXMLWorkflowTree::FindTagAmongChilds(const TXMLTagInfo* relativeTag,
  TXMLTagInfo::ETagType::Type searchTagType)
{
  if (relativeTag == NULL)
  {
    std::stringstream info;
    info << "Expected not NULL XML tag.";
    throw std::runtime_error(info.str());
  }

  for (std::list<TXMLTagInfo*>::const_iterator it = relativeTag->childTags.begin();
    it != relativeTag->childTags.end(); ++it)
  {
    TXMLTagInfo* childTag = *it;
    if (childTag == NULL)
    {
      std::stringstream info;
      info << "Expected not NULL XML tag.";
      throw std::runtime_error(info.str());
    }
    if (childTag->tagType == searchTagType)
    {
      return childTag;
    }
  }

  return NULL;
}

void TXMLWorkflowTree::DefaultStartTagHandler(
  TXMLWorkflowTree* workflowXMLTree, TXMLTagInfo* newTagInfo,
  const XML_Char** tagAttributes)
{
  if (newTagInfo == NULL)
  {
    std::stringstream info;
    info << "Expected not NULL XML tag.";
    throw std::runtime_error(info.str());
  }

  /* Checking relationships beetwen new and current tags */
  if(workflowXMLTree->currentTag != NULL)
  {
    std::list<TXMLTagInfo::ETagType::Type>::iterator it =
      std::find(workflowXMLTree->currentTag->allowedChildTagIds.begin(),
        workflowXMLTree->currentTag->allowedChildTagIds.end(),
        newTagInfo->tagType);
    if (it == workflowXMLTree->currentTag->allowedChildTagIds.end())
    {
      std::stringstream info;
      info << "New XML tag with '" << newTagInfo->tagType << "' type can " <<
        "not be child of tag with '" << workflowXMLTree->currentTag->tagType <<
        "' type.";
      throw std::runtime_error(info.str());
    }
  }
  if (workflowXMLTree->currentTag != NULL)
  {
    std::list<TXMLTagInfo::ETagType::Type>::iterator it =
      std::find(newTagInfo->allowedParentTagIds.begin(),
        newTagInfo->allowedParentTagIds.end(),
        workflowXMLTree->currentTag->tagType);
    if (it == newTagInfo->allowedParentTagIds.end())
    {
      std::stringstream info;
      info << "New XML tag with '" << newTagInfo->tagType << "' type can " <<
        "not be have parent tag with '" <<
        workflowXMLTree->currentTag->tagType << "' type.";
      throw std::runtime_error(info.str());
    }
  }

  /* Start handling */
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
    for (int i = 0; tagAttributes[i]; i+=2)
    {
      typedef std::pair<std::string, std::string> TTagAttribute;
      workflowXMLTree->currentTag->tagAttributes[std::string(tagAttributes[i])] =
        std::string(tagAttributes[i + 1]);
    }
  }
}

void TXMLWorkflowTree::DefaultTagDataHandler(TXMLWorkflowTree* workflowXMLTree,
  const XML_Char* tagData, int tagDataLength)
{
  workflowXMLTree->currentTag->tagDataLength = tagDataLength;
  if (tagDataLength != 0)
  {
    workflowXMLTree->currentTag->tagData = new XML_Char[tagDataLength];
    memcpy(static_cast<void*>(workflowXMLTree->currentTag->tagData),
      static_cast<void*>(const_cast<XML_Char*>(tagData)), tagDataLength);
  }
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

TXMLTagInfo::TXMLTagInfo(ETagType::Type tagType, const TTagHandlers& handlers,
  const std::list<TXMLTagInfo::ETagType::Type>& allowedParentTagIds,
  const std::list<TXMLTagInfo::ETagType::Type>& allowedChildTagIds) :
  tagType(tagType), handlers(handlers), parentTag(NULL),
  allowedParentTagIds(allowedParentTagIds),
  allowedChildTagIds(allowedChildTagIds), childTags(), tagAttributes(),
  tagDataLength(0), tagData(NULL)
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

const std::string TXMLWorkflowTree::CountAttributeName = "count";


const std::string TWrapperXMLParser::InternalExecutionTypeStr = "Internal";

const std::string TWrapperXMLParser::ExternalExecutionTypeStr = "External";

const std::string TWrapperXMLParser::PipeTransportTypeStr = "Pipe";

const std::string TWrapperXMLParser::FileTransportTypeStr = "File";

const std::string TWrapperXMLParser::BoolTrueStr = "yes";

const std::string TWrapperXMLParser::BoolFalseStr = "no";

const std::string TWrapperXMLParser::RegularBatchTypeStr = "Regular";

const std::string TWrapperXMLParser::CollectorInputBatchTypeStr = "Collector";

const std::string TWrapperXMLParser::DistributorOutputBatchTypeStr = "Distributor";

TWrapperParser::~TWrapperParser()
{
}

TXMLWorkflowTree::TPrioriTagInfo::TPrioriTagInfo() :
  handlers(), allowedParentTagIds(), allowedChildTagIds()
{
}

TXMLWorkflowTree::TPrioriTagInfo::TPrioriTagInfo(
  const TTagHandlersInfo& handlers,
  const std::list<TXMLTagInfo::ETagType::Type>& allowedParentTagIds,
  std::list<TXMLTagInfo::ETagType::Type>& allowedChildTagIds) :
  handlers(handlers), allowedParentTagIds(allowedParentTagIds),
  allowedChildTagIds(allowedChildTagIds)
{
}

TXMLWorkflowTree::TPrioriTagInfo::~TPrioriTagInfo()
{
}
