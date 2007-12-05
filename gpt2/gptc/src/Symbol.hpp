#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <list>

class Type;
class TypeList;

class Symbol {
public:
  static std::string buildIdentifier(const std::string&, Type*);
  static std::string buildIdentifier(const std::string&, const TypeList&);

  Symbol();

  Symbol(const std::string& lexeme,
         Type* type,
         const std::string& scope,
         const std::string& unit,
         int line);

  Symbol(const std::string& lexeme,
         Type* type,
         const std::string& unit,
         int line);

  Symbol(const std::string& lexeme,
         Type* type);

  const std::string& lexeme() const;

  Type* type() const;

  void setScope(const std::string&);
  const std::string& scope() const;

  const std::string& unit() const;

  int line() const;

  std::string toString() const;

  std::string identifier() const;

  bool equivalent(const Symbol&) const;
private:
  std::string      _lexeme;
  std::string      _identifier;
  Type*            _type;
  std::string      _scope;
  std::string      _unit;
  int              _line;
};


class SymbolList : public std::list<Symbol> {
public:

  const_iterator duplicated() const;

  TypeList typeList() const;

  SymbolList     findAllByLexeme(const std::string&) const;
  const_iterator findFirstByLexeme(const std::string&) const;
  const_iterator findByIdentifier(const std::string&) const;
  

  int count(const std::string&) const;

  void setScope(const std::string&);

  std::string toString() const;

  bool equivalent(const SymbolList&) const;
};

#endif
