#ifndef GPT_SYMBOL_LIST_H
#define GPT_SYMBOL_LIST_H

#include <string>
#include <vector>
#include <iostream>

#include <stdlib.h>

#include "CSymbol.hpp"
#include "CBinString.hpp"


class CSymbolList : public CBinString
{
public:
   CSymbolList();
   virtual ~CSymbolList();
   CSymbol* add(CSymbol* symbol);
   int getReference(const std::string &name);
   int getAddress( const std::string &name );
   CSymbol* getSymbol(const std::string &name);
   std::vector<CSymbol*>::iterator find(const std::string &name);
   int getDataSize() const
   {
      return size();
   }
   int getSymbolsCount() const
   {
      return _symbols.size();
   }
   bool readFromBinary(CBinString &bin);
   CBinString::find;
protected:
   std::vector<CSymbol*> _symbols;
private:
};

#endif

