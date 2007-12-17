#include "CDataStack.hpp"

#include "Common.hpp"

#include "Tools.hpp"
#include "CSymbol.hpp"



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
   if (IS_LOCAL_ADDRESS(address)) {
      CBinString::setInt(_BS + realAddress(address), value);
   } else {
      CBinString::setInt(realAddress(address), value);
   }
//   CBinString::setInt(_BS * IS_LOCAL_ADDRESS(address) + realAddress(address), value);
}



int CDataStack::getInt(const int &address)
{
   if (IS_LOCAL_ADDRESS(address)) {
      return CBinString::getInt(_BS + realAddress(address));
   } else {
      return CBinString::getInt(realAddress(address));
   }
//   return CBinString::getInt(_BS * IS_LOCAL_ADDRESS(address) + realAddress(address));
}


//void CDataStack::setInt(const int &address, const int &value)
//{
////   std::cout << "setInt ";
//   if (IS_LOCAL_ADDRESS(address)) {
////      std::cout << "local address sem bit ligado: " << ((unsigned int)address & UNSET_BIT_LOCAL) << std::endl;
//      CBinString::setInt(_BS + realAddress(address), value);
//   } else {
//      std::cout << "ERRO !!! Invocando setInt com global address: " << address << std::endl;
//   }
//}



//int CDataStack::getInt(const int &address)
//{
////   std::cout << "getInt ";
//   if (IS_LOCAL_ADDRESS(address)) {
////      std::cout << "local address sem bit ligado: " << ((unsigned int)address & UNSET_BIT_LOCAL) << std::endl;
//      return CBinString::getInt(_BS + realAddress(address));
//   } else {
//      std::cout << "ERRO !!! Invocando getInt com global address: " << address << std::endl;
//      return -1;
//   }
//}


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


void CDataStack::pushByte(const char &value)
{
   CBinString::pushByte(value);
   _SP+=sizeof(char);
}


char CDataStack::popByte()
{
   char result = CBinString::popByte();
   _SP-=sizeof(char);
   return result;
}


void CDataStack::setReal(const int &address, const double &value)
{
   if (IS_LOCAL_ADDRESS(address)) {
      CBinString::setReal(_BS + realAddress(address), value);
   } else {
      CBinString::setReal(realAddress(address), value);
   }
}


double CDataStack::getReal(const int &address)
{
   if (IS_LOCAL_ADDRESS(address)) {
      return CBinString::getReal(_BS + realAddress(address));
   } else {
      return CBinString::getReal(realAddress(address));
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


void CDataStack::setBytes(const int &address, const std::string &value)
{
   if (IS_LOCAL_ADDRESS(address)) {
      CBinString::setBytes(_BS + realAddress(address), value);
   } else {
      CBinString::setBytes(realAddress(address), value);
   }
}


void CDataStack::pushBytes(const std::string &value)
{
   CBinString::pushBytes(value);
   _SP+=value.size();
}


std::string CDataStack::popBytes(const int &size)
{
   std::string result = CBinString::popBytes(size);
   _SP-=size;
   return result;
}


std::string CDataStack::getBytes(const int &address, const int &size)
{
   if (IS_LOCAL_ADDRESS(address)) {
      return CBinString::getBytes(_BS + realAddress(address), size);
   } else {
      return CBinString::getBytes(realAddress(address), size);
   }
}


void CDataStack::setString(int address, const std::string &value)
{
   if (IS_LOCAL_ADDRESS(address)) {
      address = _BS + realAddress(address);
   } else {
      address = realAddress(address);
   }

   char type = getByte(address);
   address++;

   if (type == CSymbol::CONST) {
      CBinString::setCString(address, value);
   } else {
      *((std::string*)(CBinString::getInt(address))) = value;
   }
}


std::string CDataStack::getString(int address)
{
   if (IS_LOCAL_ADDRESS(address)) {
      address = _BS + realAddress(address);
   } else {
      address = realAddress(address);
   }

   char type = CBinString::getByte(address);
   address++;

   if (type == CSymbol::CONST) {
      return CBinString::getCString(address);
//      return CBinString::getCString(CBinString::getInt(address)+1);
   } else {
      return *((std::string*)CBinString::getInt(address));
   }
}


std::string CDataStack::popString()
{
   int dataSize = sizeof(char)+sizeof(std::string*);
   std::string result = getString(size()-dataSize);
   CBinString::popBytes(dataSize);
   return result;
}

void CDataStack::pushBytes(const int &number)
{
   CBinString::pushBytes(number);
   _SP += number;
}


void CDataStack::discardBytes(const int &number)
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
   CBinString::resize(value);
   _SP = value;
}


void CDataStack::decSP(const int &value)
{
   CBinString::popBytes(value);
   _SP -= value;
}


int CDataStack::getBS() const
{
   return _BS;
}


int CDataStack::getSP() const
{
   return _SP;
}

void CDataStack::readString(std::string &value)
{
   CBinString::readString(value);
   setBS(size());
}


void CDataStack::setByte(const int &address, const char &value)
{
   if (IS_LOCAL_ADDRESS(address)) {
      CBinString::setByte(_BS + realAddress(address), value);
   } else {
      CBinString::setByte(realAddress(address), value);
   }
}


char CDataStack::getByte(const int &address)
{
   if (IS_LOCAL_ADDRESS(address)) {
      return CBinString::getByte(_BS + realAddress(address));
   } else {
      return CBinString::getByte(realAddress(address));
   }
}


void *CDataStack::getPointer(const int &address)
{
   if (IS_LOCAL_ADDRESS(address)) {
      return (void*)(data() + _BS + realAddress(address));
   } else {
      return (void*)(data() + realAddress(address));
   }
}


void CDataStack::pushBytes(const void* data, const int &size)
{
   const char* bytes = (const char*) data;
   for(int i=0; i<size; i++) {
      CBinString::pushByte(*bytes);
      bytes++;
   }

   _SP+=size;
}


