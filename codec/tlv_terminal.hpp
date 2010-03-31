#ifndef ASN1_CODEC_TLV_TERMINAL_HPP
#define ASN1_CODEC_TLV_TERMINAL_HPP

#include "codec/tlv.hpp"

#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/detail/assign_to.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

namespace asn1
{
   namespace terminal
   {
      BOOST_SPIRIT_TERMINAL_EX(asn1_tag);
   }
}

namespace boost
{
   namespace spirit
   {
      template <typename A0>
      struct use_terminal<
         qi::domain,
         terminal_ex<asn1::terminal::tag::asn1_tag,fusion::vector1<A0> > >
      : mpl::true_ {};
   }
}

namespace asn1
{
   namespace terminal
   {
      using boost::spirit::token;
      using boost::spirit::qi::primitive_parser;
      using boost::spirit::qi::info;

      template <typename Derived>
      struct tlv_parser : primitive_parser<Derived>
      {
         template <typename Context, typename Iterator>
         struct attribute
         {
            typedef asn1::codec::tag_value<Iterator> type;
         };

         template <typename Iterator, typename Context, typename Skipper, typename Attribute>
         bool parse(Iterator& first, Iterator const& last,
                    Context& context, Skipper const& /*skipper*/, Attribute& attr) const
         {
            if (first != last)
            {
               typedef typename boost::detail::iterator_traits<Iterator>::value_type token_type;
               token_type const& t = *first;
               if (this->derived().test(*first, context))
               {
                  boost::spirit::traits::assign_to(t, attr);
                  ++first;
                  return true;
               }
            }
            return false;
         }
      };

      ///////////////////////////////////////////////////////////////////////////
      
      // TODO, this should take a unary predicate
      
      struct asn1_tag_token
         : tlv_parser<asn1_tag_token>
      {
         asn1_tag_token(const asn1::codec::tag& tag_)
            : tag(tag_) {}

         template <typename ValueParam, typename Context>
         bool test(ValueParam tv, Context& context) const
         {
            return tag == tv.tag;
         }

         template <typename Context>
         info what(Context& /*context*/) const
         {
            return info("asn1_tag");
         }

         asn1::codec::tag tag;
      };
   }
}

namespace boost
{
   namespace spirit
   {
      namespace qi
      {
         ///////////////////////////////////////////////////////////////////////////
         // Parser generators: make_xxx function (objects)
         ///////////////////////////////////////////////////////////////////////////
// required change to make customizable on iterator (see plain token)
//         template <typename Modifiers, typename Iterator>
//         struct make_primitive<
//            terminal_ex<asn1::terminal::tag::asn1_tag,fusion::vector1<bool (tag_value<Iterator>)> >, Modifiers >

         template <typename Modifiers, typename A0>
         struct make_primitive<
            terminal_ex<asn1::terminal::tag::asn1_tag,fusion::vector1<A0> >, Modifiers >
         {
            typedef asn1::terminal::asn1_tag_token result_type;

            template <typename Terminal>
            result_type operator()(Terminal const& term, unused_type) const
            {
               return result_type(fusion::at_c<0>(term.args));
            }
         };
      }
   }
}

#endif
