#include <iostream>

#include "CBinString.hpp"


void CBinString::writeInt(const int &value)
{
   char *byte = (char*)&value;

   for( size_t i = 0; i < sizeof(int); i++) {
      push_back(*byte);
      byte++;
   }
//   std::cout << "writeInt:" << value << std::endl;
}


void CBinString::writeByte(const char &value)
{
   (*this) += value;
//   std::cout << "writeByte:" << value << "(" << (int)value << ")" << std::endl;
}


void CBinString::writeString(const std::string &value, const bool &writeSize)
{
   if (writeSize) {
      writeInt(value.length());
   }
   (*this) += value;
   if (!writeSize) {
      (*this) += '\0';
   }
//   std::cout << "writeString: [" << value << "]" << std::endl;
}


void CBinString::writeBool(const bool &value)
{
   char byte = (char)value;

   (*this) += byte;
//   std::cout << "writeBool:" << value << std::endl;
}


void CBinString::readInt(int &value)
{
   int result = 0;
   char *byte = (char*)&result;

   for( size_t i = 0; i < sizeof(int); i++) {
      *byte = (*this)[0]; // TODO
      byte++;
      erase(0,1);
   }
   value = result;
//   std::cout << "readInt:" << value << std::endl;
}


void CBinString::readByte(char &value)
{
   value = (*this)[0];
   erase(0,1);
//   std::cout << "readByte:" << value << "(" << (int)value << ")" << std::endl;
}


void CBinString::readString(std::string &value)
{
   int size;
   readInt(size);
   value=substr(0, size);
   erase(0, size);
//   std::cout << "readString:" << value << std::endl;
}


void CBinString::readBool(bool &value)
{
   char byte;
   byte = (*this)[0];
   erase(0,1);
   value=byte;
//   std::cout << "readBool:" << value << std::endl;
}


bool CBinString::removeIfEqual(const int &value)
{
   // TODO: nao ta correto pq sempre retira...
   int data;
   readInt(data);
   return value == data;
}


bool CBinString::removeIfEqual(const char &value)
{
   // TODO: nao ta correto pq sempre retira...
   char data;
   readByte(data);
   return value == data;
}


bool CBinString::removeIfEqual(const std::string &value)
{
   // TODO: nao ta correto pq sempre retira...
   std::string data;
   readString(data);
   return value == data;
}

