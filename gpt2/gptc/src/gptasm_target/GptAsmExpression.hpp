#ifndef GPT_ASMEXPRESSION_HPP
#define GPT_ASMEXPRESSION_HPP

#include <string>


class Type;
class Subroutine;

class GptAsmExpression {
public:
  GptAsmExpression(Type* restype, const std::string& value);

  GptAsmExpression(Type* restype, Type *exprtype, std::string, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression(Type* restype, Type *exprtype, std::string, GptAsmExpression*);

  ~GptAsmExpression();

  void expand(const std::string lvalue, Subroutine* sub);

  bool isAtom();

  const std::string& value();
  Type*              exprtype();
  Type*              restype();
private:
  std::string expandCast(const std::string&,
                         const std::string&,
                         Type*, Subroutine*);

  std::string opcodeCast(Type* from, Type* to);


  Type              *_restype;
  Type              *_exprtype;
  std::string       _mm;
  std::string       _value;
  GptAsmExpression  *_op1;
  GptAsmExpression  *_op2;
};

#endif
