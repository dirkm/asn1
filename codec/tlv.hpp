#ifndef ASN1_CODEC_TLV_HPP
#define ASN1_CODEC_TLV_HPP

#include "codec/tag.hpp"

namespace asn1
{
   namespace codec
   {
      template<typename BaseIt>
      struct tag_value
      {
         typedef std::pair<BaseIt,BaseIt> value_boundaries;
         asn1::codec::tag tag;
         value_boundaries value;
      };
   }
}

#endif
