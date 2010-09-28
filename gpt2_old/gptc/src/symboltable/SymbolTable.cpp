#include "SymbolTable.hpp"

#include "Types.hpp"
#include "PortugolTokenTypes.hpp"
#include "Symbol.hpp"
#include "SymbolTableExceptions.hpp"

#include <iostream>

SymbolTable::SymbolTable(const std::string& unit)
  : _unit(unit), _typeBuilder(new TypeBuilder) {

  setGlobalScope();
}

void SymbolTable::setGlobalScope() {
  _scope = globalScope();
}

bool SymbolTable::isInGlobalScope() {
  return globalScope() == _scope;
}

void SymbolTable::setIgnoreScope() {
  static char r = 1;
  _scope = "internal" + (r++);
}

const std::string SymbolTable::globalScope() {
  return "@global";
}

void SymbolTable::setScope(const Symbol& scopeSymbol) {  
  _scope = scopeSymbol.identifier();
}

const std::string& SymbolTable::currentScope() {
  return _scope;
}

const std::string& SymbolTable::unit() {
  return _unit;
}

void SymbolTable::insertType(const std::string& name,
                             const SymbolList& fields,
                             int line) {
  
  //checando por campos duplicados na estrutura
  SymbolList::const_iterator dup = fields.duplicated();
  if (dup != fields.end()) {
    throw RedeclarationException(*dup);
  }

  //checando por redefinicao da estrutura
  if (_typeBuilder->typeList().find(name) != _typeBuilder->typeList().end()) {
    throw RedefinedTypeException(name);
  }

  _typeBuilder->typeList().push_back(
    new Type(_typeBuilder, name, fields, _unit, line));
}


// void SymbolTable::insertType(const std::string& name,
//                              const SymbolList& symbolList,
//                              int line) {
// 
//   //checando por campos duplicados na estrutura
//   SymbolList::const_iterator dup = symbolList.duplicated();
//   if (dup != symbolList.end()) {
//     throw RedeclarationException(*dup);
//   }
// 
//   //checando por redefinicao da estrutura
//   if (_typeBuilder->typeList().find(name) != _typeBuilder->typeList().end()) {
//     throw RedefinedTypeException(name);
//   }
// 
//   _typeBuilder->typeList().push_back(
//     new Type(_typeBuilder, name,
//              symbolList.toStructFieldList(), _unit, line));
// }

Type* SymbolTable::getType(const std::string& name) {
  TypeList::iterator ret = _typeBuilder->typeList().find(name);

  if (ret == _typeBuilder->typeList().end()) {
    throw UndeclaredTypeException(name);
  }

  return *ret;
}


void SymbolTable::insertSymbol(const Symbol& symbol) {
  if (symbolExists(symbol)) {
    throw RedeclarationException(symbol);
  } else { 
    checkAmbiguity(symbol);
  }
  _table[symbol.scope()].push_back(symbol);
}

void SymbolTable::insertSymbols(const SymbolList& symbols) {
  for (SymbolList::const_iterator it = symbols.begin();
       it != symbols.end();
       ++it) {
    insertSymbol(*it);
  }
}

const Symbol& SymbolTable::getFirstSymbol(const std::string& lexeme,
                                     const std::string& scope) {
  SymbolList::const_iterator it = _table[scope].findFirstByLexeme(lexeme);
  if (it == _table[scope].end()) {
    throw UndeclaredSymbolException(lexeme);
  }
  return (*it);
}

SymbolList SymbolTable::getSymbols(const std::string& lexeme) {
  return _table[globalScope()].findAllByLexeme(lexeme);
}

const Symbol& 
SymbolTable::getSymbol(const std::string& lexeme, const TypeList& params) {
  //deve considerar promocao de tipos
  //    função f(a:real) ...
  //    f(1);                //resolve para a função "f_real"

//checar:
//  f(a : inteiro)
//  f(b : real)
//  f(2); //primeira versão, sempre!


  SymbolList list = _table[globalScope()].findAllByLexeme(lexeme);

  if (list.size() == 0) {
    throw UndeclaredSymbolException(lexeme);
  }

  //try exact version
  for (SymbolList::iterator it = list.begin(); it != list.end(); ++it) {
    if ((*it).type()->isSubprogram() &&
        (*it).type()->paramTypes().equals(params)) {
      return (*it);
    }
  }

  //tentando promocoes...
  for (SymbolList::iterator it = list.begin(); it != list.end(); ++it) {
    if ((*it).type()->isSubprogram() &&
        (*it).type()->paramTypes().isLValueFor(params)) {
      return (*it);
    }
  }

  throw UnmatchedException(lexeme);

//   std::string id = Symbol::buildIdentifier(lexeme, params);
//   SymbolList::const_iterator it = _table[globalScope()].findByIdentifier(id);
//   if (it == _table[globalScope()].end()) {
//     throw UndeclaredSymbolException(id);
//   }
//   return (*it);
}

const Symbol& SymbolTable::getFirstSymbol(const std::string& lexeme) {
  //buscar primeiro no escopo atual, depois no global

  if (isInGlobalScope()) {
    return getFirstSymbol(lexeme, _scope);
  } else {
    try {
      return getFirstSymbol(lexeme, _scope);
    } catch( ... ) {
      return getFirstSymbol(lexeme, globalScope());
    }
  }
}


bool SymbolTable::symbolExists(const Symbol& s) {
  SymbolList::const_iterator it = 
    _table[s.scope()].findByIdentifier(s.identifier());

  if (it == _table[s.scope()].end()) {
    return false;
  } else {
    return true;
  }
}

void SymbolTable::checkAmbiguity(const Symbol& s) {
//  f(a : inteiro)
//  f(b : inteiro, ... resto)
//  -> declaracao ambigua

  if (!s.type()->isSubprogram()) {
    return;
  }

  SymbolList list = 
    _table[globalScope()].findAllByLexeme(s.lexeme());

  if (list.size() == 0) {
    return;
  }

  for (SymbolList::iterator it = list.begin(); it != list.end(); ++it) {
    if (!(*it).type()->isSubprogram()) {
      continue;
    }

    int size = (*it).type()->paramTypes().size();
    if ((size + 1) == s.type()->paramTypes().size()) {
      Type *sss = s.type()->paramTypes().back();
      bool bn = sss->isReticences();
      if (s.type()->paramTypes().back()->isReticences()) {
        throw AmbiguousDeclarationException(s, *it);
      }
    } else if ((size - 1) == s.type()->paramTypes().size()) {
      if ((*it).type()->paramTypes().back()->isReticences()) {
        throw AmbiguousDeclarationException(s, *it);
      }
    }
  }
}


// bool SymbolTable::symbolExists(const std::string& id) {
//   return symbolExists(id, _scope);
// }
// 
// bool SymbolTable::symbolExists(const std::string& id,
//                                const std::string& scope) {
// 
//   return _table[scope].findByIdentifier(id) != _table[scope].end();
// }


void SymbolTable::dump() {

  std::cerr << "=== SymbolTable ===\n";
  std::cerr << "Identifiers ...\n";
  std::map<std::string, SymbolList>::iterator it;
  for (it = _table.begin(); it != _table.end(); ++it) {
    std::cerr << it->second.toString() << std::endl;
  }
  std::cerr << "Types ... " << std::endl;
  std::cerr << _typeBuilder->typeList().toString() << std::endl;
  std::cerr << "=== End SymbolTable ===\n\n";
}

TypeBuilder* SymbolTable::typeBuilder() {
  return _typeBuilder;
}
