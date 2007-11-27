#include "Types.hpp"


Type::StructField::StructField(const std::string& n, Type* t)
      : name(n), type(t) {}

bool
Type::StructField::isLValueFor(const Type::StructField& other) const {
  return name == other.name && type->isLValueFor(other.type);
}

// bool
// Type::StructField::compatible(const Type::StructField& other) const {
//   return name == other.name && type->compatible(other.type);
// }

bool
Type::StructField::operator==(const Type::StructField& other) const {
  return name == other.name && type->equals(other.type);
}

// bool
// Type::StructFieldList::compatible(const Type::StructFieldList& other) const {
//   if (size() != other.size()) {
//     return false;
//   }
//   const_iterator it, jt;
//   bool found;
//   for (it = begin(); it != end(); ++it) {
//     found = false;
//     for (jt = other.begin(); jt != other.end(); ++jt) {
//       if ((*it).compatible(*jt)) {
//         found = true;
//       }
//     }
//     if (!found) {
//       return false;
//     }
//   }
//   return true;
// }

bool
Type::StructFieldList::isLValueFor(const Type::StructFieldList& other) const {
  if (size() != other.size()) {
    return false;
  }
  const_iterator it, jt;
  bool found;
  for (it = begin(); it != end(); ++it) {
    found = false;
    for (jt = other.begin(); jt != other.end(); ++jt) {
      if ((*it).isLValueFor(*jt)) {
        found = true;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}

bool
Type::StructFieldList::operator==(const Type::StructFieldList& other) const {
  if (size() != other.size()) {
    return false;
  }
  const_iterator it, jt;
  bool found;
  for (it = begin(); it != end(); ++it) {
    found = false;
    for (jt = other.begin(); jt != other.end(); ++jt) {
      if ((*it) == (*jt)) {
        found = true;
      }
    }
    if (!found) {
      return false;
    }
  }
  return true;
}



//************* GENERAL *********************************/

bool Type::isPrimitive() const {
  return _typeId == PRIMITIVE;
}

bool Type::isMatrix() const {
  return _typeId == MATRIX;
}

bool Type::isStruct() const {
  return _typeId == STRUCT;
}

bool Type::isSubprogram() const {
  return _typeId == SUBPROGRAM;
}

const std::string Type::name() const {
  return _name;
}

bool Type::equals(int id) const {
  return isPrimitive() && (_id == id);
}

bool Type::equals(Type* ofType, int dimensions) const {
  return isMatrix() && (dimensions == _dimensions) &&
          _ofType->equals(ofType);
}

/************* VIRTUAL *********************************/

// bool Type::compatible(Type* other) {
//   switch (_typeId) {
//     case PRIMITIVE:
//       return primitive_compatible(other);
//     case MATRIX:
//       return matrix_compatible(other);
//     case STRUCT:
//       return struct_compatible(other);
//     case SUBPROGRAM:
//       return subprogram_compatible(other);
//     default:
//       throw;
//   }
// }

bool Type::equals(const Type* other) const {
  switch (_typeId) {
    case PRIMITIVE:
      return primitive_equals(other);
    case MATRIX:
      return matrix_equals(other);
    case STRUCT:
      return struct_equals(other);
    case SUBPROGRAM:
      return subprogram_equals(other);
    default:
      throw;
  }
}

Type* Type::numPromotionWith(Type* other) {
  switch (_typeId) {
    case PRIMITIVE:
      return primitive_numPromotionWith(other);
    case MATRIX:
      return NULL;
    case STRUCT:
      return NULL;
    case SUBPROGRAM:
      return NULL;
    default:
      throw;
  }
}

Type* Type::litPromotionWith(Type* other) {
  switch (_typeId) {
    case PRIMITIVE:
      return primitive_litPromotionWith(other);
    case MATRIX:
      return NULL;
    case STRUCT:
      return NULL;
    case SUBPROGRAM:
      return NULL;
    default:
      throw;
  }
}


bool Type::isLValueFor(Type* rtype) {
  switch (_typeId) {
    case PRIMITIVE:
      return primitive_isLValueFor(rtype);
    case MATRIX:
      return matrix_isLValueFor(rtype);
    case STRUCT:
      return struct_isLValueFor(rtype);
    case SUBPROGRAM:
      return false;
    default:
      throw;
  }
}

//************* PRIMITIVE *********************************/



Type::Type(int id, const std::string& name)
  : _typeId(PRIMITIVE), _name(name), _id(id) {
}

int Type::primitiveType() const {
  return _id;
}

// bool Type::primitive_compatible(Type* other) {
//   if (other->isPrimitive()) {
//     return equals(other) || numPromotionWith(other);
//   } else {
//     return false;
//   }
// }

bool Type::primitive_equals(const Type* other) const {
  return other->isPrimitive() && (other->_id == _id);
}

Type* Type::primitive_numPromotionWith(Type* other) {
  if ((_id == T_INTEIRO)
       &&
      (other->equals(T_INTEIRO) ||
       other->equals(T_REAL))) {
    return other;
  } else if ((_id == T_REAL)
              &&
             (other->equals(T_INTEIRO) ||
              other->equals(T_REAL))) {
    return this;
  }

  return NULL;
}

Type* Type::primitive_litPromotionWith(Type* other) {
  if ((_id == T_LITERAL)
       &&
      (other->equals(T_LITERAL) ||
       other->equals(T_CARACTERE))) {
    return this;
  } else if (other->equals(T_LITERAL)
              &&
             ((_id == T_CARACTERE) ||
              (_id == T_LITERAL))) {
    return other;
  }

  return NULL;
}

bool Type::primitive_isLValueFor(Type* rvalue) {
  if (equals(rvalue)) {
    return true;
  }

  if ((_id == T_REAL) &&
      rvalue->equals(T_INTEIRO)) {
    return true;
  }
  return false;
}

/************* MATRIX *********************************/

Type::Type(Type* type, int dimensions)
  : _typeId(MATRIX), _ofType(type), _dimensions(dimensions) {

  _name = "matriz";
  for (int i = 0; i < _dimensions; i++) {
    _name += "[]";
  }
  _name += " do tipo " + _ofType->name();
}

const Type* Type::ofType() const {
  return _ofType;
}

int Type::dimensions() const {
  return _dimensions;
}

// bool Type::matrix_compatible(Type* other) {
//   if (other->isMatrix()) {
//     return _dimensions == other->_dimensions &&
//           _ofType->compatible(other->_ofType);
//   } else {
//     return false;
//   }
// }

bool Type::matrix_equals(const Type* other) const {
  return other->isMatrix() && equals(other->_ofType, other->_dimensions);
}

bool Type::matrix_isLValueFor(Type* rvalue) {
  if (rvalue->isMatrix() &&
      (rvalue->_dimensions == _dimensions) &&
      rvalue->_ofType->isLValueFor(_ofType)) {
    return true;
  }

  if (rvalue->isPrimitive() && rvalue->equals(T_NULO)) {
    return true;
  }

  return false;
}



/******************** STRUCT *********************************/


Type::Type(const std::string& name, const StructFieldList& fields,
           const std::string& unit, int line)
  : _typeId(STRUCT), _name(name), _anonymous(false), _fields(fields),
    _unit(unit), _line(line) {
}


Type::Type(const StructFieldList& fields)
  : _typeId(STRUCT), _name("<estrutura anônima>"), _anonymous(true),
    _fields(fields), _unit("<interno>"), _line(-1) {
}

const Type::StructFieldList& Type::fields() const {
  return _fields;
}


// bool Type::struct_compatible(Type* other) {
//   return other->isStruct() && _fields.compatible(other->_fields);
// }

bool Type::struct_equals(const Type* other) const {
  if (!other->isStruct()) {
    return false;
  }

  if ((_anonymous  && other->_anonymous) ||
      (!_anonymous && !other->_anonymous)) {
    //comparacao estrutural
    return _fields == other->_fields;
  } else {
    //comparacao nominal
    return _name == other->_name;
  }
}


bool Type::struct_isLValueFor(Type* rtype) {
  if (rtype->isPrimitive()) {
    return rtype->equals(T_NULO);
  }

  if (!rtype->isStruct()) {
    return false;
  }

  if (_anonymous || rtype->_anonymous) {
    return _fields.isLValueFor(rtype->_fields);
  } else {
    //comparacao nominal
    return equals(rtype);
  }
}



/******************** SUBPROGRAM *********************************/

Type::Type(const TypeList& paramTypes,
           Type* returnType)
  : _typeId(SUBPROGRAM), _paramTypes(paramTypes), _returnType(returnType) {

  if (_returnType->equals(T_NULO)) {
    _name = "proc(";
  } else {
    _name = "func(";
  }
  _name += _paramTypes.toString();
  _name += ")";
  _name += " : " + _returnType->name();
}


// bool Type::subprogram_compatible(Type* other) {
//   return equals(other);
// }


bool Type::subprogram_equals(const Type* other) const {
  return other->isSubprogram() &&
         (_paramTypes == other->_paramTypes) &&
         (_returnType->equals(other->_returnType));
}




/******************** TYPELIST *********************************/





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

std::string TypeList::toString() const {
  std::string ret = "";
  const_iterator it;
  for (it = begin(); it != end(); ++it) {
    ret += (*it)->name() + ",";
  }
  return ret;
}

