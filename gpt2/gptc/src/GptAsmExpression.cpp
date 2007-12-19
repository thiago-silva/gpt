#include "GptAsmExpression.hpp"
#include "PortugolTokenTypes.hpp"
#include "Types.hpp"
#include "Subroutine.hpp"

#include <sstream>
#include <map>

GptAsmExpression::GptAsmExpression(Type* restype,
                                   const std::string& value) 
  : _restype(restype), _exprtype(0), _mm(""), _value(value), _op1(0), _op2(0) {
}

GptAsmExpression::GptAsmExpression(Type* restype, Type *exprtype,
                                   std::string mm, GptAsmExpression* op1)
  : _restype(restype), _exprtype(exprtype),
    _mm(mm), _value(""), _op1(op1), _op2(0) {
}

GptAsmExpression::GptAsmExpression(Type* restype,
                   Type *exprtype,
                   std::string mm,
                   GptAsmExpression* op1,
                   GptAsmExpression* op2) 
  : _restype(restype), _exprtype(exprtype),
    _mm(mm), _value(""), _op1(op1), _op2(op2) {
}


GptAsmExpression::~GptAsmExpression() {
  delete _op1;
  delete _op2;
}

void GptAsmExpression::expand(const std::string lvalue, Subroutine* sub) {
  if (isAtom()) {
    return;
  }

  if (!_op2) {
    if (_op1->isAtom()) {
      sub->emit(_mm, lvalue, _op1->value());
    } else {
      std::string t1 = sub->declareTmp(_op1->restype());
      _op1->expand(t1, sub);
      sub->emit(_mm, lvalue, t1);
    }
  } else {
    if (_op1->isAtom() && _op2->isAtom()) {
      std::string cp1 = expandCast(lvalue, _op1->value(), _op1->restype(), sub);
      std::string cp2 = expandCast(lvalue, _op2->value(), _op2->restype(), sub);
      sub->emit(_mm, lvalue, cp1, cp2);

    } else if (_op1->isAtom()) {
      std::string t2 = sub->declareTmp(_op2->restype());

      _op2->expand(t2, sub);

      std::string cp1 = expandCast(lvalue, _op1->value(), _op1->restype(), sub);
      std::string cp2 = expandCast(lvalue, t2, _op2->restype(), sub);

      sub->emit(_mm, lvalue, cp1, cp2);

    } else if (_op2->isAtom()) {
      std::string t1 = sub->declareTmp(_op1->restype());
  
      _op1->expand(t1, sub);
  
      std::string cp1 = expandCast(lvalue, t1, _op1->restype(), sub);
      std::string cp2 = expandCast(lvalue, _op2->value(), _op2->restype(), sub);
  
      sub->emit(_mm, lvalue, cp1, cp2);
  
    } else {
  
      std::string t1 = sub->declareTmp(_op1->restype());
      std::string t2 = sub->declareTmp(_op2->restype());
  
      _op1->expand(t1, sub);
      _op2->expand(t2, sub);
  
      std::string cp1 = expandCast(lvalue, t1, _op1->restype(), sub);
      std::string cp2 = expandCast(lvalue, t2, _op2->restype(), sub);
  
      sub->emit(_mm, lvalue, cp1, cp2);
    }
  }
}

bool GptAsmExpression::isAtom() {
  return _value.length() > 0;
}

const std::string& GptAsmExpression::value() { 
  return _value;
}

Type* GptAsmExpression::GptAsmExpression::restype() {
  return _restype;
}

Type* GptAsmExpression::GptAsmExpression::exprtype() {
  return _exprtype;
}

std::string GptAsmExpression::expandCast(const std::string &lvalue,
                                         const std::string& op,
                                         Type *optype,
                                         Subroutine *sub) {
  if (!optype->equals(_exprtype)) {
    sub->emit(opcodeCast(optype, _exprtype), lvalue, op);
    return lvalue;
  }
  return op;
}

std::string GptAsmExpression::opcodeCast(Type* from, Type* to) {
  if ((from->primitiveType() == PortugolTokenTypes::T_INTEIRO) &&
      (to->primitiveType() == PortugolTokenTypes::T_REAL)) {
    return "i2r";
  }

  if ((from->primitiveType() == PortugolTokenTypes::T_CARACTERE) &&
      (to->primitiveType() == PortugolTokenTypes::T_LITERAL)) {
    return "c2s";
  }
  throw;
}
