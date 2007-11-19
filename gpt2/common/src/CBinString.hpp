#ifndef GPT_BIN_STRING_H
#define GPT_BIN_STRING_H

#include <string>


class CBinString : public std::string
{
public:
   void writeInt(const int &value);
   void writeByte(const char &value);
   void writeString(const std::string &value, const bool &writeSize=true);
   void writeBool(const bool &value);
   void readInt(int &value);
   void readByte(char &value);
   void readString(std::string &value);
   void readBool(bool &value);
   bool removeIfEqual(const int &value);
   bool removeIfEqual(const char &value);
   bool removeIfEqual(const std::string &value);
};

#endif
