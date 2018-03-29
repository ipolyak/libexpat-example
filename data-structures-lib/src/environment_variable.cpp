#include "environment_variable.h"
#include "log.h"

namespace wrp
{

TEnvironmentVariable::TEnvironmentVariable(const std::string& name,
  const std::string& value) :
  name(name), value(value)
{
  WRP_LOG(ELogPriority::DEBUG, "START");
  WRP_LOG(ELogPriority::DEBUG, "FINISH");
}

} // namespace wrp
