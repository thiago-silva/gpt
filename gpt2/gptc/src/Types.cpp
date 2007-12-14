#include "Types.hpp"
#include "TokenLabels.hpp"

#include <iostream>

// Type::StructField::StructField(const std::string& n, Type* t)
//       : _name(n), _type(t) {
// }

// bool
// Type::StructField::isLValueFor(const Type::StructField& other) const {
//   return _name == other._name && _type->isLValueFor(other._type);
// }

// bool
// Type::StructField::operator==(const Type::StructField& other) const {
//   return _name == other._name && _type->equals(other._type);
// }
//
// Type* Type::StructField::type() const {
//   return _type;
// }
//
// const std::string& Type::StructField::name() const {
//   return _name;
// }
//
//
// bool
// Type::StructFieldList::isLValueFor(const Type::StructFieldList& other) const {
//   if (size() != other.size()) {
//     return false;
//   }
//   const_iterator it, jt;
//   bool found;
//   for (it = begin(); it != end(); ++it) {
//     found = false;
//     for (jt = other.begin(); jt != other.end(); ++jt) {
//       if ((*it).isLValueFor(*jt)) {
//         found = true;
//       }
//     }
//     if (!found) {
//       return false;
//     }
//   }
//   return true;
// }
//
// bool
// Type::StructFieldList::operator==(const Type::StructFieldList& other) const {
//   if (size() != other.size()) {
//     return false;
//   }
//   const_iterator it, jt;
//   bool found;
//   for (it = begin(); it != end(); ++it) {
//     found = false;
//     for (jt = other.begin(); jt != other.end(); ++jt) {
//       if ((*it) == (*jt)) {
//         found = true;
//       }
//     }
//     if (!found) {
//       return false;
//     }
//   }
//   return true;
// }
//
//
// Type::StructFieldList::const_iterator
// Type::StructFieldList::find(const std::string& name) const {
//   const_iterator it;
//   for (it = begin(); it != end(); ++it) {
//     if ((*it).name() == name) {
//       return it;
//     }
//   }
//   return end();
// }
//
// std::string Type::StructFieldList::toString() const {
//   std::string ret = "{";
//   std::string v = "";
//   const_iterator it;
//   for (it = begin(); it != end(); ++it) {
//     ret += v + (*it).name() + ":" + (*it).type()->name();
//     v = ", ";
//   }
//   ret += "}";
//   return ret;
// }

//************* GENERAL *********************************/

void Type::setConst(bool c) {
  _isConst = c;
}

void Type::setRef(bool c) {
  _isRef = c;
}

bool Type::isConst() const {
  return _isConst;
}

bool Type::isRef()   const {
  return _isRef;
}

bool Type::isError() const {
  if (_typeId == ERROR) {
    return true;
  }

  if (isMatrix() && _ofType->isError()) {
    return true;
  }

  if (isStruct() && _fields.typeList().hasErrorType()) {
    return true;
  }

  if (isSubprogram() &&
     (_returnType->isError() || _paramTypes.hasErrorType())) {
    return true;
  }
  return false;
}

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

const std::string& Type::name() const {
  return _name;
}

std::string Type::asmName() const {
  switch (_typeId) {
    case PRIMITIVE:
      switch (_id) {
        case T_INTEIRO:
          return "int";
        case T_REAL:
          return "real";
        case T_LITERAL:
          return "string";
        case T_CARACTERE:
          return "char";
        case T_LOGICO:
          return "bool";
        case T_CORINGA:
          return "pointer";
      }
      throw;
    case MATRIX:
      return "matrix";
    case STRUCT:
      return "data";
    default:
      throw;
  }
}

const std::string& Type::identifier() const {
  return _identifier;
}

bool Type::equals(int id) const {
  return isPrimitive() && (_id == id);
}

bool Type::equals(Type* ofType, int dimensions) const {
  return isMatrix() && (dimensions == (int) _dimensions.size()) &&
          _ofType->equals(ofType);
}

/************* VIRTUAL *********************************/

Type* Type::attrPromotion(Type* other) {
  switch (_typeId) {
    case PRIMITIVE:
      return primitive_attrPromotion(other);
    case MATRIX:
      return matrix_attrPromotion(other);
    case STRUCT:
      return struct_attrPromotion(other);
    case SUBPROGRAM:
      return NULL;
    default:
      throw;
  }
}

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



//************* ERROR *********************************/


Type::Type(TypeBuilder* builder)
  : _typeId(ERROR), _builder(builder), _isConst(false), _isRef(false),
    _name("erro"), _identifier(_name) {
}

//************* PRIMITIVE *********************************/



Type::Type(TypeBuilder* builder, int id, const std::string& name)
  : _typeId(PRIMITIVE), _builder(builder), _isConst(false), _isRef(false),
    _name(name), _identifier(name), _id(id) {
}

int Type::primitiveType() const {
  return _id;
}

Type* Type::intOrReal(Type* other) {
  if (!isPrimitive() || !other->isPrimitive()) {
    return NULL;
  }

  if ((_id == T_INTEIRO)
       &&
      (other->equals(T_INTEIRO) ||
       other->equals(T_REAL))) {
    return _builder->primitiveType(other->primitiveType());
  } else if ((_id == T_REAL)
              &&
             (other->equals(T_INTEIRO) ||
              other->equals(T_REAL))) {
    return this;
  }

  return NULL;
}

Type* Type::caracOrLit(Type* other) {
  if (!isPrimitive() || !other->isPrimitive()) {
    return NULL;
  }

  if ((_id == T_LITERAL)
       &&
      (other->equals(T_LITERAL) ||
       other->equals(T_CARACTERE))) {
    return this;
  } else if (other->equals(T_LITERAL)
              &&
             ((_id == T_CARACTERE) ||
              (_id == T_LITERAL))) {
    return _builder->primitiveType(other->primitiveType());
  }

  return NULL;
}

Type* Type::primitive_attrPromotion(Type* other) {
  if (!other->isPrimitive()) {
    return NULL;
  }

  if (primitive_equals(other)) {
    return other;
  }

  if (equals(T_CORINGA)) {
    return other;
  }

  if (other->equals(T_CORINGA)) {
    return this;
  }

  return intOrReal(other);
}

bool Type::primitive_equals(const Type* other) const {
  return other->isPrimitive() && (other->_id == _id);
}

bool Type::primitive_isLValueFor(Type* rvalue) {
  if (equals(rvalue)) {
    return true;
  }

  if ((_id == T_REAL) &&
      rvalue->equals(T_INTEIRO)) {
    return true;
  }

  if (equals(T_CORINGA) || rvalue->equals(T_CORINGA)) {
    return true;
  }

  return false;
}

/************* MATRIX *********************************/


Type::Type(TypeBuilder* builder, Type* type, const std::list<int>& dimensions)
  : _typeId(MATRIX), _builder(builder), _isConst(false), _isRef(false),
    _ofType(type), _dimensions(dimensions) {

/*
  if (!_ofType->isPrimitive() && _ofType->isStruct()) {
    throw; //ofType soh pode ser do tipo primitivo ou estrutura
  }

  if (_dimensions <= 0) {
    throw;
  }
*/

  _name       = "matriz";
  _identifier = "matriz";

  for (int i = 0; i < (int)_dimensions.size(); i++) {
    _name       += "[]";
    _identifier += "_d";
  }
  _name       += " do tipo " + _ofType->name();
  _identifier += "_tipo_"    + _ofType->identifier();
}

Type* Type::ofType() const {
  return _ofType;
}

const std::list<int>& Type::dimensions() const {
  return _dimensions;
}

Type* Type::evalTypeFromSubscript(int dimensions) {
  if (!isMatrix()) {
    return NULL;
  }

  if (dimensions == 0) {
    return this;
  } else if (dimensions == (int)_dimensions.size()) {
    return _ofType;
  } else {
    return _builder->matrixType(_ofType, _dimensions.size() - dimensions);
  }
}

Type* Type::matrix_attrPromotion(Type* other) {
  if (!other->isMatrix()) {
    return NULL;
  }

  Type *ofProm = _ofType->attrPromotion(other->_ofType);
  if (!ofProm) {
    return NULL;
  } else {
    return _builder->matrixType(ofProm, _dimensions);
  }
}

bool Type::matrix_equals(const Type* other) const {
  return other->isMatrix() &&
         equals(other->_ofType, other->_dimensions.size());
}

bool Type::matrix_isLValueFor(Type* rvalue) {
  if (rvalue->isMatrix() &&
      (rvalue->_dimensions.size() == _dimensions.size()) &&
      _ofType->isLValueFor(rvalue->_ofType)) {
    return true;
  }

  if (rvalue->isPrimitive() && rvalue->equals(T_CORINGA)) {
    return true;
  }
  return false;
}



/******************** STRUCT *********************************/


Type::Type(TypeBuilder* builder,
           const std::string& name,
           const SymbolList& fields,
           const std::string& unit, int line)
  : _typeId(STRUCT), _builder(builder), _isConst(false), _isRef(false),
    _name(name), _identifier(name), _anonymous(false), _fields(fields),
    _unit(unit), _line(line) {

  _name = _identifier + ":{";
  std::string v = "";
  for (SymbolList::const_iterator it = fields.begin(); it != fields.end(); ++it) {
    _name += v + (*it).lexeme() + ":" + (*it).type()->name();
    v = ",";
  }
  _name += "}";
}


Type::Type(TypeBuilder* builder, const SymbolList& fields)
  : _typeId(STRUCT), _builder(builder), _isConst(false), _isRef(false),
    _name("<anônimo>"), _anonymous(true), _fields(fields),
    _unit("<interno>"), _line(-1) {

  _name += ":{";
  std::string v = "";
  for (SymbolList::const_iterator it = fields.begin(); it != fields.end(); ++it) {
    _name += v + (*it).lexeme() + ":" + (*it).type()->name();
    v = ",";
  }
  _name += "}";
}

const SymbolList& Type::fields() const {
  return _fields;
}

Type* Type::struct_attrPromotion(Type* other) {
  if (!other->isStruct()) {
    return NULL;
  }

  SymbolList                 retFieldList;
  Type                       *promoType;
  SymbolList::const_iterator otherFieldIt;
  SymbolList::iterator       it;


  for (it = _fields.begin(); it != _fields.end(); ++it) {
    otherFieldIt = other->fields().findByIdentifier((*it).identifier());
    if (otherFieldIt == other->fields().end()) { //structs diferentes
      return NULL;
    } else {
      promoType = (*it).type()->attrPromotion((*otherFieldIt).type());
      if (!promoType) {
        return NULL;
      } else {
        retFieldList.push_back(Symbol((*it).lexeme(), promoType));
      }
    }
  }

  return _builder->structType(retFieldList);
}

bool Type::struct_equals(const Type* other) const {
  if (!other->isStruct()) {
    return false;
  }

  if ((_anonymous  && other->_anonymous) ||
      (!_anonymous && !other->_anonymous)) {
    //comparacao estrutural
    return _fields.equivalent(other->_fields);
  } else {
    //comparacao nominal
    return _name == other->_name;
  }
}


bool Type::struct_isLValueFor(Type* rtype) {
  if (rtype->isPrimitive() && rtype->equals(T_CORINGA)) {
    return true;
  }

  if (!rtype->isStruct()) {
    return false;
  }

  if (!_anonymous && !rtype->_anonymous) {
    return equals(rtype);
  } else {
    return fieldsIsLValueFor(rtype->_fields);
  }
}

bool Type::fieldsIsLValueFor(const SymbolList& others) {
  if (_fields.size() != others.size()) {
    return false;
  }

  SymbolList::const_iterator it, jt;
  for (it = _fields.begin(); it != _fields.end(); ++it) {
    jt = others.findByIdentifier((*it).identifier());
    if (jt == others.end()) {
      return false;
    } else if (!(*it).type()->isLValueFor((*jt).type())) {
      return false;
    }
  }

  return true;
}

/******************** SUBPROGRAM *********************************/

Type::Type(TypeBuilder* builder,
           const TypeList& paramTypes,
           Type* returnType)
  : _typeId(SUBPROGRAM), _builder(builder), _isConst(false), _isRef(false),
    _paramTypes(paramTypes), _returnType(returnType) {

  if (!_returnType) {
    _name = "proc(";
  } else {
    _name = "func(";
  }

  _name += _paramTypes.toString();
  _name += ")";

  _identifier = _paramTypes.toIdentifier();

  if (_returnType) {
    _name       += " : " + _returnType->name();
  }
}


const TypeList& Type::paramTypes() const {
  return _paramTypes;
}

Type* Type::returnType() const {
  return _returnType;
}

// bool Type::paramMatches(const TypeList& types) const {
//   return _paramTypes.matches(types);
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
    if ((*it)->identifier() == lexeme) {
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

bool TypeList::equals(const TypeList& other) const {
  const_iterator it;
  const_iterator jt;

  if (size() != other.size()) {
    return false;
  }

  for (it = begin(), jt = other.begin();
      (it != end()) && (jt != other.end()); ++it, ++jt) {
    if (!(*it)->equals(*jt)) {
      return false;
    }
  }

  return true;
}

bool TypeList::isLValueFor(const TypeList& other) const {
  const_iterator it;
  const_iterator jt;

  if (size() != other.size()) {
    return false;
  }

  for (it = begin(), jt = other.begin();
      (it != end()) && (jt != other.end()); ++it, ++jt) {
    if (!(*it)->isLValueFor(*jt)) {
      return false;
    }
  }

  return true;
}

bool TypeList::hasErrorType() const {
  const_iterator it;

  for (it = begin(); it != end(); ++it) {
    if ((*it)->isError()) {
      return true;
    }
  }
  return false;
}


std::string TypeList::toString() const {
  std::string ret = "";
  std::string v = "";
  const_iterator it;
  for (it = begin(); it != end(); ++it) {
    ret += v + (*it)->name();
    v = ",";
  }
  return ret;
}

std::string TypeList::toIdentifier() const {
  std::string ret = "";
  const_iterator it;
  for (it = begin(); it != end(); ++it) {
    ret += "_" + (*it)->identifier();
  }
  return ret;
}

/********************************************************************/

Type* TypeBuilder::primitiveType(int id) {
  Type* ret = new Type(this, id, g_tokenLabels[id]);
  _types.push_back(ret);
  return ret;
}

Type* TypeBuilder::matrixType(Type* ofType,
                              const std::list<int>& dimensions) {
  Type* ret = new Type(this, ofType, dimensions);
  _types.push_back(ret);
  return ret;
}

Type* TypeBuilder::matrixType(Type* ofType, int dimensions) {
  std::list<int> dims(dimensions);
  Type* ret = new Type(this, ofType, dims);
  _types.push_back(ret);
  return ret;
}


Type* TypeBuilder::structType(const std::string& name,
                    const SymbolList& fields,
                    const std::string& unit,
                    int line) {

  Type* ret = new Type(this, name, fields, unit, line);
  _types.push_back(ret);
  return ret;
}

Type* TypeBuilder::structType(const SymbolList& fields) {
  Type* ret = new Type(this, fields);
  _types.push_back(ret);
  return ret;
}

/*
  estrutura Parâmetro
    tipo  : literal;
    valor : coringa;
  fim-estrutura

  procedimento p( ... args)

  -> args : matriz[] do tipo Parâmetro
*/
Type* TypeBuilder::reticencesType() {
  SymbolList fields;

  fields.push_back(Symbol("tipo",
    primitiveType(PortugolTokenTypes::T_LITERAL)));

  fields.push_back(Symbol("valor",
    primitiveType(PortugolTokenTypes::T_CORINGA)));

  return matrixType(
    structType("Parâmetro", fields, "<interno>",-1), std::list<int>(1));
}

Type*
TypeBuilder::subprogramType(const TypeList& paramTypes, Type* returnType) {
  Type* ret = new Type(this, paramTypes, returnType);
  _types.push_back(ret);
  return ret;
}

Type* TypeBuilder::errorType() {
  return new Type(this);
}


TypeList& TypeBuilder::typeList() {
  return _types;
}

