#ifndef ASN1_LEX_HPP
#define ASN1_LEX_HPP

#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit;
using namespace boost::spirit::lex;

enum tokenids 
{
    DEFINITIONS_TOK = boost::spirit::lex::min_token_id + 1,
    BEGIN_TOK,
    END_TOK,
    BEGIN_CURLY_BRACE_TOK,
    END_CURLY_BRACE_TOK
};

template <typename BaseLexer>
struct asn1_tokens: boost::spirit::lex::lexer<BaseLexer> 
{
    asn1_tokens()
    {
       uppercaseFirst = "[A-Z][a-zA-Z0-9-]*";
       lowercaseFirst = "[a-z][a-zA-Z0-9-]*";
       number = "0|([1-9][0-9]*)";

       this->self = 
	  boost::spirit::lex::token_def<>("DEFINITIONS", DEFINITIONS_TOK)|
	  boost::spirit::lex::token_def<>("BEGIN", BEGIN_TOK)|
	  boost::spirit::lex::token_def<>("END", END_TOK)|
	  boost::spirit::lex::token_def<>("\\{", BEGIN_CURLY_BRACE_TOK)|
	  boost::spirit::lex::token_def<>("\\}", END_CURLY_BRACE_TOK)|
	  number|
	  uppercaseFirst|
	  lowercaseFirst;

       this->self("WS")
            =   token_def<>("[ \\t\\n]+")
	    | "--(-[^\\-\\n]|[^\\-\\n])*(--|\\n|-\\n)" 
            ;
    }
   boost::spirit::lex::token_def<std::string> uppercaseFirst;
   boost::spirit::lex::token_def<std::string> lowercaseFirst;
   boost::spirit::lex::token_def<std::string> number;
};

#endif
