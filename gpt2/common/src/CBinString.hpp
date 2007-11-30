#ifndef GPT_BIN_STRING_H
#define GPT_BIN_STRING_H

#include <string>

// TODO: muitos desses metodos poderiam ser implementados ou como templates ou entao com um metodo generico que receba aonde ler/gravar a informacao e o tamanho do dado (sizeof(tipo))

class CBinString : public std::string
{
public:
   void writeInt(const int &value);
   void writeByte(const char &value);
   void writeString(const std::string &value, const bool &writeSize=true);
   void writeBool(const bool &value);
   void writeReal(const double &value);
   void readInt(int &value);
   void readByte(char &value);
   void readReal(double &value);
   char getByte(const int &pos);
   void getByte(const int &pos, char &value);
   int getInt(int pos);
   double getReal(int pos);
   void readString(std::string &value);
   std::string readString();
   void readBool(bool &value);
   void setInt(int pos, const int &value);
   void setCString(int pos, const std::string &value);
   void setReal(int pos, const double &value);
   void pushInt(const int &value);
   int popInt();
   void pushReal(const double &value);
   double popReal();
   int getLastInt() const;
   double getLastReal() const;
   void pushCString(const std::string &value);
   std::string popCString();
   void pushBytes(const int &number);
   void popBytes(const int &number);
   bool removeIfEqual(const int &value);
   bool removeIfEqual(const char &value);
   bool removeIfEqual(const std::string &value);
   std::string getCString(const int &address);
   std::string::find;
};

#endif
