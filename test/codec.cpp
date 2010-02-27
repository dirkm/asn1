//#include <fstream>
//#include <string>
#define BOOST_TEST_MODULE ASN1Codec
#include <boost/test/included/unit_test.hpp>

#include "codec/length.hpp"
#include "codec/tag.hpp"
#include "codec/tagtypes.hpp"

BOOST_AUTO_TEST_CASE(single_byte_length)
{
   const char length_field[]={'\x10'};
   const char* it=length_field;
   asn1::codec::length l=asn1::codec::length::decode(it);
   BOOST_CHECK(it==length_field+1);
   BOOST_CHECK(l.get_value()==0x10);
}

BOOST_AUTO_TEST_CASE(two_byte_length)
{
   const char length_field[]={'\x81','\x80'};
   const char* it=length_field;
   asn1::codec::length l=asn1::codec::length::decode(it);
   BOOST_CHECK(it==length_field+2);
   BOOST_CHECK(l.get_value()==0x80);
}

BOOST_AUTO_TEST_CASE(multi_byte_length)
{
   const char length_field[]={'\x83','\x12','\x34','\x56'};
   const char* it=length_field;
   asn1::codec::length l=asn1::codec::length::decode(it);
   BOOST_CHECK(it==length_field+4);
   BOOST_CHECK(l.get_value()==0x123456);
}

BOOST_AUTO_TEST_CASE(single_byte_tag)
{
   const char tag_field[]={'\x78'};
   const char* it=tag_field;
   asn1::codec::tag t=asn1::codec::tag::decode(it);
   BOOST_CHECK(it==tag_field+1);
   BOOST_CHECK(t.get_class()==asn1::codec::class_application);
   BOOST_CHECK(t.get_format()==asn1::codec::format_constructed);
   BOOST_CHECK(t.get_id()==0x18);
}

BOOST_AUTO_TEST_CASE(two_byte_tag)
{
   const char tag_field[]={'\x1F','\x24'};
   const char* it=tag_field;
   asn1::codec::tag t=asn1::codec::tag::decode(it);
   BOOST_CHECK(it==tag_field+2);
   BOOST_CHECK(t.get_class()==asn1::codec::class_universal);
   BOOST_CHECK(t.get_format()==asn1::codec::format_simple);
   BOOST_CHECK(t.get_id()==0x24);
}

BOOST_AUTO_TEST_CASE(multi_byte_tag)
{
   const char tag_field[]={'\x1F','\x81','\x91','\x11'};
   const char* it=tag_field;
   asn1::codec::tag t=asn1::codec::tag::decode(it);
   BOOST_CHECK(it==tag_field+4);
   BOOST_CHECK(t.get_class()==asn1::codec::class_universal);
   BOOST_CHECK(t.get_format()==asn1::codec::format_simple);
   BOOST_CHECK(t.get_id()==0x4891);
}
