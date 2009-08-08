#ifndef ASN1_PARSE_HPP
#define ASN1_PARSE_HPP

template <typename Iterator>
struct asn1_grammar : grammar<Iterator>
{
    template <typename TokenDef>
    asn1_grammar(TokenDef const& tok)
      : word_count_grammar::base_type(start)
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
