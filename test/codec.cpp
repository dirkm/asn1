//#include <fstream>
//#include <string>
#define BOOST_TEST_MODULE ASN1Codec
#include <boost/test/included/unit_test.hpp>

#include "codec/length.hpp"
#include "codec/tag.hpp"

BOOST_AUTO_TEST_CASE(single_byte_length)
{
   const char length_field[]={'\x10'};
   const char* it=length_field;
   asn1::codec::length l=l.decode(it);
   BOOST_CHECK(it==length_field+1);
   BOOST_CHECK(l.get_value()==0x10);
}

BOOST_AUTO_TEST_CASE(two_byte_length)
{
   const char length_field[]={'\x81','\x80'};
   const char* it=length_field;
   asn1::codec::length l=l.decode(it);
   BOOST_CHECK(it==length_field+2);
   BOOST_CHECK(l.get_value()==0x80);
}

BOOST_AUTO_TEST_CASE(multi_byte_length)
{
   const char length_field[]={'\x83','\x12','\x34','\x56'};
   const char* it=length_field;
   asn1::codec::length l=l.decode(it);
   BOOST_CHECK(it==length_field+4);
   BOOST_CHECK(l.get_value()==0x123456);
}

