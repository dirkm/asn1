#ifndef ASN1_CODEC_TLV_ITERATOR_HPP
#define ASN1_CODEC_TLV_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>

#include "codec/length.hpp"
#include "codec/tag.hpp"
#include <boost/optional.hpp>

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
      class tlv_iterator: public boost::iterator_facade<
         tlv_iterator<BaseIt>, // Derived
         tag_value<BaseIt>, // Value
         boost::single_pass_traversal_tag // do is_convertible trick
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
         // optimization because dereference and increment use similar algorithms
         mutable boost::optional<BaseIt> next_it;
         mutable value_type current_val;
         
      public:
         typename tlv_iterator::reference dereference() const
         {
            next_it=current_it;
            current_val.tag=asn1::codec::tag::decode(*next_it);
            asn1::codec::length l=asn1::codec::length::decode(*next_it);
            BaseIt startit=*next_it;
            std::advance(*next_it,l.get_value());
            current_val.value=typename value_type::value_boundaries(startit,*next_it);
            return current_val;
         }
         
         template <class OtherBaseIt>
         bool equal(const tlv_iterator<OtherBaseIt>& rval) const
         {
            return current_it==rval.current_it;
         }

         void increment()
         {
            if(!next_it)
            {
               next_it=current_it;
               asn1::codec::tag::decode(*next_it);
               asn1::codec::length l=asn1::codec::length::decode(*next_it);
               std::advance(*next_it,l.get_value());
            }
            current_it=*next_it;
            next_it=boost::none_t();
         }
      };
   }
}

#endif
