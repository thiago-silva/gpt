#include "CHeader.hpp"


CHeader::CHeader()
{
   const std::string versionStr = "GVM version 0.1";
   _data.writeInt(0x1234);
   _data.writeString(versionStr);
   _data.writeByte(0);
   _data.writeByte(1);
}


CHeader::~CHeader()
{
}


CBinString CHeader::getBinary()
{
   return _data;
}


bool CHeader::readFromBinary(CBinString &bin)
{
   bool result = bin.removeIfEqual(0x1234) &&
                 bin.removeIfEqual("GVM version 0.1") &&
                 bin.removeIfEqual((char)0) &&
                 bin.removeIfEqual((char)1);

   if (result == false) {
      return false;
   }

   return true;
}

