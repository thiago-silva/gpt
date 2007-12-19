#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <list>
#include <map>

#include "Types.hpp"

class Symbol;
class SymbolList;
class TypeBuilder;

class SymbolTable {
public:
  SymbolTable(const std::string& unit);

  void setGlobalScope();
  bool isInGlobalScope();
  void setIgnoreScope();
  void setScope(const Symbol&);
  const std::string& currentScope();
  const std::string globalScope();

  const std::string& unit();

  void insertType(const std::string& name,
                  const SymbolList&,
                  int line);

  /* recuperar tipo por nome (ex. esruturas) */
  Type* getType(const std::string& name);

  void insertSymbol(const Symbol& symbol);
  void insertSymbols(const SymbolList& symbols);

  //(lexeme, scope)
  const Symbol& getFirstSymbol(const std::string&, const std::string&);

  //(lexeme, params)
  const Symbol& getSymbol(const std::string&, const TypeList&);

  //(lexeme) escopo global
  SymbolList getSymbols(const std::string&);

  //(lexeme) -> todos os escopos, iniciando pelo atual
  const Symbol& getFirstSymbol(const std::string&);

  void dump();

  TypeBuilder* typeBuilder();
private:
/*  bool symbolExists(const std::string&);
  bool symbolExists(const std::string&, const std::string&);*/
  bool symbolExists(const Symbol& s);
  void checkAmbiguity(const Symbol& s);

  std::string                        _unit;
  std::string                        _scope;

  TypeBuilder                        *_typeBuilder;
  std::map<std::string, SymbolList>  _table;
};

#endif
