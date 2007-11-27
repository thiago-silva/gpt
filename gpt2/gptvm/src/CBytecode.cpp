#include "CBytecode.hpp"

#include <iostream>

CBytecode::CBytecode()
   : _IP(0)
{
}


//CBytecode::~CBytecode()
//{
//}


char CBytecode::fetchByte()
{
   if (_IP >= size()) {
      std::cout << "Erro limite em fetchByte()" << std::endl;
      abort();
   }
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


int CBytecode::getIP() const
{
   return _IP;
}

