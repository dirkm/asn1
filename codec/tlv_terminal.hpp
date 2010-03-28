#ifndef ASN1_CODEC_TLV_TERMINAL_HPP
#define ASN1_CODEC_TLV_TERMINAL_HPP

#include "codec/tlv.hpp"

#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/qi/skip_over.hpp>
#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/detail/assign_to.hpp>
#include <boost/range/iterator_range.hpp>
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
      using boost::spirit::qi::skip_over;
      using boost::spirit::qi::info;

      ///////////////////////////////////////////////////////////////////////////
      struct asn1_tag_token
         : primitive_parser<asn1_tag_token>
      {
         template <typename Context, typename Iterator>
         struct attribute
         {
            typedef typename Iterator::value_type type;
         };

         asn1_tag_token(const asn1::codec::tag& tag_)
            : tag(tag_) {}

         template <typename Iterator, typename Context
                   , typename Skipper, typename Attribute>
         bool parse(Iterator& first, Iterator const& last
                    , Context& /*context*/, Skipper const& skipper
                    , Attribute& attr) const
         {
            if (first != last) {
               typedef typename
                  boost::detail::iterator_traits<Iterator>::value_type
                  token_type;

               token_type const& t = *first;
               if (tag == t.tag) {
                  boost::spirit::traits::assign_to(t, attr);
                  ++first;
                  return true;
               }
            }
            return false;
         }

         template <typename Context>
         info what(Context& /*context*/) const
         {
            return info("tag");
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
