#define BOOST_TEST_MODULE ASN1ParseExperiments
#include <boost/test/included/unit_test.hpp>

#include "test/codec_util.hpp"
#include "codec/tlv_iterator.hpp"
#include "codec/tlv_terminal.hpp"

#include <boost/spirit/include/qi.hpp>

using asn1::test::end;

asn1::terminal::asn1_tag_token  a(asn1::codec::tag(0,0,0));

template <typename Iterator>
struct manual_parser1 : boost::spirit::qi::grammar<Iterator, typename Iterator::value_type()>
{
    manual_parser1() : manual_parser1::base_type(start)
    {
       start=asn1::terminal::asn1_tag_token(asn1::codec::tag(0,0,0));
    }

   boost::spirit::qi::rule<Iterator,typename Iterator::value_type()> start;
};

using boost::spirit::qi::parse;

BOOST_AUTO_TEST_CASE(manual_parser_test1)
{
   const char single_tlv[]={'\x1F','\x24','\x04','\x01','\x02','\x03','\x04'};
   asn1::test::tlv_array_iterator tlv_it(single_tlv);
   asn1::test::tlv_array_iterator tlv_itend(end(single_tlv));

   bool r = parse(
      tlv_it, tlv_itend,
      manual_parser1<asn1::test::tlv_array_iterator>()
      );
}

template <typename Iterator>
struct manual_parser2 : boost::spirit::qi::grammar<Iterator, typename Iterator::value_type()>
{
    manual_parser2() : manual_parser2::base_type(start)
    {
       start=asn1::terminal::asn1_tag(asn1::codec::tag(0,0,0));
    }

   boost::spirit::qi::rule<Iterator,typename Iterator::value_type()> start;
};

BOOST_AUTO_TEST_CASE(manual_parser_test2)
{
   const char single_tlv[]={'\x1F','\x24','\x04','\x01','\x02','\x03','\x04'};
   asn1::test::tlv_array_iterator tlv_it(single_tlv);
   asn1::test::tlv_array_iterator tlv_itend(end(single_tlv));
   
   bool r = parse(
      tlv_it, tlv_itend,
      manual_parser2<asn1::test::tlv_array_iterator>()
      );
}

BOOST_AUTO_TEST_CASE(integrated_parser)
{
   const char single_tlv[]={'\x1F','\x24','\x04','\x01','\x02','\x03','\x04'};
   asn1::test::tlv_array_iterator tlv_it(single_tlv);
   asn1::test::tlv_array_iterator tlv_itend(end(single_tlv));
   
   bool r = parse(
      tlv_it, tlv_itend,
      //  Begin grammar
            *asn1::terminal::asn1_tag(asn1::codec::tag(0,0,0))
      );
   //  End grammar
}
