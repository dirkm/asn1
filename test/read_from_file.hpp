#ifndef ASN1_TEST_READ_FROM_FILE
#define ASN1_TEST_READ_FROM_FILE

namespace asn1
{
   namespace test
   {

      template<typename T, std::size_t size> T* end(T (&array)[size])
      {
         return array+size;
      }

      extern
      std::string
      read_from_file(const char* f);
   }
}

#endif
