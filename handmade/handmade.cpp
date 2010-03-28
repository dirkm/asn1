#include "asn1/codec/tlv_iterator.hpp"
#include "asn1/codec/tlv_terminal.hpp"

namespace asn1
{
   namespace handmade
   {
      using boost::spirit::qi::parse;

      
      struct ParseSemantics
      {
      };

      typedef tlv_iterator<const char*> iter

      template<typename Iterator>
      void parse(Iterator it, Iterator itend, ParseSemantics sems)
      {
         // spirit micro parser
         check_type();
         check_constraints();
         first_element(it->value.first,it->value.second);
      }

      void parse(const char* it, const char* itend,ParseSemantics sems)
      {
         tlv_iterator tlv_it(it);
         tlv_iterator tlv_itend(itend);
         parse(tlv_it,tlv_itend,sems);

      }
   }
}

