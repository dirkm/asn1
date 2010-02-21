#ifndef ASN1_PARSE_HPP
#define ASN1_PARSE_HPP

#include "syntax/lex_static.hpp"
#include "syntax/lex.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

namespace asn1
{
  namespace syntax
  {
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
	  | (tok.lowercaseFirst >> -(token(BEGIN_BRACKET_TOK) >> tok.number >> token(END_BRACKET_TOK)))
	  ;

	moduleBody=
	  (exports ^ imports)
	  >> assignmentList
	  ;

	exports=token(EXPORTS_TOK) >> exportList >> token(SEMICOLON_TOK);
	imports=token(IMPORTS_TOK) >> importList >> token(SEMICOLON_TOK);

	exportList=(tok.uppercaseFirst % token(COMMA_TOK));
	importList=+symbolFromModuleList;

	symbolFromModuleList=
	  (tok.uppercaseFirst % token(COMMA_TOK))
	  >> token(FROM_TOK)
	  >> moduleIdentifier
	  ;

	assignmentList=
	  *assignment
	  ;

	typeAssignment=
	  tok.uppercaseFirst
	  >> token(IS_DEFINED_AS_TOK)
	  >> typeDef
	  ;

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

	setType=
	  token(SET_TOK)
	  >> token(BEGIN_CURLY_BRACKET_TOK)
	  >> (elementType % token(COMMA_TOK))
	  >> token(END_CURLY_BRACKET_TOK)
	  ;

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
	  | sequenceOfType
	  | sequenceType
	  | setOfType
	  | setType
	  | choiceType
	  | bitString
	  | octetString
	  | objectIdentifier
	  ;
	// | selectionType
	// | taggedType
	// | anyType
	// 
	// | enumeratedType
	// | realType;

	objectIdentifier=
	  token(OBJECT_TOK) 
	  >> token(IDENTIFIER_TOK)
	  ;

	bitString=
	  token(BIT_TOK) >> token(STRING_TOK)
	  ;

	octetString=
	  token(OCTET_TOK) >> token(STRING_TOK)
	  ;

	typeDef=
	  subType | 
	  builtinType | 
	  taggedType | 
	  namedType
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

	booleanValue = token(TRUE_TOK)|token(FALSE_TOK);
	nullValue = token(NULL_TOK);
	oidValue = objectIdComponentList;
    
	builtinValue = tok.number|booleanValue|nullValue|oidValue;

	externalValueReference= moduleReference >> token(DOT_TOK) >> tok.lowercaseFirst;

	definedValue= externalValueReference| tok.lowercaseFirst;
	
	value=
	  builtinValue| definedValue;

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
	  | containedSubtype
	  // | PermittedAlphabet
	  | innerTypeConstraints
	  ;

	singleValue=value;

	containedSubtype=
	  token(INCLUDES_TOK)
	  >> type
	  ;

	singleTypeConstraint=subtypeSpec;
    
	multipleTypeConstraint=
	  fullSpecification
	  | partialSpecification;
 
	fullSpecification=
	  token(BEGIN_CURLY_BRACKET_TOK)
	  >> (namedConstraint % token(COMMA_TOK))
	  >> token(END_CURLY_BRACKET_TOK)
	  ;

	namedConstraint=(-tok.lowercaseFirst) >> constraint;
    
	constraint=
	  (-subtypeSpec)
	  >>(-presenceConstraint)
	  ;
    
	presenceConstraint=
	  token(PRESENT_TOK)|
	  token(ABSENT_TOK)|
	  token(OPTIONAL_TOK);

	innerTypeConstraints=
	  token(WITH_TOK) 
	  >>((token(COMPONENT_TOK) >> singleTypeConstraint)
	     | (token(COMPONENTS_TOK) >> multipleTypeConstraint))
	  ;
      
	valueRange=
	  value
	  >> (-token(LESSTHAN_TOK))
	  >> token(DOUBLEDOT_TOK)
	  >> (-token(LESSTHAN_TOK))
	  >> value
	  ;

	type=
	  builtinType|
	  // simply name of existing type
	  tok.uppercaseFirst;
	;
      
	subType=
	  type 
	  >> subtypeSpec;
	;
      }

      typedef in_state_skipper<Lexer> skipper_type;

      rule<Iterator,skipper_type> moduleDefinition,moduleReference,moduleIdentifier,moduleBody,
	tagDefault,
	assignedIdentifier,objectIdComponentList,objectIdComponent,nameAndNumberForm,
	exports, imports, assignmentList, assignment,
	exportList, importList,symbolFromModuleList,
	typeAssignment, valueAssignment,
	typeDef,builtinType,
	booleanType, integerType, nullType, sequenceType, sequenceOfType, setType,
	setOfType, choiceType,  taggedType, namedType,
	bitString, octetString,
	objectIdentifier,
      /*selectionType, anyType, objectIdentifierType, enumeratedType, realType */
	elementType, tag, tagClass, value,builtinValue,booleanValue,nullValue,oidValue,definedValue,externalValueReference,
	sizeConstraint,subtypeSpec,subtypevalueSet,
	singleValue, containedSubtype,innerTypeConstraints,singleTypeConstraint,multipleTypeConstraint,fullSpecification,
	partialSpecification,namedConstraint,constraint,presenceConstraint,valueRange,
	type,normalType,subType;
    };

#endif

  }
}
