//#include <fstream>
//#include <string>
#define BOOST_TEST_MODULE ASN1Codec
#include <boost/test/included/unit_test.hpp>

#include "codec/length.hpp"
#include "codec/tag.hpp"
#include "codec/tagtypes.hpp"
#include "codec/tlv_iterator.hpp"

namespace
{
   template<typename T, std::size_t size> T* end(T (&array)[size])
   {
      return array+size;
   }
}

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

typedef asn1::codec::tlv_iterator<const char*> tlv_array_type;

BOOST_AUTO_TEST_CASE(single_step_iterator_tag)
{
   const char single_tlv[]={'\x1F','\x24','\x04','\x01','\x02','\x03','\x04'};
   const char* it=single_tlv;
   tlv_array_type tlv_it(it);
   tlv_array_type tlv_it2(it);
   // see test two_byte_tag
   const asn1::codec::tag& t=tlv_it->tag;
   const tlv_array_type::value_type::value_boundaries& v=tlv_it->value;
   BOOST_CHECK(t.get_class()==asn1::codec::class_universal);
   BOOST_CHECK(t.get_format()==asn1::codec::format_simple);
   BOOST_CHECK(t.get_id()==0x24);
   BOOST_CHECK(v.second==(v.first+4));
   BOOST_CHECK(v.first==(single_tlv+3));
   BOOST_CHECK(std::equal(v.first,v.second,single_tlv+3));
   BOOST_CHECK(tlv_it==tlv_it);
   BOOST_CHECK(tlv_it==tlv_it2);
   ++tlv_it;
   BOOST_CHECK(tlv_it!=tlv_it2);
   BOOST_CHECK(tlv_array_type(end(single_tlv))==tlv_it);
}

BOOST_AUTO_TEST_CASE(two_step_iterator_tag)
{
   char two_tlv[4+3+0x80]={'\x67','\x02','\xBE','\xAF',
                             '\xC1','\x81','\x80'};
   std::fill_n(two_tlv+4+3+1,0x80,'\x11');
   const char* it=two_tlv;
   tlv_array_type tlv_it(it);
   const asn1::codec::tag& t=tlv_it->tag;
   const tlv_array_type::value_type::value_boundaries& v=tlv_it->value;
   BOOST_CHECK(t.get_class()==asn1::codec::class_application);
   BOOST_CHECK(t.get_format()==asn1::codec::format_constructed);
   BOOST_CHECK(t.get_id()==0x07);
   BOOST_CHECK(std::equal(v.first,v.second,two_tlv+2));
   ++tlv_it;
   const asn1::codec::tag& t2=tlv_it->tag;
   const tlv_array_type::value_type::value_boundaries& v2=tlv_it->value;
   BOOST_CHECK(t2.get_class()==asn1::codec::class_private);
   BOOST_CHECK(t2.get_format()==asn1::codec::format_simple);
   BOOST_CHECK(t2.get_id()==0x01);
   BOOST_CHECK(std::distance(v2.first,v2.second)==0x80);
   BOOST_CHECK(std::equal(v2.first,v2.second,two_tlv+7));
}
