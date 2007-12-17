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
   void setBytes(const int &address, const std::string &value);
   std::string getBytes(const int &address, const int &size);
   void pushBytes(const std::string &value);
   std::string popBytes(const int &size);
   void pushBytes(const int &number);
   void discardBytes(const int &number);
   void setBS(const int &value);
   void setSP(const int &value);
   void decSP(const int &value);
   int getBS() const;
   int getSP() const;
   void readString(std::string &value);
   std::string getString(int address);
   std::string popString();
   void setString(int address, const std::string &value);
   void setByte(const int &address, const char &value);
   char getByte(const int &address);
   void pushByte(const char &value);
   char popByte();
   void *getPointer(const int &address);
   void pushBytes(const void* data, const int &size);
private:
   int _BS;
   int _SP; // TODO: SP eh equivalente a size() ???
};

#endif

