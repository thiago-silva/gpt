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
  ret += " - ";
  if (_isConst) {
    ret += "c";
  }
  if (_isRef) {
    ret += "r";
  }
  return ret;
}


//-------------------------------------------------------


SymbolList::const_iterator SymbolList::duplicated() const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if (count((*it).lexeme()) > 1) {
      return it;
    }
  }
  return end();
}

Type::StructFieldList SymbolList::toStructFieldList() const {
  Type::StructFieldList fields;

  for (const_iterator it = begin(); it != end(); ++it) {
    fields.push_back(Type::StructField((*it).lexeme(), (*it).type()));
  }
  return fields;
}

TypeList SymbolList::toTypeList() const {
  TypeList list;

  for (const_iterator it = begin(); it != end(); ++it) {
    list.push_back((*it).type());
  }
  return list;
}

int SymbolList::count(const std::string& lexeme) const {
  int ret = 0;
  for (const_iterator it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == lexeme) {
      ret++;
    }
  }
  return ret;
}

SymbolList::iterator SymbolList::find(const std::string& lexeme) {
  for (iterator it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == lexeme) {
      return it;
    }
  }
  return end();
}

std::string SymbolList::toString() const {
  std::string ret = "";
  for (const_iterator it = begin(); it != end(); ++it) {
    ret += (*it).toString() + "\n";
  }
  return ret;
}
