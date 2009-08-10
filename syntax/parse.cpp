#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_LEXERTL_DEBUG

#define BOOST_VARIANT_MINIMIZE_SIZE

// #include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_static_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_container.hpp>

#include <iostream>
#include <string>

#include "syntax/parse.hpp"
#include "syntax/read_from_file.hpp"

using namespace boost::spirit;
using namespace boost::spirit::ascii;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;

int main(int argc, char* argv[])
{
   typedef lexertl::token<
   char const*, boost::mpl::vector<std::string> > token_type;
    
   // typedef lexertl::lexer<token_type> lexer_type;
   typedef lexertl::static_lexer< token_type, lexertl::static_::lexer_asn1> lexer_type;

   typedef asn1_tokens<lexer_type> asn1_lex_t;
   asn1_lex_t asn1_lex;
   asn1_grammar<asn1_lex_t::iterator_type,asn1_lex_t::lexer_def> g (asn1_lex);
   
   // Read in the file into memory.
   std::string str (read_from_file(1 == argc ? "data/asn1_comments.input" : argv[1]));
   char const* first = str.c_str();
   char const* last = &first[str.size()];

   bool r = tokenize_and_phrase_parse(
      first, last, asn1_lex, g,
      in_state("WS")[asn1_lex.self],boost::spirit::qi::skip_flag::postskip);
   // boost::spirit::qi::skip_flag::dont_postskip
   if (r && (first==last))
   {    // success
      std::cerr << "keep it real" << std::endl;
   }
   else 
   {
      std::string rest(first, last);
      std::cerr << "Parsing failed\n" << "stopped at: \""
		<< rest << "\"\n";
   }
   return 0;
}
