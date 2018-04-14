#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
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
  virtual void Parse(const std::string& pathToWorkflowFile) = 0;
};

class TWrapperXMLParser : public TWrapperParser
{
public:
  TWrapperXMLParser();

  virtual ~TWrapperXMLParser();

  virtual void Parse(const std::string& pathToWorkflowFile);
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

  int EjectModulesCount();

  std::map<TModuleId::TWorkflowId, std::string>
    EjectModulesNamesMap(int modulesCount);
};

int main(int argc, char* argv[])
{
  std::string pathToWorkflowFile =
    "D:/git-projects/libexpat-example-build/bin/pingpong3_internal_windows_workflow.xml";

  TWrapperParser* parser = TWrapperParserFactory::CreateInstance("XML");
  parser->Parse(pathToWorkflowFile);

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

void TWrapperXMLParser::Parse(const std::string & pathToWorkflowFile)
{
  TWorkflowFileContent workflowFileContent;
  ReadWorkflowFile(pathToWorkflowFile, workflowFileContent);
  bool done = false;
  //XML_SetElementHandler(parser, start, end);
  //XML_SetCharacterDataHandler(parser, handle_data);
  if (XML_Parse(parser, workflowFileContent.buffer, workflowFileContent.bufferSize, XML_TRUE) == XML_STATUS_ERROR)
  {
    std::stringstream info;
    info << "Parsing error. Error :" << XML_ErrorString(XML_GetErrorCode(parser));
    throw std::runtime_error(info.str());
  }
  delete[] workflowFileContent.buffer;
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
