#include "Types.hpp"
#include "PortugolTokenTypes.hpp"

Type::~Type() {}

//------------------------------------------------------------------


TypeList::iterator TypeList::find(const std::string& lexeme) {
  iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->name() == lexeme) {
      return it;
    }
  }
  return end();
}

TypeList::iterator TypeList::find(Type* ofType, int dimensions) {
  iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->equals(ofType, dimensions)) {
      return it;
    }
  }
  return end();
}

TypeList::iterator TypeList::find(int id) {
  iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->equals(id)) {
      return it;
    }
  }
  return end();
}

// void TypeList::push_back(const TypeList& other) {
//   for (iterator it = other.begin(); it != other.end(); ++it) {
//     push_back(*it);
//   }
// }

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

bool PrimitiveType::equals(const Type* other) const {
  other->_equals(this);
}

bool PrimitiveType::equals(int id) const {
  return _id == id;
}

bool PrimitiveType::equals(Type* ofType, int dimensions) const {
  return false;
}

bool PrimitiveType::_equals(const PrimitiveType* other) const {
  return other->_id == _id;
}

bool PrimitiveType::_equals(const MatrixType*) const {
  return false;
}

bool PrimitiveType::_equals(const StructType*) const {
  return false;
}

bool PrimitiveType::_equals(const SubprogramType*) const {
  return false;
}

bool PrimitiveType::isPrimitive() const {
  return true;
}

Type* PrimitiveType::numPromotionWith(Type* other) {
  if ((_id == PortugolTokenTypes::T_INTEIRO)
       &&
      (other->equals(PortugolTokenTypes::T_INTEIRO) ||
       other->equals(PortugolTokenTypes::T_REAL))) {
    return other;
  } else if ((_id == PortugolTokenTypes::T_REAL)
              &&
             (other->equals(PortugolTokenTypes::T_INTEIRO) ||
              other->equals(PortugolTokenTypes::T_REAL))) {
    return this;
  }

  return NULL;
}

Type* PrimitiveType::litPromotionWith(Type* other) {
  if ((_id == PortugolTokenTypes::T_LITERAL)
       &&
      (other->equals(PortugolTokenTypes::T_LITERAL) ||
       other->equals(PortugolTokenTypes::T_CARACTERE))) {
    return this;
  } else if (other->equals(PortugolTokenTypes::T_LITERAL)
              &&
             ((_id == PortugolTokenTypes::T_CARACTERE) ||
              (_id == PortugolTokenTypes::T_LITERAL))) {
    return other;
  }

  return NULL;
}

bool PrimitiveType::isLValueFor(Type* rtype) {
  rtype->isRValueFor(this);
}

bool PrimitiveType::isRValueFor(PrimitiveType* lvalue) {
  if (lvalue->equals(this)) {
    return true;
  }

  if (lvalue->equals(PortugolTokenTypes::T_REAL) &&
      _id == PortugolTokenTypes::T_INTEIRO) {
    return true;
  }
  return false;
}

bool PrimitiveType::isRValueFor(MatrixType*) {
  return false;
}

bool PrimitiveType::isRValueFor(StructType*) {
  return false;
}

bool PrimitiveType::isRValueFor(SubprogramType*) {
  return false;
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

const Type* MatrixType::ofType() const {
  return _ofType;
}

int MatrixType::dimensions() const {
  return _dimensions;
}

bool MatrixType::equals(const Type* other) const {
  other->_equals(this);
}

bool MatrixType::equals(int id) const {
  return false;
}

bool MatrixType::equals(Type* ofType, int dimensions) const {
  return (ofType == _ofType) && (dimensions == _dimensions);
}

bool MatrixType::_equals(const PrimitiveType*) const {
  return false;
}

bool MatrixType::_equals(const MatrixType* other) const {
  return (other->_ofType == _ofType) && (other->_dimensions == _dimensions);
}

bool MatrixType::_equals(const StructType*) const {
  return false;
}

bool MatrixType::_equals(const SubprogramType*) const {
  return false;
}

bool MatrixType::isPrimitive() const {
  return false;
}

Type* MatrixType::numPromotionWith(Type*) {
  return NULL;
}

Type* MatrixType::litPromotionWith(Type*) {
  return NULL;
}

bool MatrixType::isLValueFor(Type* rtype) {
  rtype->isRValueFor(this);
}

bool MatrixType::isRValueFor(PrimitiveType*) {
  return false;
}

bool MatrixType::isRValueFor(MatrixType* lvalue) {
  if (_equals(lvalue)) {
    return true;
  }

  if ((lvalue->dimensions() == dimensions()) &&
      lvalue->ofType()->equals(PortugolTokenTypes::T_REAL) &&
      _ofType->equals(PortugolTokenTypes::T_INTEIRO)) {
    return true;
  }

  return false;
}

bool MatrixType::isRValueFor(StructType*) {
  return false;
}

bool MatrixType::isRValueFor(SubprogramType*) {
  return false;
}

//------------------------------------------------------------------


StructType::StructType(const std::string& name, const std::list<Field>& fields,
                       const std::string& unit, int line)
  : _name(name), _fields(fields), _unit(unit), _line(line) {
}


StructType::StructType(const std::list<Field>& fields)
  : _name("<anonymous>"), _fields(fields), _unit("<intern>"), _line(-1) {
}

const std::list<StructType::Field>& StructType::fields() const {
  return _fields;
}

std::string StructType::name() const {
  return _name;
}

bool StructType::equivalent(Type* other) {

}

bool StructType::equals(const Type* other) const {
  other->_equals(this);
}

bool StructType::equals(int id) const {
  return false;
}

bool StructType::equals(Type* ofType, int dimensions) const {
  return false;
}

bool StructType::_equals(const PrimitiveType*) const {
  return false;
}

bool StructType::_equals(const MatrixType*) const {
  return false;
}

bool StructType::_equals(const StructType* other) const {
  return _name == other->_name;
}

bool StructType::_equals(const SubprogramType*) const {
  return false;
}

bool StructType::isPrimitive() const {
  return false;
}

Type* StructType::numPromotionWith(Type*) {
  return NULL;
}

Type* StructType::litPromotionWith(Type*) {
  return NULL;
}

bool StructType::isLValueFor(Type* rtype) {
  rtype->isRValueFor(this);
}

bool StructType::isRValueFor(PrimitiveType*) {
  return false;
}

bool StructType::isRValueFor(MatrixType*) {
  return false;
}

bool StructType::isRValueFor(StructType* other) {
  return _equals(other);
}

bool StructType::isRValueFor(SubprogramType*) {
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

bool SubprogramType::equals(const Type* other) const {
  other->_equals(this);
}

bool SubprogramType::equals(int id) const {
  return false;
}

bool SubprogramType::equals(Type*, int) const {
  return false;
}

bool SubprogramType::_equals(const PrimitiveType*) const {
  return false;
}

bool SubprogramType::_equals(const MatrixType*) const {
  return false;
}

bool SubprogramType::_equals(const StructType*) const {
  return false;
}

bool SubprogramType::_equals(const SubprogramType* other) const {
  return (_paramTypes == other->_paramTypes) &&
         (_returnType == other->_returnType);
}

bool SubprogramType::isPrimitive() const {
  return false;
}

Type* SubprogramType::numPromotionWith(Type*) {
  return NULL;
}

Type* SubprogramType::litPromotionWith(Type*) {
  return NULL;
}

bool SubprogramType::isLValueFor(Type*) {
  return false;
}

bool SubprogramType::isRValueFor(PrimitiveType*) {
  return false;
}

bool SubprogramType::isRValueFor(MatrixType*) {
  return false;
}

bool SubprogramType::isRValueFor(StructType*) {
  return false;
}

bool SubprogramType::isRValueFor(SubprogramType*) {
  return false;
}
