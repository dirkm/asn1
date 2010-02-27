#ifndef ASN1_CODEC_TAGTYPES_HPP
#define ASN1_CODEC_TAGTYPES_HPP

namespace asn1
{
   namespace codec
   {
      enum tagClass
      {
         class_universal=0x00,
         class_application=0x40,
         class_context=0x80,
         class_private=0xC0
      };
      
      enum tagFormat
      {
         format_simple=0x00,
         format_constructed=0x20
      };
      
      enum tagCategory
      {
         category_implicit=0x01,
         category_explicit=0x02,
         category_untagged=0x04
      };
   }
}

#endif
