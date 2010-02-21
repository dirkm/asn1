#include <fstream>
#include <string>

#include "syntax/parse.hpp"
#include "syntax/lex.hpp"

#include <boost/test/unit_test.hpp>

namespace asn1
{
  namespace test
  {
    std::string
    read_from_file(const char* f)
    {
      std::ifstream instream(f);
      BOOST_REQUIRE(instream.is_open());  
      instream.unsetf(std::ios::skipws); // No white space skipping!
      return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
			 std::istreambuf_iterator<char>());
    }
  }
  namespace syntax
  {
    // for compilation speedup
    template bool parse(std::string::const_iterator,std::string::const_iterator);
  }
}
