#ifndef ASN1_PARSE_HPP
#define ASN1_PARSE_HPP

#include "syntax/lex_static.hpp" 
//#include "syntax/lex.hpp" 

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_container.hpp>

using namespace boost::spirit;
using namespace boost::spirit::ascii;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;

template <typename Iterator>
struct asn1_grammar : grammar<Iterator>
{
    template <typename TokenDef>
    asn1_grammar(TokenDef const& tok)
      : asn1_grammar::base_type(start)
      , c(0), w(0), l(0)
    {
        using boost::phoenix::ref;
        using boost::phoenix::size;

        //  associate the defined tokens with the lexer, at the same time 
        //  defining the actions to be executed 
        start =  *(   tok.word      [ ++ref(w), ref(c) += size(_1) ]
                  |   lit('\n')     [ ++ref(l), ++ref(c) ] 
                  |   token(IDANY)  [ ++ref(c) ]
                  )
              ;
    }

    std::size_t c, w, l;      // counter for characters, words, and lines
    rule<Iterator> start;
};

#endif
