#ifndef GPT_GEN_BYTECODE_H
#define GPT_GEN_BYTECODE_H

#include <map>
#include <string>
#include <list>

#include "CHeader.hpp"
#include "CSymbolTable.hpp"
#include "CData.hpp"
#include "CBinString.hpp"

struct SParameter
{
   std::string _name;
   int         _type;
   int         _size;
   SParameter(std::string name, int type, int size)
      : _name(name)
      , _type(type)
      , _size(size)
   { }
};

class CGenBytecode
{
public:
   CGenBytecode();
   ~CGenBytecode();
   void initProcedure(const std::string &procedureName, const bool &hasVarArguments, const int &staticParameters, std::vector<CSymbol> parameters);
   void finishProcedure();
   void makeVarDefinition(const std::string &lexeme, const int &type, int size);
   void makeParDefinition(const std::string &lexeme, const int &type, int size);
   void finishParDefinition();
   void registryLabel(const std::string &labelName);
   void addOpcode(const std::string &mn);
   void addAddress(const std::string &id, const int &category, const int &type, int size = 0);
   CBinString getBinary();
   void unsolvedLabel(const std::string &label);
   void translateLabelsToAddress();
   void addSymbolSize(const std::string &symbol);
private:

   CHeader                    _header;
   CSymbolTable               _symbolTable;
   CData                      _data;
   CBinString                 _code;
   std::map<std::string,char> _opcodes;
   std::string                _currentProcedure;
   int                        _currentSP;
//   int                        _parametersSize;
   int                        _raSize;
   std::list<std::pair<std::string, int> > _unsolvedLabels;
   std::map<std::string, int> _solvedLabels;
//   std::list<std::pair<std::string,int> > _parameters;
   std::list<SParameter> _parameters;
};

#endif

