#ifndef SYMBOLTABLEEXCEPTIONS_HPP
#define SYMBOLTABLEEXCEPTIONS_HPP

#include "Symbol.hpp"
#include "Types.hpp"

#include <string>


class UndeclaredTypeException {
public:
  UndeclaredTypeException(const std::string& name);
  UndeclaredTypeException(int id);

  std::string typeName() const;

private:
  int _id;
  std::string _name;
};

class RedefinedTypeException {
public:
  RedefinedTypeException(const std::string& name);

  const std::string& typeName() const;
private:
  std::string _name;
};

class UndeclaredSymbolException {
public:
  UndeclaredSymbolException(const std::string& lexeme);

  std::string lexeme() const;

private:
  std::string _lexeme;
};

class RedeclarationException {
public:
  RedeclarationException(const Symbol&);

  const Symbol& symbol() const;
private:
  Symbol _symbol;
};

class UnmatchedException {
public:
  UnmatchedException(const std::string&);

  const std::string& lexeme();
private:
  std::string _lexeme;
};

#endif
