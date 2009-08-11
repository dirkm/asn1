#ifndef ASN1_LEX_HPP
#define ASN1_LEX_HPP

#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit;
using namespace boost::spirit::lex;

enum tokenids 
{
    DEFINITIONS_TOK = boost::spirit::lex::min_token_id + 1,
    IS_DEFINED_AS_TOK,
    COMMA_TOK,
    SEMICOLON_TOK,
    BEGIN_TOK,
    END_TOK,
    BEGIN_CURLY_BRACKET_TOK,
    END_CURLY_BRACKET_TOK,
    BEGIN_SQUARE_BRACKET_TOK,
    END_SQUARE_BRACKET_TOK,
    EXPORTS_TOK,
    IMPORTS_TOK,
    IMPLICIT_TOK,
    EXPLICIT_TOK,
    TAGS_TOK,
    BOOLEAN_TOK,
    INTEGER_TOK,
    BIT_STRING_TOK,
    NULL_TOK,
    STRING_TOK,
    OCTET_TOK,
    SEQUENCE_TOK,
    SEQUENCE_OF_TOK,
    SET_TOK,
    SET_OF_TOK,
    CHOICE_TOK,
    ANY_TOK,
    OBJECT_IDENTIFIER_TOK,
    ENUMERATED_TOK,
    REAL_TOK,
    OPTIONAL_TOK
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
	  boost::spirit::lex::token_def<>("::=", IS_DEFINED_AS_TOK)|
	  boost::spirit::lex::token_def<>(",", COMMA_TOK)|
	  boost::spirit::lex::token_def<>(";", SEMICOLON_TOK)|
	  boost::spirit::lex::token_def<>("BEGIN", BEGIN_TOK)|
	  boost::spirit::lex::token_def<>("SEQUENCE", SEQUENCE_TOK)|
	  boost::spirit::lex::token_def<>("END", END_TOK)|
	  boost::spirit::lex::token_def<>("EXPORTS", EXPORTS_TOK)|
	  boost::spirit::lex::token_def<>("IMPORTS", IMPORTS_TOK)|
	  boost::spirit::lex::token_def<>("IMPLICIT", IMPLICIT_TOK)|
	  boost::spirit::lex::token_def<>("EXPLICIT", EXPLICIT_TOK)|
	  boost::spirit::lex::token_def<>("TAGS", TAGS_TOK)|
	  boost::spirit::lex::token_def<>("BOOLEAN", BOOLEAN_TOK)|
	  boost::spirit::lex::token_def<>("INTEGER", INTEGER_TOK)|
	  boost::spirit::lex::token_def<>("BIT STRING", BIT_STRING_TOK)|
	  boost::spirit::lex::token_def<>("NULL", NULL_TOK)|
	  boost::spirit::lex::token_def<>("STRING", STRING_TOK)|
	  boost::spirit::lex::token_def<>("OCTET", OCTET_TOK)|
	  boost::spirit::lex::token_def<>("SEQUENCE", SEQUENCE_TOK)|
	  boost::spirit::lex::token_def<>("SEQUENCE OF", SEQUENCE_OF_TOK)|
	  boost::spirit::lex::token_def<>("SET", SET_TOK)|
	  boost::spirit::lex::token_def<>("SET_OF", SET_OF_TOK)|
	  boost::spirit::lex::token_def<>("CHOICE", CHOICE_TOK)|
	  boost::spirit::lex::token_def<>("ANY", ANY_TOK)|
	  boost::spirit::lex::token_def<>("OBJECT IDENTIFIER", OBJECT_IDENTIFIER_TOK)|
	  boost::spirit::lex::token_def<>("ENUMERATED", ENUMERATED_TOK)|
	  boost::spirit::lex::token_def<>("REAL", REAL_TOK)|
	  boost::spirit::lex::token_def<>("OPTIONAL", OPTIONAL_TOK)|
	  boost::spirit::lex::token_def<>("\\{", BEGIN_CURLY_BRACKET_TOK)|
	  boost::spirit::lex::token_def<>("\\}", END_CURLY_BRACKET_TOK)|
	  boost::spirit::lex::token_def<>("\\[", BEGIN_SQUARE_BRACKET_TOK)|
	  boost::spirit::lex::token_def<>("\\]", END_SQUARE_BRACKET_TOK)|
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
