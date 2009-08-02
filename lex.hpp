#ifndef ASN1_LEX_HPP
#define ASN1_LEX_HPP

#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit;
using namespace boost::spirit::lex;

enum tokenids 
{
    IDANY = boost::spirit::lex::min_token_id + 1,
};

template <typename BaseLexer>
struct asn1_tokens: boost::spirit::lex::lexer<BaseLexer> 
{
    asn1_tokens()
      : asn1_tokens::base_type(
          boost::spirit::lex::match_flags::match_not_dot_newline)
    {
        // define tokens and associate them with the lexer
        word = "[^ \t\n]+";
        this->self = word | '\n' | boost::spirit::lex::token_def<>(".", IDANY);

        // define the whitespace to ignore (spaces, tabs, newlines and C-style 
        // comments)
        this->self("WS")
            =   token_def<>("[ \\t\\n]+")
	    | "--(-[^\\-\\n]|[^\\-\\n])*(--|\\n|-\\n)" 
            ;
    }
    boost::spirit::lex::token_def<std::string> word;
};

#endif
