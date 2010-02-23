#ifndef ASN1_SYNTAX_PARSE_FWD_HPP
#define ASN1_SYNTAX_PARSE_FWD_HPP

namespace asn1
{
  namespace syntax
  {
    template<typename Iterator>
    bool parse(Iterator first,Iterator last);
  }
}

#endif
