#ifndef ASN1_PARSE_HPP
#define ASN1_PARSE_HPP

#include "syntax/lex_static.hpp"
#include "syntax/lex.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;

template <typename Iterator, typename Lexer>
struct asn1_grammar : grammar<Iterator,in_state_skipper<Lexer> >
{
    template <typename TokenDef>
    asn1_grammar(TokenDef const& tok)
      : asn1_grammar::base_type(moduleDefinition)
    {
       moduleDefinition =
	  moduleIdentifier
	  >> token(DEFINITIONS_TOK)
	  >> tagDefault
	  >> token(IS_DEFINED_AS_TOK)
	  >> token(BEGIN_TOK)
	  >> moduleBody
	  >> token(END_TOK);

      moduleIdentifier =
	 moduleReference
	 >> assignedIdentifier;

      tagDefault=
	 -((token(IMPLICIT_TOK)|token(EXPLICIT_TOK))
	   >> token(TAGS_TOK));

      moduleReference = tok.uppercaseFirst;

      assignedIdentifier = -objectIdComponentList;

      objectIdComponentList =
	 token(BEGIN_CURLY_BRACKET_TOK)
	 >> *objectIdComponent
	 >> token(END_CURLY_BRACKET_TOK);

      objectIdComponent=
	 tok.number
	 | tok.lowercaseFirst;
	 // NameAndNumberForm;

      moduleBody=
	 (exports ^ imports)
	 >> -assignmentList;

      exports=token(EXPORTS_TOK) >> exportList >> token(SEMICOLON_TOK);
      imports=token(IMPORTS_TOK) >> importList >> token(SEMICOLON_TOK);

      exportList=tok.uppercaseFirst % token(COMMA_TOK);
      importList=tok.uppercaseFirst % token(COMMA_TOK);

      assignmentList=
	 assignment % token(COMMA_TOK);

      typeAssignment=
	 tok.uppercaseFirst
	 >> token(IS_DEFINED_AS_TOK)
	 >> typeDef;

      booleanType=token(BOOLEAN_TOK);
      integerType=token(INTEGER_TOK);
      bitStringType=token(BIT_STRING_TOK);
      nullType=token(NULL_TOK);

      sequenceType=
	 token(SEQUENCE_TOK)
	 >> token(BEGIN_CURLY_BRACKET_TOK)
	 >> (elementType % token(COMMA_TOK))
	 >> token(END_CURLY_BRACKET_TOK);

      sequenceOfType=token(SEQUENCE_OF_TOK);
      setType=token(SET_TOK);
      setOfType=token(SET_OF_TOK);
      choiceType=token(CHOICE_TOK);

      builtinType=
	 booleanType
	 | integerType
	 | bitStringType
	 | nullType
	 | sequenceType
	 | sequenceOfType
	 | setType
	 | setOfType
	 | choiceType;
      // | selectionType
      // | taggedType
      // | anyType
      // | objectIdentifierType
      // | enumeratedType
      // | realType;

      typeDef=builtinType;

      assignment=
	 typeAssignment;
	 // | valueAssignment;

      elementType=
	 tok.uppercaseFirst;

      tag=
       	 token(BEGIN_SQUARE_BRACKET_TOK)
	 >> tok.number
	 >> token(END_SQUARE_BRACKET_TOK);
      // selectionType=
      // taggedType=token
      // anyType
      // objectIdentifierType
      // enumeratedType
      // realType;
    }

   typedef in_state_skipper<Lexer> skipper_type;

   rule<Iterator,skipper_type> moduleDefinition,moduleReference,moduleIdentifier,moduleBody,
	    tagDefault,
	    assignedIdentifier,objectIdComponentList,objectIdComponent,
	    exports, imports, assignmentList, assignment,
	    exportList, importList,
	    typeAssignment, /*valueAssignment,*/
	    typeDef,builtinType,
	    booleanType, integerType, bitStringType, nullType, sequenceType, sequenceOfType, setType,
	    setOfType, choiceType, /*selectionType, taggedType, anyType, objectIdentifierType, enumeratedType, realType */
	    elementType, tag;
};

#endif
