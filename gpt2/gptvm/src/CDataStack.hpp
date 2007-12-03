#ifndef GPT_DATA_STACK_H
#define GPT_DATA_STACK_H


#include "CBinString.hpp"


class CDataStack : public CBinString
{
public:
   CDataStack();
   ~CDataStack();
   void setInt(const int &address, const int &value);
   int getInt(const int &address);
   void pushInt(const int &value);
   int popInt();
   void setReal(const int &address, const double &value);
   double getReal(const int &address);
   void pushReal(const double &value);
   double popReal();
   void pushBytes(const int &number);
   void popBytes(const int &number);
   void setBS(const int &value);
   void setSP(const int &value);
   int getBS() const;
   int getSP() const;
   void readString(std::string &value);
   std::string getString(int address);
   void setString(int address, const std::string &value);
   void setByte(const int &address, const char &value);
   char getByte(const int &address);
   void pushByte(const char &value);
   char popByte();
private:
   int _BS;
   int _SP; // TODO: SP eh equivalente a size() ???
};

#endif

