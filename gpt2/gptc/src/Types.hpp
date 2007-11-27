#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <list>

#include "PortugolTokenTypes.hpp"

class Type;

class TypeList : public std::list<Type*> {
public:

  iterator find(const std::string& lexeme);

  iterator find(Type* ofType, int dimensions);

  iterator find(int id);

//   void push_back(const TypeList&);

  std::string toString() const;
};


class Type : private PortugolTokenTypes {
public:
  enum { PRIMITIVE, MATRIX, STRUCT, SUBPROGRAM };

  class StructField {
  public:
    StructField(const std::string& n, Type* t);
    bool isLValueFor(const StructField& other) const;

//     bool compatible(const StructField& other) const;

    bool operator==(const StructField& other) const;

  private:
    std::string name;
    Type* type;
  };

  class StructFieldList : public std::list<StructField> {
  public:
//     bool compatible(const StructFieldList& other) const;
    bool isLValueFor(const StructFieldList& other) const;
    bool operator==(const StructFieldList& other) const;
  };


  /* Constructors */

  //Primitive
  Type(int id, const std::string& name);
  int primitiveType() const;

  //Matrix
  Type(Type* ofType, int dimensions);
  const Type*         ofType() const;
  int                 dimensions() const;

  //Struct
  Type(const std::string& name, const StructFieldList& fields,
             const std::string& unit, int line);
  Type(const StructFieldList& fields);
  const StructFieldList& fields() const;

  //Subprogram
  Type(const TypeList& paramTypes, Type* returnType);

  /* General Methods */

  bool isPrimitive() const;
  bool isMatrix() const;
  bool isStruct() const;
  bool isSubprogram() const;

  const std::string name() const;

  bool equals(int id) const;
  bool equals(Type* ofType, int dimensions) const;



  //virtual
//   bool  compatible(Type* other);
  bool  equals(const Type* other) const;
  Type* numPromotionWith(Type* other);
  Type* litPromotionWith(Type* other);
  bool  isLValueFor(Type* rtype);


  //impl primitive
//   bool  primitive_compatible(Type* other);
  bool  primitive_equals(const Type* other) const;
  Type* primitive_numPromotionWith(Type* other);
  Type* primitive_litPromotionWith(Type* other);
  bool  primitive_isLValueFor(Type* rvalue);

  //impl matrix
//   bool matrix_compatible(Type* other);
  bool matrix_equals(const Type* other) const;
  bool matrix_isLValueFor(Type* rvalue);

  //impl struct
//   bool struct_compatible(Type* other);
  bool struct_equals(const Type* other) const;
  bool struct_isLValueFor(Type* rtype);

  //impl subprogram
//   bool subprogram_compatible(Type* other);
  bool subprogram_equals(const Type* other) const;

private:
  int         _typeId;

  std::string _name;

  //Primitive
  int         _id;

  //matrix
  Type          *_ofType;
  int           _dimensions;

  //struct
  bool             _anonymous;
  StructFieldList  _fields;
  std::string      _unit;
  int              _line;

  //Subprogram
  TypeList         _paramTypes;
  Type*            _returnType;
};

#endif
