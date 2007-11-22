#include "CBytecode.hpp"

CBytecode::CBytecode()
   : _IP(0)
{
}


CBytecode::~CBytecode()
{
}


char CBytecode::fetchByte()
{
   char result = getByte(_IP);
   _IP += sizeof(char);
   return result;
}


int  CBytecode::fetchInt()
{
   int result = getInt(_IP);
   _IP += sizeof(int);
   return result;
}


void CBytecode::setIP(const int &IP)
{
   _IP = IP;
}

