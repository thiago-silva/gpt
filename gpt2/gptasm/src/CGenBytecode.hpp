#ifndef GPT_GEN_BYTECODE_H
#define GPT_GEN_BYTECODE_H

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "CHeader.hpp"
#include "CSymbolTable.hpp"
#include "CData.hpp"
#include "CCode.hpp"
#include "CBinString.hpp"

class CGenBytecode
{
public:
   CGenBytecode();
   ~CGenBytecode();
   void initProcedure(const std::string &procedureName, const bool &hasVarArguments, const int &staticParameters, std::vector<CSymbol> parameters);
   void finishProcedure();
   void makeVarDefinition(const std::string &lexeme, const int &type);
   void makeParDefinition(const std::string &lexeme, const int &type);
   void registryLabel(const std::string &labelName);
   void addOpcode(const std::string &mn);
   void addAddress(const std::string &id, const int &category, const int &type);
   CBinString getBinary();
private:
   CHeader                    _header;
   CSymbolTable               _symbolTable;
   CData                      _data;
   CCode                      _code;
   std::map<std::string,char> _opcodes;
   std::string                _currentProcedure;
};

#endif

