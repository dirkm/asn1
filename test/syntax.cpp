//#define BOOST_SPIRIT_DEBUG
//#define BOOST_SPIRIT_LEXERTL_DEBUG

//#define BOOST_VARIANT_MINIMIZE_SIZE

#include "syntax/parse.hpp"
#include "syntax/lex.hpp"
#include <fstream>
#include <string>
#define BOOST_TEST_MODULE ASN1Syntax
#include <boost/test/included/unit_test.hpp>

std::string
read_from_file(const char* f)
{
  std::ifstream instream(f);
  BOOST_REQUIRE(instream.is_open());  
  instream.unsetf(std::ios::skipws); // No white space skipping!
  return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
		     std::istreambuf_iterator<char>());
}

// template<typename Iterator>
// bool lex(Iterator first,Iterator last)
// {
//   using namespace asn1::syntax;
//   typedef lexertl::token<Iterator> token_type;

//   typedef lexertl::actor_lexer<token_type> lexer_type;

//   asn1_tokens<lexer_type> asn1_lexer;

//   typename lexer_type::iterator_type iter = asn1_lexer.begin(first, last);
//   typename lexer_type::iterator_type end = asn1_lexer.end();
//   std::cout << token_is_valid(*iter) << std::endl;
//   while ((iter != end) && token_is_valid(*iter))
//     {
//       std::cout << " " << iter->value() <<  std::endl;
//       ++iter;
//       std::cout << token_is_valid(*iter) << std::endl;
//     }

//   return (iter == end);
// }

// BOOST_AUTO_TEST_CASE(parse_no_comments)
// {
//   const std::string& s=read_from_file("syntax_data/no_comments.asn1");
//   BOOST_CHECK(lex(s.begin(),s.end()));
// }

BOOST_AUTO_TEST_CASE(parse_full_example)
{
  const std::string& s=read_from_file("syntax_data/full_example.asn1");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(parse_empty_module)
{
  const std::string& s=read_from_file("syntax_data/empty_module.asn1");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(parse_sdhpm)
{
  const std::string& s=read_from_file("syntax_data/sdhpm.asn1");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(parse_single_line_comments)
{
  const std::string& s=read_from_file("syntax_data/single_line_comments.asn1");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(parse_partial_line_comments)
{
  const std::string& s=read_from_file("syntax_data/partial_line_comments.asn1");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(wrong_comment)
{
  const std::string& s=read_from_file("syntax_data/wrong_comment.asn1");
  BOOST_CHECK(!asn1::syntax::parse(s.begin(),s.end()));
}
