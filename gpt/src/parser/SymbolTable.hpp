#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include "Symbol.hpp"

#include <map>
#include <list>
#include <string>

using namespace std;

class SymbolTableException {
public:
  SymbolTableException(const string& msg_) : msg(msg_) {}

  const string& getMessage() { return msg;}
  
  string msg;
};

class SymbolTable{
public:
  static string GlobalScope;//@global

  SymbolTable();
  ~SymbolTable();

  void declareVar(const string& scope, const string& lexeme, int line, int type);

  void declareVar(const string& scope, const string& lexeme, int line, int type
      , const list<int>& dimensions);

  void insertSymbol(Symbol& s, const string& scope);

  Symbol& getSymbol(const string& scope, const string& lexeme, bool searchGlobal = false);

  list<Symbol> getSymbols(const string& scope);

protected:
  void SymbolTable::registrarLeia();
  void SymbolTable::registrarImprima();

  int currentCod;
  map<string,list<Symbol> > symbols;//map<scope, symbols>
};

#endif
