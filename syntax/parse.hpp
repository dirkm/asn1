#ifndef ASN1_PARSE_HPP
#define ASN1_PARSE_HPP

#include "syntax/lex_static.hpp" 
#include "syntax/lex.hpp" 

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;

template <typename Iterator>
struct asn1_grammar : grammar<Iterator>
{
    template <typename TokenDef>
    asn1_grammar(TokenDef const& tok)
      : asn1_grammar::base_type(start)
    {
	start =  *(token(IDANY));
    }
    rule<Iterator> start;
};

#endif
