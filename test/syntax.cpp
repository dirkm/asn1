#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_LEXERTL_DEBUG

//#define BOOST_VARIANT_MINIMIZE_SIZE

#include <fstream>
#include <string>
#define BOOST_TEST_MODULE ASN1Syntax
#include <boost/test/included/unit_test.hpp>
#include "syntax/parse_fwd.hpp"
#include "test/read_from_file.hpp"

const char simple_module_prefix[]=
  "TEST DEFINITIONS IMPLICIT TAGS ::=\n"
  "BEGIN\n";

const char simple_module_suffix[]=
  "END\n";

std::string
pack_request(const char* p)
{
  std::string r(simple_module_prefix);
  r.append(p);
  r.append(simple_module_suffix);
  return r;
}

using namespace asn1::test;

BOOST_AUTO_TEST_CASE(parse_single_line_comments)
{
  const std::string& s=pack_request
    ("-- test\n"
     "-- test2\n");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(parse_partial_line_comments)
{
  const std::string& s=pack_request
    ("-- test -- \n");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}

BOOST_AUTO_TEST_CASE(wrong_comment)
{
  const std::string& s=pack_request
    ("- test -- \n");
  BOOST_CHECK(!asn1::syntax::parse(s.begin(),s.end()));
}

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

BOOST_AUTO_TEST_CASE(parse_handmade)
{
  const std::string& s=read_from_file("syntax_data/handmade.asn1");
  BOOST_CHECK(asn1::syntax::parse(s.begin(),s.end()));
}
