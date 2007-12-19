#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <list>

#include "PortugolTokenTypes.hpp"
#include "Symbol.hpp"

class TypeBuilder;
class Type;

class TypeList : public std::list<Type*> {
public:

  iterator find(const std::string& lexeme);

  iterator find(Type* ofType, int dimensions);

  iterator find(int id);


  bool isLValueFor(const TypeList&) const;
  bool equals(const TypeList&) const;

  bool hasErrorType() const;

  std::string toString() const;
  std::string toIdentifier() const;
};




class Type : private PortugolTokenTypes {
public:
  enum { PRIMITIVE, MATRIX, STRUCT, SUBPROGRAM, ERROR };

  //Error type
  Type(TypeBuilder*);

  //Primitive
  Type(TypeBuilder*, int id, const std::string& name);
  int primitiveType() const;

  Type* intOrReal(Type* other);
  Type* caracOrLit(Type* other);


  //Matrix
  Type(TypeBuilder*, Type* ofType, const std::list<int>& dimensions);

  Type*                    ofType() const;
  const std::list<int>&    dimensions() const;

  Type*                    evalTypeFromSubscript(int);

  //Struct
  Type(TypeBuilder*,
       const std::string& name,
       const SymbolList& fields,
       const std::string& unit, int line);

  Type(TypeBuilder*, const SymbolList& fields);

  const SymbolList& fields() const;
  bool fieldsIsLValueFor(const SymbolList&);

  //Subprogram
  Type(TypeBuilder*, const TypeList& paramTypes, Type* returnType);

  const TypeList& paramTypes() const;
  Type* returnType() const;
//   bool  paramMatches(const TypeList&) const;


  /* General Methods */
  void setConst(bool);
  void setRef(bool);
  bool isConst() const;
  bool isRef()   const;

  bool isError() const;
  bool isPrimitive() const;
  bool isMatrix() const;
  bool isStruct() const;
  bool isSubprogram() const;

  const std::string& name() const;
  std::string asmName(bool = false) const;
  const std::string& identifier() const;

  int byteSize() const;

  bool equals(int id) const;
  bool equals(Type* ofType, int dimensions) const;



  //virtual
  Type* attrPromotion(Type*);
  bool  equals(const Type* other) const;
  bool  isLValueFor(Type* rtype);


  //impl primitive
  Type* primitive_attrPromotion(Type* other);
  bool  primitive_equals(const Type* other) const;
  bool  primitive_isLValueFor(Type* rvalue);

  //impl matrix
  Type* matrix_attrPromotion(Type* other);
  bool matrix_equals(const Type* other) const;
  bool matrix_isLValueFor(Type* rvalue);

  //impl struct
  Type* struct_attrPromotion(Type* other);
  bool struct_equals(const Type* other) const;
  bool struct_isLValueFor(Type* rtype);

  //impl subprogram
  bool subprogram_equals(const Type* other) const;

private:
  int          _typeId;
  TypeBuilder* _builder;

  bool         _isConst;
  bool         _isRef;

  std::string  _name;
  std::string  _identifier;

  //Primitive
  int         _id;

  //matrix
  Type             *_ofType;
  std::list<int>   _dimensions;

  //struct
  bool             _anonymous;
  SymbolList       _fields;
  std::string      _unit;
  int              _line;

  //Subprogram
  TypeList         _paramTypes;
  Type*            _returnType;
};

class TypeBuilder {
public:
  Type* primitiveType(int id);

  Type* matrixType(Type* ofType, const std::list<int>& dimensions);
  Type* matrixType(Type* ofType, int);

  Type* structType(const std::string& name,
                   const SymbolList& fields,
                   const std::string& unit,
                   int line);

  Type* structType(const SymbolList& fields);

  Type* reticencesType();

  Type* subprogramType(const TypeList& paramTypes, Type* returnType);

  Type* errorType();

  TypeList& typeList();
private:
  TypeList  _types;
};

#endif
