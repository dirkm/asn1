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
         BaseIt, // Base
         tag_value<BaseIt>, // Value
         boost::forward_traversal_tag>
      {
      private:
         typedef boost::iterator_adaptor<tlv_iterator<BaseIt>,
                                         BaseIt, tag_value<BaseIt>,
                                         boost::forward_traversal_tag> super_type;
      public:
         explicit tlv_iterator(BaseIt& baseit)
            : super_type(baseit)
         {
            increment(); // init first value, happens to be the same method
         }

         tlv_iterator()
            : super_type(BaseIt())
         {
         }
         typedef tag_value<BaseIt> value_type;
      private:
         mutable value_type current_val;

         void increment()
         {
            current_val.tag=asn1::codec::tag::decode(this->base_reference());
            asn1::codec::length l=asn1::codec::length::decode(this->base_reference());
            BaseIt itstart=this->base_reference();
            std::advance(this->base_reference(),l.get_value());
            current_val.value=typename value_type::value_boundaries(itstart,this->base_reference());
         }
      public:
         BaseIt const& get_base_iterator() const
         {
            return this->base_reference();
         }

         BaseIt const& get_base_iterator()
         {
            return this->base_reference();
         }

         typename tlv_iterator::reference dereference() const
         {
            return current_val;
         }
      };
   }
}

#endif
