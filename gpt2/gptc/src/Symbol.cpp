#include "Symbol.hpp"
#include "Types.hpp"

#include <sstream>

Symbol::Symbol()
  : _type(0) {
}

Symbol::Symbol(const std::string& lexeme,
               Type* type,
               const std::string& scope,
               const std::string& unit,
               int line)
  :   _lexeme(lexeme),
      _type(type),
      _scope(scope),
      _unit(unit),
      _line(line) {

  _identifier = buildIdentifier(lexeme, _type);

}

Symbol::Symbol(const std::string& lexeme,
               Type* type,
               const std::string& unit,
               int line)
  :   _lexeme(lexeme),
      _type(type),
      _unit(unit),
      _line(line) {

  _identifier = buildIdentifier(lexeme, _type);
}



Symbol::Symbol(const std::string& lexeme, Type* type) 
  : _lexeme(lexeme), _type(type), _scope("?"), _unit("?"), _line(-1) {

  _identifier = buildIdentifier(lexeme, _type);
}


const std::string& Symbol::lexeme() const {
  return _lexeme;
}

Type* Symbol::type() const {
  return _type;
}

void Symbol::setScope(const std::string& scope) {
  _scope = scope;
}

const std::string& Symbol::scope() const {
  return _scope;
}

const std::string& Symbol::unit() const {
  return _unit;
}

int Symbol::line() const {
  return _line;
}

std::string Symbol::toString() const {
  std::stringstream ret;
  ret << _unit << ":" << _line
      << " (" << _scope << ") -> " << lexeme();

  ret << " : " << _type->name();

  ret << " -- attributes: {";
  if (_type->isConst()) {
    ret << "c";
  }

  if (_type->isRef()) {
    ret << "r";
  }
  ret << "}";

  return ret.str();
}

std::string 
Symbol::buildIdentifier(const std::string& name, Type* type) {
  if (!type->isSubprogram()) {
    return name;
  } else {
    return buildIdentifier(name, type->paramTypes());
  }
}

std::string 
Symbol::buildIdentifier(const std::string& name, const TypeList& list) {
  return name + "_" + list.toIdentifier();
}

std::string Symbol::identifier() const {
  return _identifier;
}


bool Symbol::equivalent(const Symbol& other) const {
  return _lexeme == other._lexeme &&
         _type->equals(other._type);
}

//-------------------------------------------------------


SymbolList::const_iterator SymbolList::duplicated() const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if (count((*it).identifier()) > 1) {
      return it;
    }
  }
  return end();
}

int SymbolList::count(const std::string& identifier) const {
  int c = 0;
  for (const_iterator it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == identifier) {
      c++;
    }
  }
  return c;
}

TypeList SymbolList::typeList() const {
  TypeList list;

  for (const_iterator it = begin(); it != end(); ++it) {
    list.push_back((*it).type());
  }
  return list;
}

SymbolList
SymbolList::findAllByLexeme(const std::string& lexeme) const {
  SymbolList ret;
  SymbolList::const_iterator it;
  for (it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == lexeme) {
      ret.push_back(*it);
    }
  }
  return ret;
}

SymbolList::const_iterator 
SymbolList::findFirstByLexeme(const std::string& lexeme) const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if ((*it).lexeme() == lexeme) {
      return it;
    }
  }
  return end();
}

SymbolList::const_iterator 
SymbolList::findByIdentifier(const std::string& identifier) const {
  for (const_iterator it = begin(); it != end(); ++it) {
    if ((*it).identifier() == identifier) {
      return it;
    }
  }
  return end();
}

void SymbolList::setScope(const std::string& scope) {
  for (iterator it = begin(); it != end(); ++it) {
    (*it).setScope(scope);
  }
}

std::string SymbolList::toString() const {
  std::string ret = "";
  for (const_iterator it = begin(); it != end(); ++it) {
    ret += (*it).toString() + "\n";
  }
  return ret;
}

bool SymbolList::equivalent(const SymbolList& other) const {
  const_iterator it, jt;
  for (it = begin(), jt = other.begin(); 
       it != end() && jt != other.end(); 
       ++it, ++jt) {
    if (!(*it).equivalent(*jt)) {
      return false;
    }
  }

  return true;
}
