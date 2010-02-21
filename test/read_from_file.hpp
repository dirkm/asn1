#ifndef ASN1_TEST_READ_FROM_FILE
#define

namespace asn1
{
  namespace test
  {

    extern
    std::string
    read_from_file(const char* f)
    {
      std::ifstream instream(f);
      BOOST_REQUIRE(instream.is_open());  
      instream.unsetf(std::ios::skipws); // No white space skipping!
      return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
			 std::istreambuf_iterator<char>());
    }
  }
}
#endif
