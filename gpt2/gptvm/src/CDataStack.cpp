#include "CDataStack.hpp"

#include "Common.hpp"

#include "Tools.hpp"



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
   if (IS_LOCAL_ADDRESS(address)) {
//      std::cout << "local address sem bit ligado: " << ((unsigned int)address & UNSET_BIT_LOCAL) << std::endl;
      CBinString::setInt(_BS + realAddress(address), value);
   } else {
      std::cout << "ERRO !!! Invocando setInt com global address: " << address << std::endl;
   }
}


int CDataStack::getInt(const int &address)
{
//   std::cout << "getInt ";
   if (IS_LOCAL_ADDRESS(address)) {
//      std::cout << "local address sem bit ligado: " << ((unsigned int)address & UNSET_BIT_LOCAL) << std::endl;
      return CBinString::getInt(_BS + realAddress(address));
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


void CDataStack::setReal(const int &address, const double &value)
{
   if (IS_LOCAL_ADDRESS(address)) {
      CBinString::setReal(_BS + realAddress(address), value);
   } else {
      std::cout << "ERRO !!! Invocando setReal com global address: " << address << std::endl;
   }
}


double CDataStack::getReal(const int &address)
{
   if (IS_LOCAL_ADDRESS(address)) {
      return CBinString::getReal(_BS + realAddress(address));
   } else {
      std::cout << "ERRO !!! Invocando getReal com global address: " << address << std::endl;
      return -1;
   }
}


void CDataStack::pushReal(const double &value)
{
   CBinString::pushReal(value);
   _SP+=sizeof(double);
}


double CDataStack::popReal()
{
   double result = CBinString::popReal();
   _SP-=sizeof(double);
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

