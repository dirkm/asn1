#ifndef ASN1_TEST_CODEC_UTIL
#define ASN1_TEST_CODEC_UTIL

#include "codec/tlv_iterator.hpp"

namespace asn1
{
   namespace test
   {
      typedef asn1::codec::tlv_iterator<const char*> tlv_array_iterator;
      
      template<typename T, std::size_t size> T* end(T (&array)[size])
      {
         return array+size;
      }
   }
}

#endif
