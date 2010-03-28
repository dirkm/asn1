#ifndef ASN1_CODEC_TAG_HPP
#define ASN1_CODEC_TAG_HPP

#include "codec/tagtypes.hpp"

namespace asn1
{
   namespace codec
   {
      class tag
      {
      public:
         typedef unsigned long tag_type;
         static const uint8_t class_bitmask='\xC0';
         static const uint8_t format_bitmask='\x20';
         static const uint8_t tagid_bitmask=(uint8_t)(~(class_bitmask|format_bitmask)); // 0x1F
      private:
         static const unsigned int shift_classformat=(sizeof(tag_type)-1)*8;
         static const tag_type id_mask=~(((tag_type)0xFF)<<shift_classformat);
      public:
         tag(uint8_t class_type, uint8_t format, tag_type id):
            v((((tag_type)(class_type|format))<<shift_classformat)|id)
         {
         }

         tag()
         {
         }

         tagClass get_class() const // shifted
         {
            return static_cast<tagClass>((v>>shift_classformat) & class_bitmask);
         }

         tagFormat get_format() const // shifted
         {
            return static_cast<tagFormat>((v>>shift_classformat) & format_bitmask);
         }

         tag::tag_type get_id() const
         {
            return v & id_mask;
         }
    
         template<typename InIt>
         static tag decode(InIt& it)
         {
            tag t;
            uint8_t b=*(it++);
            t.v=((tag_type)(b&(class_bitmask|format_bitmask)))<<shift_classformat;
            if((b&tagid_bitmask)!=tagid_bitmask)
            {
               // single byte representation
               t.v|=(b&tagid_bitmask);
            }
            else
            {
               // multiple byte representation
               tag_type tag_id=0;
               do
               {
                  b=*(it++);
                  tag_id<<=7;
                  tag_id|=(b&tag_type(0x7F));
               }
               while(b&'\x80');
          
               t.v|=tag_id;
            }
            return t;
         };
      private:
         tag_type v;
         friend bool operator==(const tag&l, const tag&r);
      };

      bool operator==(const tag&l, const tag&r)
      {
         return l.v==r.v;
      }
      
      bool operator!=(const tag&l, const tag&r)
      {
         return l!=r;
      }
   }
}

#endif
