#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "Types.hpp"

#include <string>

class Symbol {
public:
  Symbol();

  Symbol(const std::string& lexeme, Type* type, int line,
         const std::string& unit, const std::string& scope,
         bool isConst = false, bool isRef = false);

  const std::string& lexeme() const;

  Type* type() const;

  bool isConst() const;

  bool isRef() const;

  std::string toString() const;

private:
  std::string      _lexeme;
  Type*            _type;
  bool             _isConst;
  bool             _isRef;

  std::string _unit;
  std::string _scope;
  int         _line;
};


class SymbolList : public std::list<Symbol> {
public:
  const_iterator duplicated() const;

  std::list<StructType::Field> toStructFieldList() const;

  TypeList toTypeList() const;

  int count(const std::string& lexeme) const;

  const_iterator find(const std::string& lexeme) const;

  std::string toString() const;
};

#endif
