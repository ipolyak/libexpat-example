#ifndef ENVIRONMENT_VARIABLE_H_
#define ENVIRONMENT_VARIABLE_H_

#include <string>

struct TEnvironmentVariable
{
  std::string name;
  std::string value;

  TEnvironmentVariable(const std::string& name = std::string(),
    const std::string& value = std::string());
};

#endif  // ENVIRONMENT_VARIABLE_H_
