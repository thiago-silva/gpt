#include "BaseGptAsmWalker.hpp"

#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "AsmProgram.hpp"
#include "Types.hpp"
#include "GptAsmExpression.hpp"

BaseGptAsmWalker::BaseGptAsmWalker(SymbolTable* symtable,
                                   const std::string& filepath)
  : antlr::TreeParser(), _symtable(symtable), _filepath(filepath) {

  _symtable->setGlobalScope();

  _options.filename = _filepath;
  _asmPrg = new AsmProgram(&_options);

  _subroutine = _asmPrg->initSubroutine("main");
}


void BaseGptAsmWalker::finishParser() {
  delete _asmPrg;
  _asmPrg = NULL;
}

void BaseGptAsmWalker::declareProgramVars(const std::list<std::string>& ids) {
  for (std::list<std::string>::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    Symbol symbol = _symtable->getSymbol(*it);
    _asmPrg->emitVarDefinition(symbol.identifier(), symbol.type()->asmName(true));

    if (symbol.type()->isMatrix()) {
      addMatrixInitialization(symbol);
    }
  }
}

void BaseGptAsmWalker::addMatrixInitialization(const Symbol& symbol) {
  std::list<int> dims = symbol.type()->dimensions();

  std::stringstream code;

  if (dims.size() == 1) {
    code << "m1alloc " << symbol.identifier()
            << ", " << symbol.type()->ofType()->byteSize()
            << ", " << dims.front();
  } else if(dims.size() == 2) {
    code << "m2alloc " << symbol.identifier()
            << ", " << symbol.type()->ofType()->byteSize()
            << ", " << dims.front()
            << ", " << *(dims.rbegin());
  }
  _subroutine->emitAsmCode(code.str());
}


void BaseGptAsmWalker::declareSubroutineVars(const std::list<std::string>& ids) {
  for (std::list<std::string>::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    _subroutine->emitVarDefinition(
      *it, _symtable->getSymbol(*it).type());
  }
}

void BaseGptAsmWalker::declareParameter(const string& lexeme) {
  _subroutine->emitParDefinition(
    lexeme, _symtable->getSymbol(lexeme).type());
}

int BaseGptAsmWalker::getSymbolType(const string& lexeme) {
  return _symtable->getSymbol(lexeme).type()->primitiveType();
}

// std::string BaseGptAsmWalker::expandOffset(const std::string&, const std::list<std::string>&) {
  //??TODO
// }


void BaseGptAsmWalker::emitAttribution(const Symbol& lv, const std::string& tmp) {
  switch (lv.type()->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      _subroutine->emit("rset", lv.identifier(), tmp);
      break;
    case PortugolTokenTypes::T_LITERAL:
      _subroutine->emit("sset", lv.identifier(), tmp);
      break;
    case PortugolTokenTypes::T_INTEIRO:
    case PortugolTokenTypes::T_LOGICO:
    case PortugolTokenTypes::T_CARACTERE:
      _subroutine->emit("iset", lv.identifier(), tmp);
      break;
    default:
      throw;
  }
}





GptAsmExpression* BaseGptAsmWalker::expr_OU(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, optype, "lor", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_E(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, optype, "land", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_BIT_OU(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, optype, "bor", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_BIT_XOU(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, optype, "bxor", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_BIT_E(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, optype, "band", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_IGUAL(Type *optype,
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  
  //lh e rh tem o mesmo tipo
  switch (left->restype()->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, left->restype(), "req", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, left->restype(), "seq", left, right);
      break;
    default: //inteiro, lógico, caractere
      return new GptAsmExpression(optype, left->restype(), "ieq", left, right);
      break;
  }
}

GptAsmExpression* BaseGptAsmWalker::expr_DIFERENTE(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  //lh e rh tem o mesmo tipo
  switch (left->restype()->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, left->restype(), "rne", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, left->restype(), "sne", left, right);
      break;
    default: //inteiro, lógico, caractere
      return new GptAsmExpression(optype, left->restype(), "ine", left, right);
      break;
  }
}

GptAsmExpression* BaseGptAsmWalker::expr_MAIOR(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType;
  if ((promType = left->restype()->intOrReal(right->restype())) == NULL) {
    promType = left->restype()->caracOrLit(right->restype());
  }

  switch (promType->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, promType, "rgt", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, promType, "sgt", left, right);
      break;
    default: //inteiro, lógico, caractere
      return new GptAsmExpression(optype, promType, "igt", left, right);
      break;
  }
}

GptAsmExpression* BaseGptAsmWalker::expr_MENOR(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType;
  if ((promType = left->restype()->intOrReal(right->restype())) == NULL) {
    promType = left->restype()->caracOrLit(right->restype());
  }

  switch (promType->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, promType, "rlt", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, promType, "slt", left, right);
      break;
    default: //inteiro, lógico, caractere
      return new GptAsmExpression(optype, promType, "ilt", left, right);
      break;
  }
}

GptAsmExpression* BaseGptAsmWalker::expr_MAIOR_EQ(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType;
  if ((promType = left->restype()->intOrReal(right->restype())) == NULL) {
    promType = left->restype()->caracOrLit(right->restype());
  }

  switch (promType->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, promType, "rge", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, promType, "sge", left, right);
      break;
    default: //inteiro, lógico, caractere
      return new GptAsmExpression(optype, promType, "ige", left, right);
      break;
  }
}

GptAsmExpression* BaseGptAsmWalker::expr_MENOR_EQ(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  Type *promType;
  if ((promType = left->restype()->intOrReal(right->restype())) == NULL) {
    promType = left->restype()->caracOrLit(right->restype());
  }

  switch (promType->primitiveType()) {
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, promType, "rle", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, promType, "sle", left, right);
      break;
    default: //inteiro, lógico, caractere
      return new GptAsmExpression(optype, promType, "ile", left, right);
      break;
  }
}

GptAsmExpression* BaseGptAsmWalker::expr_BIT_SHIFT_LEFT(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, left->restype(), "bshl", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_BIT_SHIFT_RIGHT(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, left->restype(), "bshr", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_MAIS(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType;
  if ((promType = left->restype()->intOrReal(right->restype())) == NULL) {
    promType = left->restype()->caracOrLit(right->restype());
  }

  switch (optype->primitiveType()) {
    case PortugolTokenTypes::T_INTEIRO:
      return new GptAsmExpression(optype, promType, "isum", left, right);
      break;
    case PortugolTokenTypes::T_REAL:
      return new GptAsmExpression(optype, promType, "rsum", left, right);
      break;
    case PortugolTokenTypes::T_LITERAL:
      return new GptAsmExpression(optype, promType, "ssum", left, right);
      break;
  }
  throw;
}

GptAsmExpression* BaseGptAsmWalker::expr_MENOS(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType = left->restype()->intOrReal(right->restype());

  if (optype->equals(PortugolTokenTypes::T_INTEIRO)) {
    return new GptAsmExpression(optype, promType, "isub", left, right);
  } else if (optype->equals(PortugolTokenTypes::T_REAL)) {
    return new GptAsmExpression(optype, promType, "rsub", left, right);
  }
  throw;
}

GptAsmExpression* BaseGptAsmWalker::expr_DIV(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType = left->restype()->intOrReal(right->restype());

  if (optype->equals(PortugolTokenTypes::T_INTEIRO)) {
    return new GptAsmExpression(optype, promType, "idiv", left, right);
  } else if (optype->equals(PortugolTokenTypes::T_REAL)) {
    return new GptAsmExpression(optype, promType, "rdiv", left, right);
  }
  throw;
}

GptAsmExpression* BaseGptAsmWalker::expr_MULTIP(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {

  Type *promType = left->restype()->intOrReal(right->restype());

  if (optype->equals(PortugolTokenTypes::T_INTEIRO)) {
    return new GptAsmExpression(optype, promType, "imul", left, right);
  } else if (optype->equals(PortugolTokenTypes::T_REAL)) {
    return new GptAsmExpression(optype, promType, "rmul", left, right);
  }
  throw;
}

GptAsmExpression* BaseGptAsmWalker::expr_MOD(Type *optype, 
                                            GptAsmExpression* left, 
                                            GptAsmExpression* right) {
  return new GptAsmExpression(optype, left->restype(), "imod", left, right);
}

GptAsmExpression* BaseGptAsmWalker::expr_UN_NEGATIVO(Type *optype, 
                                            GptAsmExpression* term) {

  if (optype->equals(PortugolTokenTypes::T_INTEIRO)) {
    return new GptAsmExpression(optype, term->restype(), "inet", term);
  } else if (optype->equals(PortugolTokenTypes::T_REAL)) {
    return new GptAsmExpression(optype, term->restype(), "rneg", term);
  }
  throw;
}

GptAsmExpression* BaseGptAsmWalker::expr_UN_POSITIVO(Type*,GptAsmExpression* term) {
  return term;
}

GptAsmExpression* BaseGptAsmWalker::expr_UN_NAO(Type* optype,GptAsmExpression* term) {
  return new GptAsmExpression(optype, term->restype(), "not", term);
}

GptAsmExpression* BaseGptAsmWalker::expr_BIT_NAO(Type* optype,GptAsmExpression* term) {
  return new GptAsmExpression(optype, term->restype(), "bnot", term);
}

std::string BaseGptAsmWalker::getCode() {
  return _asmPrg->getAsm();
}

