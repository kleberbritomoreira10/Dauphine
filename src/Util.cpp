#include "Util.h"
#include <sstream>

std::string Util::toString ( const int number_to_be_parameterized )
{
  std::string result;
  std::ostringstream oss;
  oss.str( "" );
  oss.clear();
  oss << number_to_be_parameterized;
  result = oss.str();
  return result;
}
