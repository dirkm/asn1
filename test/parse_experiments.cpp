#define BOOST_TEST_MODULE ASN1ParseExperiments
#include <boost/test/included/unit_test.hpp>

#include "test/codec_util.hpp"
#include "codec/tlv_iterator.hpp"
#include "codec/tlv_terminal.hpp"

#include <boost/spirit/include/qi.hpp>

using asn1::test::end;

asn1::terminal::asn1_tag_token<asn1::test::tlv_array_iterator>  a(asn1::codec::tag(0,0,0));

template <typename Iterator>
struct manual_parser : boost::spirit::qi::grammar<Iterator, typename Iterator::value_type()>
{
    manual_parser() : manual_parser::base_type(start)
    {
       //start=asn1::terminal::asn1_tag(asn1::codec::tag(0,0,0));
       start=asn1::terminal::asn1_tag_token<asn1::test::tlv_array_iterator>(asn1::codec::tag(0,0,0));
    }

   boost::spirit::qi::rule<Iterator,typename Iterator::value_type()> start;
};

BOOST_AUTO_TEST_CASE(manual_parser_test)
{
   const char single_tlv[]={'\x1F','\x24','\x04','\x01','\x02','\x03','\x04'};
   const char* it=single_tlv;
   const char* itend=end(single_tlv);
   asn1::test::tlv_array_iterator tlv_it(it);
   asn1::test::tlv_array_iterator tlv_itend(itend);
   
   using boost::spirit::qi::parse;
   bool r = parse(
      tlv_it, tlv_itend,
      manual_parser<asn1::test::tlv_array_iterator>()
      );
}
// BOOST_AUTO_TEST_CASE(integrated_parser)
// {
//    const char single_tlv[]={'\x1F','\x24','\x04','\x01','\x02','\x03','\x04'};
//    const char* it=single_tlv;
//    const char* itend=end(single_tlv);
//    asn1::test::tlv_array_iterator tlv_it(it);
   
//    using boost::spirit::qi::parse;
//    bool r = parse(
//       it, itend,
//       //  Begin grammar
//       *asn1::terminal::asn1_tag(0,0,0)
//       );
//    //  End grammar
// }
