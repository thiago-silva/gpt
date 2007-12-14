#ifndef GPTASMEXPRESSION_HPP
#define GPTASMEXPRESSION_HPP

#include "PortugolTokenTypes.hpp"
#include "Types.hpp"

#include <sstream>
#include <string>

class GptAsmExpression {
public:

  void set(Type* type,
           int mm,
           GptAsmExpression* op1,
           GptAsmExpression* op2) {

    _mm   = mm;
    _op1  = op1;
    _op2  = op2;
    _type = type;
  }

  void set(const std::string& value, Type* type) {
    _value = value;
    _type = type;
  }


  void expand(const std::string lvalue, /*const GptAsmSubprogram& ctx*/ int ctx) {
    //TODO: check for type promotion

    if (isAtom()) {
      return;
    }

    std::stringstream code;

    if (_op1->isAtom() && _op2->isAtom()) {
      std::string cp1 = expandCast(lvalue, _op1->value(), _op1->type(), ctx);
      std::string cp2 = expandCast(lvalue, _op2->value(), _op2->type(), ctx);
//       ctx.emit(_mm, lvalue, cp1, cp2);

    } else if (_op1->isAtom()) {
      std::string t2/* = ctx.declareTmp(_op2->type())*/;

      _op2->expand(t2, ctx);

      std::string cp1 = expandCast(lvalue, _op1->value(), _op1->type(), ctx);
      std::string cp2 = expandCast(lvalue, t2, _op2->type(), ctx);

//       ctx.emit(_mm, lvalue, cp1, cp2);

    } else if (_op2->isAtom()) {
      std::string t1/* = ctx.declareTmp(_op1->type())*/;

      _op1->expand(t1, ctx);

      std::string cp1 = expandCast(lvalue, t1, _op1->type(), ctx);
      std::string cp2 = expandCast(lvalue, _op2->value(), _op2->type(), ctx);

//       ctx.emit(_mm, lvalue, cp1, cp2);

    } else {

      std::string t1/* = ctx.declareTmp(_op1->type())*/;
      std::string t2/* = ctx.declareTmp(_op2->type())*/;

      _op1->expand(t1, ctx);
      _op2->expand(t2, ctx);

      std::string cp1 = expandCast(lvalue, t1, _op1->type(), ctx);
      std::string cp2 = expandCast(lvalue, t2, _op2->type(), ctx);

//       ctx.emit(_mm, lvalue, cp1, cp2);
    }
  }

  bool isAtom() {
    return _value.length() > 0;
  }

  const std::string& value() { return _value; }
  Type*              type()  { return _type; }
private:

  std::string expandCast(const std::string &lvalue,
                         const std::string& opval,
                         Type *optype,
                         /*const GptAsmSubprogram& */ int ctx) {
    if (optype->equals(PortugolTokenTypes::T_CORINGA)) {
      return opval;
    }

    if (_type->equals(optype)) {
      return opval;
    } else {
//       ctx.emit(opcodeCast(optype, _type), lvalue, opval);
      return lvalue;
    }
  }

  Type              *_type;
  int               _mm;
  std::string       _value;
  GptAsmExpression  *_op1;
  GptAsmExpression  *_op2;
};

#endif
