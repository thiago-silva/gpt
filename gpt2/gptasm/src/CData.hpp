#ifndef GPT_DATA_H
#define GPT_DATA_H

#include <string>
#include <map>

#include "CSymbolList.hpp"


class CData : public CSymbolList
{
public:
   CData();
   ~CData();
   CSymbol* add(CSymbol *symbol);
   CSymbol* addVariable (const int &scope, const std::string &name, const int &type, const int &address);
   CSymbol* addConstant (const std::string &name, const int &type, const int &address);
};

#endif

