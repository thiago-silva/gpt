#include "SymbolTable.hpp"

#include "Types.hpp"
#include "PortugolTokenTypes.hpp"
#include "Symbol.hpp"
#include "SymbolTableExceptions.hpp"

#include <iostream>

void SymbolTable::setScope(const std::string& scope) {
  _scope = scope;
}
void SymbolTable::setGlobalScope() {
  _scope = "@global";
}

Type* SymbolTable::getType(const std::string& name) {
  Type* ret = _types.find(name);

  if (ret == 0) {
    throw UndeclaredTypeException(name);
  }

  return ret;
}

Type* SymbolTable::getType(int id) {
  Type* ret = _types.find(id);

  if (ret == 0) {
    throw UndeclaredTypeException(id);
  }

  return ret;
}

Type* SymbolTable::retrieveMatrixType(Type *ofType, int dimensions) {
  Type* t = _types.find(ofType, dimensions);
  if (t == 0) {
    t = new MatrixType(ofType, dimensions);
    _types.push_back(t);
  }
  return t;
}

Symbol SymbolTable::newSymbol(const std::string& name, Type* type,
                int line, const std::string& scope,
                bool isConst, bool isRef) {
  return Symbol(name, type, line, _unit, scope, isConst, isRef);
}

Symbol SymbolTable::newSymbol(const std::string& name, Type* type,
                int line, bool isConst, bool isRef) {
  return Symbol(name, type, line, _unit, _scope, isConst, isRef);
}

void SymbolTable::defineStruct(const std::string& name,
                               const SymbolList& symbolList,
                               int line) {
  SymbolList::const_iterator dup = symbolList.duplicated();
  if (dup != symbolList.end()) {
    throw RedeclarationException(*dup);
  }

  if (_types.find(name) != 0) {
    throw RedefinedTypeException(name);
  }
  _types.push_back(new StructType(name,
      symbolList.toStructFieldList(), _unit, line));
}

bool SymbolTable::declared(const Symbol& s) {
  return _table[_scope].count(s.lexeme()) >= 1;
}

void SymbolTable::declare(const Symbol& symbol) {
  if (declared(symbol)) {
    throw RedeclarationException(symbol);
  }
  _table[_scope].push_back(symbol);
}

void SymbolTable::declare(const SymbolList& params,
                          const std::string& scope) {
  try {
    setScope(scope);
    for (SymbolList::const_iterator it = params.begin(); it != params.end(); ++it) {
      declare(*it);
    }
  } catch( ... ) {
    setGlobalScope();
    throw;
  }
}


const Symbol& SymbolTable::getSymbol(const std::string& lexeme) {
  SymbolList::const_iterator it = _table[_scope].find(lexeme);
  if (it == _table[_scope].end()) {
    throw UndeclaredSymbolException(lexeme);
  }
  return (*it);
}


SymbolTable* SymbolTable::create(const std::string& unit) {
  SymbolTable* s = new SymbolTable(unit);
  s->initialize();
  return s;
}

void SymbolTable::dump() {

  std::cerr << "Ids...\n";
  std::map<std::string, SymbolList>::iterator it;
  for (it = _table.begin(); it != _table.end(); ++it) {
    std::cerr << it->second.toString() << std::endl;
  }
  std::cerr << "Types ... " << std::endl;
  std::cerr << _types.toString() << std::endl;
}


SymbolTable::SymbolTable(const std::string& unit)
  : _unit(unit) {
  setGlobalScope();
}

SymbolTable::~SymbolTable() {
  //TODO: delete tables, types, etc...
}

void SymbolTable::initialize() {
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_INTEIRO,"inteiro"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_REAL,"real"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_CARACTERE,"caractere"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_LITERAL,"literal"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_LOGICO,"lógico"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_NULO,"nulo"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_CORINGA,"coringa"));
  _types.push_back(new PrimitiveType(PortugolTokenTypes::T_RETICENCIAS,"reticências"));
}
