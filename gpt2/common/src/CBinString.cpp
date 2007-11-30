#include <iostream>

#include "CBinString.hpp"


void CBinString::writeInt(const int &value)
{
   resize(size()+sizeof(int));
   int *result = ((int*)(data()+size()))-1;
   *result = value;
}


void CBinString::writeByte(const char &value)
{
   push_back(value);
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
   push_back(value);
}


void CBinString::writeReal(const double &value)
{
   resize(size()+sizeof(double));
   double *result = ((double*)(data()+size()))-1;
   *result = value;
}


void CBinString::readInt(int &value)
{
   int result = *((int*)data());
   erase(0, sizeof(int));
   value = result;
//   std::cout << "readInt:" << value << std::endl;
}


void CBinString::readByte(char &value)
{
   value = (*this)[0];
   erase(0,1);
//   std::cout << "readByte:" << value << "(" << (int)value << ")" << std::endl;
}


void CBinString::readReal(double &value)
{
   double result = *((double*)data());
   erase(0, sizeof(double));
   value = result;
}


char CBinString::getByte(const int &pos)
{
   return (*this)[pos];
}

void CBinString::getByte(const int &pos, char &value)
{
   value = getByte(pos);
}

int CBinString::getInt(int pos)
{
   int result = *((int*)(data()+pos));
   return result;
}


double CBinString::getReal(int pos)
{
   double result = *((double*)(data()+pos));
   return result;
}


void CBinString::readString(std::string &value)
{
   int size;
   readInt(size);
   value=substr(0, size);
   erase(0, size);
//   std::cout << "readString:" << value << std::endl;
}


std::string CBinString::readString()
{
   std::string result;
   int size;
   readInt(size);
   result=substr(0, size);
   erase(0, size);
   return result;
}


void CBinString::readBool(bool &value)
{
   char byte;
   byte = (*this)[0];
   erase(0,1);
   value=byte;
//   std::cout << "readBool:" << value << std::endl;
}


void CBinString::setInt(int pos, const int &value)
{
   int *address = (int*)(data()+pos);
   *address = value;
}


void CBinString::setCString(int pos, const std::string &value)
{
   replace(pos, value.length()+1, value + '\0');
}


void CBinString::setReal(int pos, const double &value)
{
   double *address = (double*)(data()+pos);
   *address = value;
}


void CBinString::pushInt(const int &value)
{
   writeInt(value);
}


int CBinString::popInt()
{
   int pos = size()-sizeof(int);
   int result = *((int*)(data()+pos));

   erase(pos,sizeof(int));

   return result;
//   std::cout << "readInt:" << value << std::endl;
}


void CBinString::pushReal(const double &value)
{
   writeReal(value);
}


double CBinString::popReal()
{
   int pos = size()-sizeof(double);
   double result = *((double*)(data()+pos));

   erase(pos,sizeof(double));

   return result;
}


int CBinString::getLastInt() const
{
   int pos = size()-sizeof(int);
   int result = *((int*)(data()+pos));

   return result;
}


double CBinString::getLastReal() const
{
   int pos = size()-sizeof(double);
   double result = *((double*)(data()+pos));

   return result;
}


void CBinString::pushCString(const std::string &value)
{
   (*this) += value;
   (*this) += '\0';
}


void CBinString::pushBytes(const int &number)
{
   for (int i = 0; i < number; i++) {
      writeByte(0);
   }
}


void CBinString::popBytes(const int &number)
{
   int pos = size()-number;
   erase(pos,number);
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


std::string CBinString::getCString(const int &address)
{
   int pos = 0;

   pos = find('\0', address);

   if (pos == -1) {
      abort();
   }

   return substr(address, pos - address);
}


void CBinString::writeData(const void *value, const size_t &size)
{
   const char *byte = (const char*)value;

   for( size_t i = 0; i < size; i++) {
      push_back(*byte);
      byte++;
   }
}

