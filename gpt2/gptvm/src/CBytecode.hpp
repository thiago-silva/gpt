#ifndef GPT_BYTECODE_H
#define GPT_BYTECODE_H


#include "CBinString.hpp"


class CBytecode : public CBinString
{
public:
   CBytecode();
  ~CBytecode();
   char fetchByte();
   int  fetchInt();
   void setIP(const int &IP);
   CBinString::readString;
private:
   int _IP;
};

#endif

