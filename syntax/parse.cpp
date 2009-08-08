//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  The purpose of this example is to show, how it is possible to use a lexer 
//  token definition for two purposes:
//
//    . To generate C++ code implementing a static lexical analyzer allowing
//      to recognize all defined tokens 
//    . To integrate the generated C++ lexer into the /Spirit/ framework.
//

// #define BOOST_SPIRIT_LEXERTL_DEBUG
#define BOOST_VARIANT_MINIMIZE_SIZE

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
//[wc_static_include
#include <boost/spirit/include/lex_static_lexertl.hpp>
//]
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_container.hpp>

#include <iostream>
#include <string>

#include "syntax/parse.hpp" 
#include "syntax/read_from_file.hpp" 

using namespace boost::spirit;
using namespace boost::spirit::ascii;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;

int main(int argc, char* argv[])
{
    typedef lexertl::token<
        char const*, boost::mpl::vector<std::string>
    > token_type;

    typedef lexertl::static_lexer<
        token_type, lexertl::static_::lexer_asn1
    > lexer_type;

    // Define the iterator type exposed by the lexer.
    typedef asn1_tokens<lexer_type>::iterator_type iterator_type;

    asn1_tokens<lexer_type> word_count;           // Our lexer
    asn1_grammar<iterator_type> g (word_count);   // Our parser

    // Read in the file into memory.
    std::string str (read_from_file(1 == argc ? "word_count.input" : argv[1]));
    char const* first = str.c_str();
    char const* last = &first[str.size()];

    // Parsing is done based on the the token stream, not the character stream.
    bool r = tokenize_and_parse(first, last, word_count, g);

    if (r) {    // success
        std::cout << "lines: " << g.l << ", words: " << g.w 
                  << ", characters: " << g.c << "\n";
    }
    else {
        std::string rest(first, last);
        std::cerr << "Parsing failed\n" << "stopped at: \"" 
                  << rest << "\"\n";
    }
    return 0;
}
