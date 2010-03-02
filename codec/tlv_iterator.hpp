#ifndef ASN1_CODEC_TLV_ITERATOR_HPP
#define ASN1_CODEC_TLV_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>

#include "codec/length.hpp"
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

      template <class BaseIt>
      class tlv_iterator: public boost::iterator_adaptor<
         tlv_iterator<BaseIt>, // Derived
         BaseIt, 
         tag_value<BaseIt>, // Value
         boost::single_pass_traversal_tag,
         tag_value<BaseIt> // Reference
         >
      {
      public:
         explicit tlv_iterator(BaseIt& it)
            : tlv_iterator<BaseIt>::iterator_adaptor_(it)
         {
         }

         tlv_iterator()
            : tlv_iterator<BaseIt>::iterator_adaptor_(BaseIt())
         {
         }

         typedef tag_value<BaseIt> value_type;


         typename tlv_iterator::reference dereference() const
         {
            value_type val;
            BaseIt it=this->base();
            val.tag=asn1::codec::tag::decode(it);
            asn1::codec::length l=asn1::codec::length::decode(it);
            BaseIt startit=it;
            std::advance(it,l.get_value());
            val.value=typename value_type::value_boundaries(startit,it);
            return val;
         }
         
         void increment()
         {
            asn1::codec::tag::decode(this->base_reference());
            asn1::codec::length l=asn1::codec::length::decode(this->base_reference());
            std::advance(this->base_reference(),l.get_value());
         }
      };
   }
}

#endif
