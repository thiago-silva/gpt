#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <list>
#include <map>

#include "Types.hpp"

class Symbol;
class SymbolList;

class SymbolTable {
public:
  //void setCurrentUnit(string file)
  //void loadSymbolTable(other) //load stable from other units

  void setScope(const std::string& scope);
  void setGlobalScope();

  Type* getType(const std::string& name);

  Type* getType(int id);

  Type* retrieveMatrixType(Type *ofType, int dimensions);

  Symbol newSymbol(const std::string& name, Type* type,
                   const std::string& scope, int line = -1,
                   bool isConst = false, bool isRef = false);

  Symbol newSymbol(const std::string& name, Type* type,
                   int line = -1, bool isConst = false, bool isRef = false);

  void defineStruct(const std::string& name,
                   const SymbolList& symbolList,
                   int line);

  StructType* createAnonymousStruct(const SymbolList& symbolList);

  bool declared(const Symbol& s);

  void declare(const Symbol& symbol);

  void declare(const SymbolList& params, const std::string& proc);

  const Symbol& getSymbol(const std::string& lexeme);


  static SymbolTable* create(const std::string& unit);

  void dump();

private:
  SymbolTable(const std::string& unit);
  ~SymbolTable();

  void initialize();

  std::string                        _scope;
  std::string                        _unit;

  std::map<std::string, SymbolList>  _table;
  TypeList                           _types;
};

#endif
