#include "SymbolTableExceptions.hpp"
#include "TokenNames.hpp"

UndeclaredTypeException::UndeclaredTypeException(const std::string& name)
  : _id(-1), _name(name) {
}

UndeclaredTypeException::UndeclaredTypeException(int id)
  : _id(id) {
}

std::string UndeclaredTypeException::typeName() const {
  if (_id == -1) {
    return _name;
  } else {
    return g_tokenNames[_id];
  }
}

//--------------------------------------------------------

RedefinedTypeException::RedefinedTypeException(const std::string& name)
  : _name(name) {
}

const std::string& RedefinedTypeException::typeName() const {
  return _name;
}


//--------------------------------------------------------

UndeclaredSymbolException::UndeclaredSymbolException(const std::string& lexeme)
  : _lexeme(lexeme) {
}

std::string UndeclaredSymbolException::lexeme() const {
  return _lexeme;
}


//--------------------------------------------------------


RedeclarationException::RedeclarationException(const Symbol& s)
  : _symbol(s) {
}

const Symbol& RedeclarationException::symbol() const {
  return _symbol;
}

//--------------------------------------------------------


UnmatchedException::UnmatchedException(const std::string& l)
  : _lexeme(l) {

}

const std::string& UnmatchedException::lexeme() {
  return _lexeme;
}
