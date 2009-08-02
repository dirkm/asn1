#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/lex_generate_static_lexertl.hpp>

#include <fstream>
#include "lex.hpp"

using namespace boost::spirit;
using namespace boost::spirit::lex;

int main(int argc, char* argv[])
{
    // create the lexer object instance needed to invoke the generator
    asn1_tokens<lexertl::lexer<> > asn1_lexer; // the token definition

    std::ofstream out(argc < 2 ? "lex_static.hpp" : argv[1]);
    return lexertl::generate_static(asn1_lexer, out, "wc") ? 0 : -1;
}
