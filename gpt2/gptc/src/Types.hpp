#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <list>

class PrimitiveType;
class MatrixType;
class StructType;
class SubprogramType;


class Type {
public:
  virtual ~Type();
  virtual std::string name() const = 0;

  virtual bool equals(const Type*) const = 0;
  virtual bool equals(int id) const = 0;
  virtual bool equals(Type* ofType, int dimensions) const = 0;

  virtual bool _equals(const PrimitiveType*) const = 0;
  virtual bool _equals(const MatrixType*) const = 0;
  virtual bool _equals(const StructType*) const = 0;
  virtual bool _equals(const SubprogramType*) const = 0;

  virtual bool  isPrimitive() const = 0;

  virtual Type* numPromotionWith(Type*) = 0;
  virtual Type* litPromotionWith(Type*) = 0;

  virtual bool isLValueFor(Type*) = 0;

  virtual bool isRValueFor(PrimitiveType*) = 0;
  virtual bool isRValueFor(MatrixType*) = 0;
  virtual bool isRValueFor(StructType*) = 0;
  virtual bool isRValueFor(SubprogramType*) = 0;
};


class TypeList : public std::list<Type*> {
public:

  iterator find(const std::string& lexeme);

  iterator find(Type* ofType, int dimensions);

  iterator find(int id);

//   void push_back(const TypeList&);

  std::string toString() const;
};


class PrimitiveType : public Type {
public:
  PrimitiveType(int id, const std::string& name);

  int id();
  virtual std::string name() const;

  virtual bool equals(const Type*) const;
  virtual bool equals(int id) const;
  virtual bool equals(Type* ofType, int dimensions) const;

  virtual bool _equals(const PrimitiveType*) const;
  virtual bool _equals(const MatrixType*) const;
  virtual bool _equals(const StructType*) const;
  virtual bool _equals(const SubprogramType*) const;

  virtual bool isPrimitive() const;
  virtual Type* numPromotionWith(Type*);
  virtual Type* litPromotionWith(Type*);

  virtual bool isLValueFor(Type*);
  virtual bool isRValueFor(PrimitiveType*);
  virtual bool isRValueFor(MatrixType*);
  virtual bool isRValueFor(StructType*);
  virtual bool isRValueFor(SubprogramType*);
private:
  int         _id;
  std::string _name;
};




class MatrixType : public Type {
public:
  MatrixType(Type* ofType, int dimensions);

  const Type*         ofType() const;
  int                 dimensions() const;

  virtual std::string name() const;

  virtual bool equals(const Type*) const;
  virtual bool equals(int id) const;
  virtual bool equals(Type* ofType, int dimensions) const;

  virtual bool _equals(const PrimitiveType*) const;
  virtual bool _equals(const MatrixType*) const;
  virtual bool _equals(const StructType*) const;
  virtual bool _equals(const SubprogramType*) const;

  virtual bool isPrimitive() const;
  virtual Type* numPromotionWith(Type*);
  virtual Type* litPromotionWith(Type*);

  virtual bool isLValueFor(Type*);
  virtual bool isRValueFor(PrimitiveType*);
  virtual bool isRValueFor(MatrixType*);
  virtual bool isRValueFor(StructType*);
  virtual bool isRValueFor(SubprogramType*);
private:
  Type          *_ofType;
  int           _dimensions;
};


class StructType : public Type {
public:
  class Field {
    public:
    Field(const std::string& n, Type* t)
      : name(n), type(t) {}

    std::string name;
    Type* type;
  };

  StructType(const std::string& name, const std::list<Field>& fields,
             const std::string& unit, int line);

  StructType(const std::list<Field>& fields);

  const std::list<Field>& fields() const;

  bool equivalent(Type*);

  virtual std::string name() const;

  virtual bool equals(const Type*) const;
  virtual bool equals(int id) const;
  virtual bool equals(Type* ofType, int dimensions) const;
  //virtual bool equals(list<Field> fields)
  virtual bool _equals(const PrimitiveType*) const;
  virtual bool _equals(const MatrixType*) const;
  virtual bool _equals(const StructType*) const;
  virtual bool _equals(const SubprogramType*) const;

  virtual bool isPrimitive() const;
  virtual Type* numPromotionWith(Type*);
  virtual Type* litPromotionWith(Type*);

  virtual bool isLValueFor(Type*);
  virtual bool isRValueFor(PrimitiveType*);
  virtual bool isRValueFor(MatrixType*);
  virtual bool isRValueFor(StructType*);
  virtual bool isRValueFor(SubprogramType*);
private:
  std::string      _name;
  std::list<Field> _fields;
  std::string      _unit;
  int              _line;
};


class SubprogramType : public Type {
public:
  SubprogramType(const TypeList& paramTypes, Type* returnType);

  virtual std::string name() const;

  virtual bool equals(const Type*) const;
  virtual bool equals(int id) const;
  virtual bool equals(Type* ofType, int dimensions) const;

  virtual bool _equals(const PrimitiveType*) const;
  virtual bool _equals(const MatrixType*) const;
  virtual bool _equals(const StructType*) const;
  virtual bool _equals(const SubprogramType*) const;

  virtual bool isPrimitive() const;
  virtual Type* numPromotionWith(Type*);
  virtual Type* litPromotionWith(Type*);

  virtual bool isLValueFor(Type*);
  virtual bool isRValueFor(PrimitiveType*);
  virtual bool isRValueFor(MatrixType*);
  virtual bool isRValueFor(StructType*);
  virtual bool isRValueFor(SubprogramType*);
private:
  TypeList         _paramTypes;
  Type*            _returnType;
};


#endif
