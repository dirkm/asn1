#ifndef ASN1_CODEC_LENGTH_HPP
#define ASN1_CODEC_LENGTH_HPP

namespace asn1
{
  namespace codec
  {
    class length
    {
    public:
       typedef unsigned long length_type;
       
       length(length_type l):
          v(l)
       {
       };

       template<typename InIt>
       void decode(InIt& it, InIt itend)
       {
       };
    private:
       length_type v;
    };
  }
}

#endif

