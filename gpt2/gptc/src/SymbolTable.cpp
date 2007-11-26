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
  TypeList::iterator ret = _types.find(name);

  if (ret == _types.end()) {
    throw UndeclaredTypeException(name);
  }

  return *ret;
}

Type* SymbolTable::getType(int id) {
  TypeList::iterator ret = _types.find(id);

  if (ret == _types.end()) {
    throw UndeclaredTypeException(id);
  }

  return *ret;
}

Type* SymbolTable::retrieveMatrixType(Type *ofType, int dimensions) {
  TypeList::iterator it = _types.find(ofType, dimensions);
  Type* type;
  if (it == _types.end()) {
    type = new MatrixType(ofType, dimensions);
    _types.push_back(type);
  } else {
    type = *it;
  }
  return type;
}

Symbol SymbolTable::newSymbol(const std::string& name, Type* type,
                const std::string& scope, int line,
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

  if (_types.find(name) != _types.end()) {
    throw RedefinedTypeException(name);
  }
  _types.push_back(new StructType(name,
      symbolList.toStructFieldList(), _unit, line));
}

StructType* SymbolTable::createAnonymousStruct(const SymbolList& symbolList) {
  StructType* ret = new StructType(symbolList.toStructFieldList());
  _types.push_back(ret);
  return ret;
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
                          const std::string& proc) {
  try {
    setScope(proc);
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

  std::cerr << "=== SymbolTable ===\n";
  std::cerr << "Identifiers ...\n";
  std::map<std::string, SymbolList>::iterator it;
  for (it = _table.begin(); it != _table.end(); ++it) {
    std::cerr << it->second.toString() << std::endl;
  }
  std::cerr << "Types ... " << std::endl;
  std::cerr << _types.toString() << std::endl;
  std::cerr << "=== End SymbolTable ===\n\n";
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
