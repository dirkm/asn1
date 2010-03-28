#ifndef ASN1_CODEC_LENGTH_HPP
#define ASN1_CODEC_LENGTH_HPP

#include <stdint.h>
#include <assert.h>

namespace asn1
{
  namespace codec
  {
    class length
    {
    public:
       typedef std::size_t length_type;

       length(length_type l):
          v(l)
       {
       }

       template<typename InIt>
       static length decode(InIt& it)
       {
          length l;
          const uint8_t b=*(it++);
          const uint8_t len=b&'\x7F';
          if(len==b) // most significant bit set
          {
             l.v=b;
          }
          else if (b=='\x80')
          {
             assert(false); // don't know
          }
          else
          {
             l.v=0;
             for(uint8_t i=0;i<len;++i)
             {
                l.v<<=8;
                l.v|=(*(it++))&length_type(0xFF);
             }
          }
          return l;
       }

       length_type get_value() const
       {
          return v;
       }
    private:
       length()
       {
       }

       length_type v;
    };
  }
}

#endif
