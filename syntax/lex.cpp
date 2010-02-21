#include "syntax/lex.hpp"
#include "syntax/read_from_file.hpp"
#include <string>

int main(int argc, char* argv[])
{
  using namespace asn1::syntax;
  typedef lexertl::token<char const*> token_type;

  typedef lexertl::actor_lexer<token_type> lexer_type;

  asn1_tokens<lexer_type> asn1_lexer;

  std::string str (read_from_file(1 == argc ? "data/asn1_comments.input" : argv[1]));
  char const* first = str.c_str();
  char const* last = &first[str.size()];

  lexer_type::iterator_type iter = asn1_lexer.begin(first, last);
  lexer_type::iterator_type end = asn1_lexer.end();

  while (iter != end && token_is_valid(*iter))
    {
      std::cout << " " << iter->value() <<  std::endl;
      ++iter;
    }

  if (iter == end)
    {
      std::cout << "Lexical analysis succeeded" << std::endl;
    }
  else
    {
      std::string rest(first, last);
      std::cout << "Lexical analysis failed\n" << "stopped at: \""
		<< rest << "\"\n";
    }
  return 0;
}
