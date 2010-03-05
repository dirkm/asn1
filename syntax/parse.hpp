#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_LEXERTL_DEBUG

#ifndef ASN1_SYNTAX_PARSE_HPP
#define ASN1_SYNTAX_PARSE_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_static_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#include "syntax/lex_static.hpp"
#include "syntax/lex.hpp"

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
               -(exports ^ imports)
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
               | enumeratedType
               ;
            // | selectionType
            // | taggedType
            // | anyType
            //
            // 
            // | realType;

            bitString=
               token(BIT_TOK) >> token(STRING_TOK)
               ;

            octetString=
               token(OCTET_TOK) >> token(STRING_TOK)
               ;

            objectIdentifier=
               token(OBJECT_TOK)
               >> token(IDENTIFIER_TOK)
               ;

            enumeratedType=
               token(ENUMERATED_TOK)
               >> token(BEGIN_CURLY_BRACKET_TOK)
               >> (namedNumber % token(COMMA_TOK))
               >> token(END_CURLY_BRACKET_TOK)
               ;
            
            namedNumber=
               tok.lowercaseFirst
               >> token(BEGIN_BRACKET_TOK)
               >> tok.number
               >> token(END_BRACKET_TOK)
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
            oidValue = objectIdComponentList.alias();

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
               ( token(SIZE_TOK) >> subtypeSpec )
               |
               ( token(BEGIN_BRACKET_TOK) >> sizeConstraint >> token(END_BRACKET_TOK) )
               ;

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
               fullSpecification.alias();
            // | partialSpecification;

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

         rule<Iterator,skipper_type> moduleDefinition,
            moduleIdentifier,tagDefault,
            moduleReference,
            assignedIdentifier,
            objectIdComponentList,
            moduleBody,
            exports, imports,
            exportList, importList,
            symbolFromModuleList,
            assignmentList,
            typeAssignment,
            booleanType, integerType, nullType, sequenceType,
            sequenceOfType, setType,
            setOfType, choiceType,
            builtinType,
            bitString, octetString, objectIdentifier,
            enumeratedType,namedNumber,
            typeDef,
            taggedType,
            valueAssignment,
            booleanValue,nullValue,oidValue,
            builtinValue,
            externalValueReference,definedValue,
            value,
            assignment,
            objectIdComponent,
            nameAndNumberForm,
            elementType,namedType,
            tag,tagClass,
            sizeConstraint,subtypeSpec,subtypevalueSet,
            singleValue,
         /*selectionType, anyType, objectIdentifierType, enumeratedType, realType */
            containedSubtype,
            singleTypeConstraint,multipleTypeConstraint,
            fullSpecification,
         /* partialSpecification, */
            namedConstraint,constraint,
            presenceConstraint,innerTypeConstraints,
            valueRange,type,subType;
      };

      template<typename Iterator>
      bool parse(Iterator first,Iterator last)
      {
         typedef lexertl::token<Iterator, boost::mpl::vector<std::string> > token_type;

         // typedef lexertl::lexer<token_type> lexer_type;
         typedef lexertl::static_lexer< token_type, typename lexertl::static_::lexer_asn1> lexer_type;

         typedef asn1_tokens<lexer_type> asn1_lex_t;
         asn1_lex_t asn1_lex;
         asn1_grammar<typename asn1_lex_t::iterator_type,typename asn1_lex_t::lexer_def> g (asn1_lex);

         bool r = tokenize_and_phrase_parse
            (first, last, asn1_lex, g,
             in_state("WS")[asn1_lex.self],boost::spirit::qi::skip_flag::postskip);
         // boost::spirit::qi::skip_flag::dont_postskip
         // if (r && (first==last))
         // 	{    // success
         // 	  std::cerr << "keep it real" << std::endl;
         // 	}
         // else
         // 	{
         // 	  std::string rest(first, last);
         // 	  std::cerr << "Parsing failed\n" << "stopped at: \""
         // 		    << rest << "\"\n";
         // 	}

         return (r && (first==last));
      }

   }
}
#endif
