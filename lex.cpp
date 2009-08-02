#include "lex.hpp"
#include "read_from_file.hpp"

int main(int argc, char* argv[])
{

   typedef lexertl::token<char const*, omitted, boost::mpl::false_> token_type;

   typedef lexertl::actor_lexer<token_type> lexer_type;

   asn1_tokens<lexer_type> asn1_lexer;


   std::string str (read_from_file(1 == argc ? "word_count.input" : argv[1]));
   char const* first = str.c_str();
   char const* last = &first[str.size()];
   
   lexer_type::iterator_type iter = asn1_lexer.begin(first, last);
   lexer_type::iterator_type end = asn1_lexer.end();
   
   while (iter != end && token_is_valid(*iter))
      ++iter;
   
   if (iter == end) {
   }
    else {
       std::string rest(first, last);
       std::cout << "Lexical analysis failed\n" << "stopped at: \"" 
		 << rest << "\"\n";
    }
   return 0;
}
