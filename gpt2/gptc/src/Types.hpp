#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <list>

class Type {
public:
  virtual ~Type();
  virtual std::string name() const = 0;

  virtual bool equals(Type* ofType, int dimensions) const = 0;
  virtual bool equals(int id) const = 0;
};



class TypeList : public std::list<Type*> {
public:

  Type* find(const std::string& lexeme) const;

  Type* find(Type* ofType, int dimensions) const;

  Type* find(int id) const;

  std::string toString() const;
};



class PrimitiveType : public Type {
public:
  PrimitiveType(int id, const std::string& name);

  int id();
  virtual std::string name() const;

  virtual bool equals(Type* ofType, int dimensions) const;
  virtual bool equals(int id) const;

private:
  int         _id;
  std::string _name;
};




class MatrixType : public Type {
public:
  MatrixType(Type* ofType, int dimensions);

  const Type*         ofType();
  int                 dimensions();

  virtual std::string name() const;

  virtual bool equals(Type* ofType, int dimensions) const;
  virtual bool equals(int id) const;
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

  const std::list<Field>& fields();

  virtual std::string name() const;

  virtual bool equals(Type* ofType, int dimensions) const;
  virtual bool equals(int id) const;

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


  virtual bool equals(Type* ofType, int dimensions) const;
  virtual bool equals(int id) const;

private:
  TypeList         _paramTypes;
  Type*            _returnType;
};


#endif
