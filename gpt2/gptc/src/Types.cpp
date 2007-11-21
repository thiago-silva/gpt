#include "Types.hpp"
#include "PortugolTokenTypes.hpp"

Type::~Type() {}


//------------------------------------------------------------------


Type* TypeList::find(const std::string& lexeme) const {
  const_iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->name() == lexeme) {
      return *it;
    }
  }
  return 0;
}

Type* TypeList::find(Type* ofType, int dimensions) const {
  const_iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->equals(ofType, dimensions)) {
      return *it;
    }
  }
  return 0;
}

Type* TypeList::find(int id) const {
  const_iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->equals(id)) {
      return *it;
    }
  }
  return 0;
}

std::string TypeList::toString() const {
  std::string ret = "";
  const_iterator it;
  for (it = begin(); it != end(); ++it) {
    ret += (*it)->name() + ",";
  }
  return ret;
}


//------------------------------------------------------------------


PrimitiveType::PrimitiveType(int id, const std::string& name)
  : _id(id), _name(name) {
}

std::string PrimitiveType::name() const {
  return _name;
}

bool PrimitiveType::equals(Type* ofType, int dimensions) const {
  return false;
}

bool PrimitiveType::equals(int id) const {
  return _id == id;
}

//------------------------------------------------------------------


MatrixType::MatrixType(Type* type, int dimensions)
  : _ofType(type), _dimensions(dimensions) {
}

std::string MatrixType::name() const {
  std::string ret;
  ret = "matriz";
  for (int i = 0; i < _dimensions; i++) {
    ret += "[]";
  }
  ret += " do tipo " + _ofType->name();
  return ret;
}

bool MatrixType::equals(Type* ofType, int dimensions) const {
  return (ofType == _ofType) && (dimensions == _dimensions);
}

bool MatrixType::equals(int id) const {
  return false;
}

//------------------------------------------------------------------


StructType::StructType(const std::string& name, const std::list<Field>& fields,
                       const std::string& unit, int line)
  : _name(name), _fields(fields), _unit(unit), _line(line) {
}


const std::list<StructType::Field>& StructType::fields() {
  return _fields;
}

std::string StructType::name() const {
  return _name;
}

bool StructType::equals(Type* ofType, int dimensions) const {
  return false;
}

bool StructType::equals(int id) const {
  return false;
}
//------------------------------------------------------------------


SubprogramType::SubprogramType(const TypeList& paramTypes,
                               Type* returnType)
  : _paramTypes(paramTypes), _returnType(returnType) {
}

std::string SubprogramType::name() const {
  std::string ret;
  if (_returnType->equals(PortugolTokenTypes::T_NULO)) {
    ret = "proc(";
  } else {
    ret = "func(";
  }
  ret += _paramTypes.toString();
  ret += ")";
  ret += " : " + _returnType->name();
  return ret;
}

bool SubprogramType::equals(Type* ofType, int dimensions) const {
  return false;
}

bool SubprogramType::equals(int id) const {
  return false;
}
