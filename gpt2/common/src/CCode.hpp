#ifndef GPT_CODE_H
#define GPT_CODE_H


#include <string>

#include "CBinString.hpp"


class CCode
{
public:
   CCode();
   ~CCode();
   void addByte(const char &code);
   void addInt(const int &code);
   int size() const;
   CBinString getBinary() const;
   bool readFromBinary(CBinString &bin);
   char getByte(const int &pos);
   int getInt(const int &pos);
private:
   CBinString _data;
};

#endif

