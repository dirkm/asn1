#include "syntax/lex.hpp"

#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/lex_generate_static_lexertl.hpp>

#include <fstream>

using namespace boost::spirit;
using namespace boost::spirit::lex;

int main(int argc, char* argv[])
{
  using namespace asn1::syntax;
  // create the lexer object instance needed to invoke the generator
  asn1_tokens<lexertl::lexer<> > asn1_lexer; // the token definition
  if(argc < 2)
    {
      std::cerr << "syntax: " << argv[0] << " " << "<output_file>";
      exit(1);
    }
  std::ofstream out(argv[1]);
  return lexertl::generate_static(asn1_lexer, out, "asn1") ? 0 : -1;
}
