#include "CDataStack.hpp"


#include <iostream>


CDataStack::CDataStack()
   : _BS(0)
   , _SP(0)
{
}


CDataStack::~CDataStack()
{
}


void CDataStack::setInt(const int &address, const int &value)
{
//   std::cout << "setInt ";
   if (address & 0x80000000) {
//      std::cout << "local address sem bit ligado: " << (address & 0x7FFFFFFF) << std::endl;
      CBinString::setInt(_BS + (address & 0x7FFFFFFF), value);
   } else {
      std::cout << "ERRO !!! Invocando setInt com global address: " << address << std::endl;
   }
}


int CDataStack::getInt(const int &address)
{
//   std::cout << "getInt ";
   if (address & 0x80000000) {
//      std::cout << "local address sem bit ligado: " << (address & 0x7FFFFFFF) << std::endl;
      return CBinString::getInt(_BS + (address & 0x7FFFFFFF));
   } else {
      std::cout << "ERRO !!! Invocando getInt com global address: " << address << std::endl;
      return -1;
   }
}


void CDataStack::pushInt(const int &value)
{
   CBinString::pushInt(value);
   _SP+=sizeof(int);
}


int CDataStack::popInt()
{
   int result = CBinString::popInt();
   _SP-=sizeof(int);
   return result;
}


void CDataStack::pushBytes(const int &number)
{
   CBinString::pushBytes(number);
   _SP += number;
}


void CDataStack::popBytes(const int &number)
{
   CBinString::popBytes(number);
   _SP -= number;
}


void CDataStack::setBS(const int &value)
{
   _BS = value;
}


void CDataStack::setSP(const int &value)
{
   _SP = value;
}


int CDataStack::getBS() const
{
   return _BS;
}


int CDataStack::getSP() const
{
   return _SP;
}

