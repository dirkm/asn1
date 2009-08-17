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
	  >> token(END_TOK)
	  ;

      moduleIdentifier =
	 moduleReference
	 >> assignedIdentifier;

      tagDefault=
	 -((token(IMPLICIT_TOK)|token(EXPLICIT_TOK))
	   >> token(TAGS_TOK))
	 ;

      moduleReference = tok.uppercaseFirst;

      assignedIdentifier = -objectIdComponentList;

      objectIdComponentList =
	 token(BEGIN_CURLY_BRACKET_TOK)
	 >> *objectIdComponent
	 >> token(END_CURLY_BRACKET_TOK)
	 ;

      objectIdComponent=
	 tok.number
	 | tok.lowercaseFirst
	 ;
	 // NameAndNumberForm;

      moduleBody=
	 (exports ^ imports)
	 >> assignmentList
	 ;

      exports=token(EXPORTS_TOK) >> exportList >> token(SEMICOLON_TOK);
      imports=token(IMPORTS_TOK) >> importList >> token(SEMICOLON_TOK);

      exportList=tok.uppercaseFirst % token(COMMA_TOK);
      importList=tok.uppercaseFirst % token(COMMA_TOK);

      assignmentList=
	 *assignment;

      typeAssignment=
	 tok.uppercaseFirst
	 >> token(IS_DEFINED_AS_TOK)
	 >> typeDef;

      booleanType=token(BOOLEAN_TOK);
      integerType=token(INTEGER_TOK);
      nullType=token(NULL_TOK);

      sequenceType=
	 token(SEQUENCE_TOK)
	 >> token(BEGIN_CURLY_BRACKET_TOK)
	 >> (elementType % token(COMMA_TOK))
	 >> token(END_CURLY_BRACKET_TOK)
	 ;

      sequenceOfType=
	 token(SEQUENCE_TOK)
	 >> -sizeConstraint
	 >> token(OF_TOK)
	 >> type
	 ;

      setType=token(SET_TOK);

      setOfType=
	 token(SET_TOK)
	 >> -sizeConstraint
	 >> token(OF_TOK)
	 >> type
	 ;

      choiceType=
	 token(CHOICE_TOK)
	 >> token(BEGIN_CURLY_BRACKET_TOK)
	 >> (elementType % token(COMMA_TOK))
	 >> token(END_CURLY_BRACKET_TOK)
	 ;

      builtinType=
	 booleanType
	 | integerType
	 | nullType
	 | sequenceType
	 | sequenceOfType
	 | setType
	 | setOfType
	 | choiceType
	 | bitString
	 | octetString
	 ;
      // | selectionType
      // | taggedType
      // | anyType
      // | objectIdentifierType
      // | enumeratedType
      // | realType;

      bitString=
	 token(BIT_TOK) >> token(STRING_TOK)
	 ;

      octetString=
	 token(OCTET_TOK) >> token(STRING_TOK)
	 ;

      typeDef=
	 subType | builtinType | taggedType | namedType;
	 ;

      taggedType=
	 tag
	 >> -token(IMPLICIT_TOK)
	 >> typeDef
	 ;

      valueAssignment =
	 tok.lowercaseFirst
	 >> typeDef
	 >> token(IS_DEFINED_AS_TOK)
	 >> value
	 ;

      value=
	 tok.number|tok.lowercaseFirst // for now
	 ; 
      
      assignment=
	 typeAssignment | valueAssignment
	 ;

      elementType=
	 tok.lowercaseFirst
	 >> typeDef
	 >> -(token(OPTIONAL_TOK)|token(DEFAULT_TOK)>>value /*namedValue*/)
	 ;

      namedType=
	 tok.uppercaseFirst
	 ;

      tag=
       	 token(BEGIN_SQUARE_BRACKET_TOK)
	 >> tagClass
	 >> tok.number
	 >> token(END_SQUARE_BRACKET_TOK)
	 ;

      tagClass=-(token(UNIVERSAL_TOK)|token(APPLICATION_TOK)|token(PRIVATE_TOK));

      sizeConstraint=
	 token(SIZE_TOK)
	 >> subtypeSpec;

      subtypeSpec=
	 token(BEGIN_BRACKET_TOK)
	 >> (subtypevalueSet % token(BAR_TOK))
	 >> token(END_BRACKET_TOK)
	 ;

      subtypevalueSet=
	 sizeConstraint
	 | valueRange
	 | singleValue
	 | containedSubtype;
      // | ValueRange
      // | PermittedAlphabet
      // | SizeConstraint
      // | InnerTypeConstraints
	 ;
      singleValue=value;

      containedSubtype=
	 token(INCLUDES_TOK)
	 >> type
	 ;

      valueRange=
	 value
	 >> (-token(LESSTHAN_TOK))
	 >> token(DOUBLEDOT_TOK)
	 >> (-token(LESSTHAN_TOK))
	 >> value
	 ;

      type=
	 // simply name of existing type
	 tok.uppercaseFirst|
	 builtinType;
	 ;
      
      subType=
	 type 
	 >> subtypeSpec;
	 ;
    }

   typedef in_state_skipper<Lexer> skipper_type;

   rule<Iterator,skipper_type> moduleDefinition,moduleReference,moduleIdentifier,moduleBody,
      tagDefault,
      assignedIdentifier,objectIdComponentList,objectIdComponent,
      exports, imports, assignmentList, assignment,
      exportList, importList,
      typeAssignment, valueAssignment,
      typeDef,builtinType,
      booleanType, integerType, nullType, sequenceType, sequenceOfType, setType,
      setOfType, choiceType,  taggedType, namedType,
      bitString, octetString,
   /*selectionType, anyType, objectIdentifierType, enumeratedType, realType */
      elementType, tag, tagClass, value,
      sizeConstraint,subtypeSpec,subtypevalueSet,
      singleValue, containedSubtype,valueRange,
      type,normalType,subType;
};

#endif
