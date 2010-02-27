#ifndef ASN1_CODEC_TLV_HPP
#define ASN1_CODEC_TLV_HPP

#include "codec/tag.hpp"
#include "codec/length.hpp"
#include "codec/value.hpp"

namespace asn1
{
   namespace codec
   {
      struct tlv
      {
         template<typename InIt>
         static tlv decode(InIt& it)
         {
            tlv r;
            r.tag=decode(it);
            length l=decode(it);
            Init itend=it;
            std::advance(itend,l.get_value());
            r.value=value(it,itend);
         };
         // length is encoded within value
         tag tag;
         value value;
      };
   }
}

#endif
