#ifndef ASN1_CODEC_TLV_ITERATOR_HPP
#define ASN1_CODEC_TLV_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>

#include "codec/length.hpp"
#include "codec/tlv.hpp"
#include <boost/optional.hpp>

namespace asn1
{
   namespace codec
   {
      template <class BaseIt>
      class tlv_iterator: public boost::iterator_facade<
         tlv_iterator<BaseIt>, // Derived
         tag_value<BaseIt>, // Value
         boost::forward_traversal_tag // do is_convertible trick
         >
      {
      public:
         explicit tlv_iterator(BaseIt it)
            : current_it(it)
         {
         }
         explicit tlv_iterator()
            : current_it(BaseIt())
         {
         }
         typedef tag_value<BaseIt> value_type;
      private:
         BaseIt current_it;
         mutable boost::optional<value_type> current_val;
         
      public:
         typename tlv_iterator::reference dereference() const
         {
            if(!current_val)
            {
               BaseIt it=current_it;
               current_val=value_type();
               current_val->tag=asn1::codec::tag::decode(it);
               asn1::codec::length l=asn1::codec::length::decode(it);
               BaseIt startit=it;
               std::advance(it,l.get_value());
               current_val->value=typename value_type::value_boundaries(startit,it);
            }
            return *current_val;
         }
         
         template <class OtherBaseIt>
         bool equal(const tlv_iterator<OtherBaseIt>& rval) const
         {
            return current_it==rval.current_it;
         }

         void increment()
         {
            if(!current_val)
            {
               asn1::codec::tag::decode(current_it);
               asn1::codec::length l=asn1::codec::length::decode(current_it);
               std::advance(current_it,l.get_value());
            }
            else
            {
               // dereference and increment use similar algorithms
               // so, we get the iterator from the last dereference
               current_it=current_val->value.second;
               current_val=boost::none_t();
            }
         }
      };
   }
}

#endif
