#ifndef ASN1_CODEC_TAG_HPP
#define ASN1_CODEC_TAG_HPP

namespace asn1
{
  namespace codec
  {
    class tag
    {
    public:
      typedef unsigned long tag_type;
      static const uint8_t class_bitmask=0xC0;
      static const uint8_t format_bitmask=0x20;
      static const uint8_t tagid_bitmask=~(class_bitmask|format_bitmask); // 0x1F
    private:
      static const unsigned int shift_classformat=(sizeof(tag_type)-1)*8;
      static const tag_type id_mask=~(((tag_type)0xFF)<<shift_classformat);
    public:
      tag(uint8_t class_type, uint8_t format, tag_type id):
         v((((tag_type)(class_type|format))<<shift_classformat)|id)
      {
      };

      uint8_t get_class() const // shifted
      {
	return ((v>>shift_classformat) & class_bitmask);
      };

      uint8_t get_format() const // shifted
      {
	return ((v>>shift_classformat) & format_bitmask);
      };

      tag::tag_type get_id() const
      {
	return v & id_mask;
      };
    
      template<typename InIt>
      void decode(InIt& it, InIt itend)
      {
	uint8_t b=*it;
	v=((tag_type)(b&(class_bitmask|format_bitmask)))<<shift_classformat;
	if((b&tagid_bitmask)!=tagid_bitmask)
	  {
	    // single byte representation
	    v|=b&tagid_bitmask;
	  }
	else
	  {
	    // multiple byte representation
	    tag_type tag_id=0;
	    b=*(++it);
	    while(b&'\x80')
	      {
		tag_id<<=8;
		tag_id|=b;
		b=*(++it);
	      }
	    v|=tag_id;
	  }
      };
    private:
      tag_type v;
    };
  }
}

#endif
