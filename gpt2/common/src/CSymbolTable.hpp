#ifndef GPT_SYMBOL_TABLE_H
#define GPT_SYMBOL_TABLE_H

#include <string>
#include <map>


#include "CSymbolList.hpp"


class CSymbolTable : public CSymbolList
{
public:
   CSymbolTable();
   ~CSymbolTable();
   CSymbol* addProcedure (const std::string &name, const int &type, const int &address, const bool &hasVarArguments, const int &staticParameters, std::vector<CSymbol> parameters);
   CSymbol* addParameter (const std::string &name, const int &type, const int &address);
   CSymbol* addConstant (const std::string &name, const int &type, const int &address);
   CSymbol* addVariable (const std::string &name, const int &type, const int &address);
   CSymbol* add(CSymbol *symbol);
   bool readFromBinary(CBinString &bin);
};

#endif

