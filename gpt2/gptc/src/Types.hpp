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

  virtual bool compatible(Type*) = 0;
  virtual bool _compatible(PrimitiveType*) = 0;
  virtual bool _compatible(MatrixType*) = 0;
  virtual bool _compatible(StructType*) = 0;
  virtual bool _compatible(SubprogramType*) = 0;

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

  virtual bool _isRValueFor(PrimitiveType*) = 0;
  virtual bool _isRValueFor(MatrixType*) = 0;
  virtual bool _isRValueFor(StructType*) = 0;
  virtual bool _isRValueFor(SubprogramType*) = 0;
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

  virtual bool compatible(Type*);
  virtual bool _compatible(PrimitiveType*);
  virtual bool _compatible(MatrixType*);
  virtual bool _compatible(StructType*);
  virtual bool _compatible(SubprogramType*);

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
  virtual bool _isRValueFor(PrimitiveType*);
  virtual bool _isRValueFor(MatrixType*);
  virtual bool _isRValueFor(StructType*);
  virtual bool _isRValueFor(SubprogramType*);
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

  virtual bool compatible(Type*);
  virtual bool _compatible(PrimitiveType*);
  virtual bool _compatible(MatrixType*);
  virtual bool _compatible(StructType*);
  virtual bool _compatible(SubprogramType*);

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
  virtual bool _isRValueFor(PrimitiveType*);
  virtual bool _isRValueFor(MatrixType*);
  virtual bool _isRValueFor(StructType*);
  virtual bool _isRValueFor(SubprogramType*);
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

    bool isLValueFor(const Field& other) const {
      return name == other.name && type->isLValueFor(other.type);
    }

    bool compatible(const Field& other) const {
      return name == other.name && type->compatible(other.type);
    }

    bool operator==(const Field& other) const {
      return name == other.name && type->equals(other.type);
    }

    std::string name;
    Type* type;
  };

  class FieldList : public std::list<Field> {
    public:

    bool compatible(const FieldList& other) const {
      if (size() != other.size()) {
        return false;
      }
      const_iterator it, jt;
      bool found;
      for (it = begin(); it != end(); ++it) {
        found = false;
        for (jt = other.begin(); jt != other.end(); ++jt) {
          if ((*it).compatible(*jt)) {
            found = true;
          }
        }
        if (!found) {
          return false;
        }
      }
      return true;
    }

    bool isLValueFor(const FieldList& other) const {
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

    bool operator==(const FieldList& other) const {
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
  };

  StructType(const std::string& name, const FieldList& fields,
             const std::string& unit, int line);

  StructType(const FieldList& fields);

  const FieldList& fields() const;

  virtual std::string name() const;

  virtual bool compatible(Type*);
  virtual bool _compatible(PrimitiveType*);
  virtual bool _compatible(MatrixType*);
  virtual bool _compatible(StructType*);
  virtual bool _compatible(SubprogramType*);

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
  virtual bool _isRValueFor(PrimitiveType*);
  virtual bool _isRValueFor(MatrixType*);
  virtual bool _isRValueFor(StructType*);
  virtual bool _isRValueFor(SubprogramType*);
private:
  bool             _anonymous;
  std::string      _name;
  FieldList        _fields;
  std::string      _unit;
  int              _line;
};


class SubprogramType : public Type {
public:
  SubprogramType(const TypeList& paramTypes, Type* returnType);

  virtual std::string name() const;

  virtual bool compatible(Type*);
  virtual bool _compatible(PrimitiveType*);
  virtual bool _compatible(MatrixType*);
  virtual bool _compatible(StructType*);
  virtual bool _compatible(SubprogramType*);

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
  virtual bool _isRValueFor(PrimitiveType*);
  virtual bool _isRValueFor(MatrixType*);
  virtual bool _isRValueFor(StructType*);
  virtual bool _isRValueFor(SubprogramType*);
private:
  TypeList         _paramTypes;
  Type*            _returnType;
};


#endif
