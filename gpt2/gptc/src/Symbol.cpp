#include "Symbol.hpp"

Symbol::Symbol()
  : _type(0) {
}

Symbol::Symbol(const std::string& lexeme, Type* type, int line,
         const std::string& unit, const std::string& scope,
         bool isConst, bool isRef)
    : _lexeme(lexeme), _type(type), _isConst(isConst), _isRef(isRef),
      _unit(unit), _scope(scope), _line(line) {
}

const std::string& Symbol::lexeme() const {
  return _lexeme;
}

Type* Symbol::type() const {
  return _type;
}

bool Symbol::isConst() const {
  return _isConst;
}

bool Symbol::isRef() const {
  return _isRef;
}

std::string Symbol::toString() const {
  std::string ret = _unit + "(" + _scope + ") - " + lexeme() + " : ";
  if (_type) {
    ret += _type->name();
  }
  ret += " {";
  if (_isConst) {
    ret += "c";
  }
  if (_isRef) {
    ret += "r";
  }
  ret += "}";
  return ret;
}


//-------------------------------------------------------


SymbolList::const_iterator SymbolList::duplicated() const {
  for (SymbolList::const_iterator it = begin(); it != end(); ++it) {
    if (count((*it).lexeme()) > 1) {
      return it;
    }
  }
  return end();
}

std::list<StructType::Field> SymbolList::toStructFieldList() const {
  std::list<StructType::Field> fields;

  for (SymbolList::const_iterator it = begin(); it != end(); ++it) {
    fields.push_back(StructType::Field((*it).lexeme(), (*it).type()));
  }
  return fields;
}

TypeList SymbolList::toTypeList() const {
  TypeList list;

  for (SymbolList::const_iterator it = begin(); it != end(); ++it) {
    list.push_back((*it).type());
  }
  return list;
}

int SymbolList::count(const std::string& lexeme) const {
  int ret = 0;
  for (SymbolList::const_iterator it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == lexeme) {
      ret++;
    }
  }
  return ret;
}

SymbolList::const_iterator SymbolList::find(const std::string& lexeme) const {
  for (SymbolList::const_iterator it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == lexeme) {
      return it;
    }
  }
  return end();
}

std::string SymbolList::toString() const {
  std::string ret = "";
  for (SymbolList::const_iterator it = begin(); it != end(); ++it) {
    ret += (*it).toString() + "\n";
  }
  return ret;
}

