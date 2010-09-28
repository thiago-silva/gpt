#ifndef GPT_HEADER_H
#define GPT_HEADER_H

#include <string>

#include "CBinString.hpp"


class CHeader
{
public:
   CHeader();
   ~CHeader();
   CBinString getBinary();
   bool readFromBinary(CBinString &bin);
private:
   CBinString _data;
};

#endif

