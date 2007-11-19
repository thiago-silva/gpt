#include "CCode.hpp"


CCode::CCode()
{
}


CCode::~CCode()
{
}


void CCode::addByte(const char &code)
{
   _data += code;
}


void CCode::addInt(const int &code)
{
   const char *byte = (const char*) &code;

   for (size_t i = 0; i < sizeof(int); i++) {
      _data += *byte;
      byte++;
   }
}


int CCode::size() const
{
   return _data.size();
}


CBinString CCode::getBinary() const
{
   return _data;
}


bool CCode::readFromBinary(CBinString &bin)
{
   bin.readString(_data);
   return true;
}


char CCode::getByte(const int &pos)
{
   return _data[pos];
}


int CCode::getInt(const int &pos)
{
   int *ret = (int*)&(_data[pos]);
   return *ret;
}

