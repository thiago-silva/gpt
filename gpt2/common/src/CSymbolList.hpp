#ifndef GPT_SYMBOL_LIST_H
#define GPT_SYMBOL_LIST_H

#include <string>
#include <vector>
#include <iostream>

#include <stdlib.h>

#include "CSymbol.hpp"


class CSymbolList
{
public:
   CSymbolList();
   virtual ~CSymbolList();
   CSymbol* add(CSymbol* symbol);
   int getReference(const std::string &name);
   int getAddress( const std::string &name );
   CSymbol* getSymbol(const std::string &name);
   std::vector<CSymbol*>::iterator find(const std::string &name);
   CBinString getBinary() const
   {
      return _data;
   }
   int getDataSize() const
   {
      return _data.size();
   }
   int getSymbolsCount() const
   {
      return _symbols.size();
   }
   bool readFromBinary(CBinString &bin);
protected:
   std::vector<CSymbol*> _symbols;
   CBinString _data;
private:
};

#endif

