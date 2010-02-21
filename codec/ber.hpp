namespace asn1
{
  namespace codec
  {
    class tag
    {
    public:
      typedef unsigned long tagtype;
      static const uint8_t class_bitmask=0xC0;
      static const uint8_t format_bitmask=0x20;
      static const uint8_t tagid_bitmask=~(class_mask|format_mask); // 0x1F
    private:
      static const unsigned int shift_classformat=(sizeof(tagtype)-1)*8;
      static const tagtype id_mask=~(((tagtype)0xFF)<<shift_classformat);
    public:
      tag(uint8_t class_type, uint8_t format, tagtype id):
	v(((classtype|format)<<id_mask)+id)
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

      tag::tagtype get_id() const
      {
	return v & id_mask;
      };
    
      template<typename InIt>
      void decode(InIt& it, InIt itend)
      {
	uint8_t b=*it;
	v=(b&(class_bitmask|format_bitmask))<<shift_classformat;
	if((b&tagid_bitmask)!=tagid_bitmask)
	  {
	    // single byte representation
	    v|=b&tagid_bitmask;
	  }
	else
	  {
	    // multiple byte representation
	    tagtype tagid=0;
	    b=*(++it);
	    while(b&'\80')
	      {
		tagid<<=8;
		tagid|=b;
		b=*(++it);
	      }
	    v|=tagid;
	  }
      };
    private:
      tagtype v;
    };
  }
}
