#ifndef ASN1_PARSE_HPP
#define ASN1_PARSE_HPP

#include "syntax/lex_static.hpp"
#include "syntax/lex.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;

// template <typename Iterator>
// struct asn1_grammar : grammar<Iterator>
// {
//     template <typename TokenDef>
//     asn1_grammar(TokenDef const& tok)
//       : asn1_grammar::base_type(moduleDefinition)
//     {
//        moduleDefinition =
// 	  moduleIdentifier >>
// 	  token(DEFINITIONS_TOK) >>
// 	  token(BEGIN_TOK) >>
// 	  moduleBody >>
// 	  token(END_TOK);

//       moduleIdentifier =
// 	 moduleReference >>
// 	 assignedIdentifier;

//       moduleReference = tok.uppercaseFirst;

//       assignedIdentifier = -objectIdComponentList;

//       objectIdComponentList =
// 	 token(BEGIN_CURLY_BRACE_TOK) >>
// 	 *objectIdComponent >>
// 	 token(END_CURLY_BRACE_TOK);

//       objectIdComponent=
// 	 tok.number
// 	 | tok.lowercaseFirst;
// 	 // NameAndNumberForm;

//       moduleBody=
// 	 token(BEGIN_CURLY_BRACE_TOK) >>
// 	 token(END_CURLY_BRACE_TOK);
//     }

//    // typedef in_state_skipper<Lexer> skipper_type;

//    rule<Iterator> moduleDefinition,moduleReference,moduleIdentifier,moduleBody;
//    rule<Iterator> assignedIdentifier,objectIdComponentList,objectIdComponent;

template <typename Iterator, typename Lexer>
struct asn1_grammar : grammar<Iterator,in_state_skipper<Lexer> >
{
    template <typename TokenDef>
    asn1_grammar(TokenDef const& tok)
      : asn1_grammar::base_type(moduleDefinition)
    {
       moduleDefinition =
	  moduleIdentifier >>
	  token(DEFINITIONS_TOK) >>
	  token(BEGIN_TOK) >>
	  moduleBody >>
	  token(END_TOK);

      moduleIdentifier =
	 moduleReference >>
	 assignedIdentifier;

      moduleReference = tok.uppercaseFirst;

      assignedIdentifier = -objectIdComponentList;

      objectIdComponentList =
	 token(BEGIN_CURLY_BRACE_TOK) >>
	 *objectIdComponent >>
	 token(END_CURLY_BRACE_TOK);

      objectIdComponent=
	 tok.number
	 | tok.lowercaseFirst;
	 // NameAndNumberForm;

      moduleBody=
	 token(BEGIN_CURLY_BRACE_TOK) >>
	 token(END_CURLY_BRACE_TOK);
    }

   typedef in_state_skipper<Lexer> skipper_type;

   rule<Iterator,skipper_type> moduleDefinition,moduleReference,moduleIdentifier,moduleBody;
   rule<Iterator,skipper_type> assignedIdentifier,objectIdComponentList,objectIdComponent;

};

#endif
