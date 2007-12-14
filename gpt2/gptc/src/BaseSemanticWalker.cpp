
#include "BaseSemanticWalker.hpp"
#include "PortugolTokenTypes.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Types.hpp"
#include "SymbolTableExceptions.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

bool InitMatrixList::hasUniformDimensions() const {
  const_iterator it;
  int d = -1;
  for (it = begin(); it != end(); ++it) {
    if (d == -1) {
      d = (*it).first;
    } else if (d != (*it).first) {
      return false;
    }
  }
  return true;
}

Type* InitMatrixList::elementsDivergeFrom(Type* from) const {
  if (from->isError()) {
    return NULL;
  }

  const_iterator it;
  for (it = begin(); it != end(); ++it) {
    if (!(*it).second->isError() && !from->attrPromotion((*it).second)) {
      return (*it).second;
    }
  }
  return NULL;
}

Type* InitMatrixList::evaluatedElementType(Type* from) const {
  const_iterator it;
  Type *ret = from;
  for (it = begin(); it != end(); ++it) {
    if (!(*it).second->isError()) {
      ret = ret->attrPromotion((*it).second);
    }
  }
  return ret;
}

int InitMatrixList::divergentDimensionFrom(int d) const {
  const_iterator it;
  for (it = begin(); it != end(); it++) {
    if (d == (*it).first) {
      return d;
    }
  }
  return -1;
}

int InitMatrixList::dimensions() const {
  return back().first;
}

/////////////////////////////////////////////////////////////////////



BaseSemanticWalker::BaseSemanticWalker(SymbolTable* symtable,
                                       const std::string& filepath)
 : antlr::TreeParser(),
  _symtable(symtable),
  _typeBuilder(symtable->typeBuilder()),
  _filepath(filepath) {

  buildSourceLines();
}

void BaseSemanticWalker::buildSourceLines() {
  std::ifstream fi(_filepath.c_str());
  std::string line;
  while (!fi.eof()) {
    getline(fi, line);
    _sourcelines.push_back(line);
  }
}

void BaseSemanticWalker::useLib(const std::string& lib) {
  std::cerr << "Using " << lib << std::endl;
}

Type* BaseSemanticWalker::getStructType(RefPortugolAST node) {
  try {
    return _symtable->getType(node->getText());
  } catch (UndeclaredTypeException e) {
    report(node->getLine(), node->getColumn(),
           std::string("tipo indefinido: ") + node->getText());
    return _typeBuilder->errorType();
  }
}

Type* BaseSemanticWalker::getSymbolType(RefPortugolAST node) {
  try {
    return
      _symtable->getSymbol(node->getText()).type();
  } catch (UndeclaredSymbolException e) {
    report(node->getLine(), node->getColumn(),
           std::string("símbolo não declarado: ") + node->getText());
    return _typeBuilder->errorType();
  }
}

Type* BaseSemanticWalker::getSymbolType(RefPortugolAST parent,
                                        Type* sttype, RefPortugolAST id) {
  if (sttype->isError()) {
    return sttype;
  }

  if (!sttype->isStruct()) {
    report(id->getLine(), id->getColumn(), 
      std::string("'") + parent->getText() + "' não é uma estrutura");
    return _typeBuilder->errorType();
  }

  SymbolList::const_iterator it =
      sttype->fields().findFirstByLexeme(id->getText());

  if (it == sttype->fields().end()) {
    report(id->getLine(), id->getColumn(),
        std::string("estrutura '")  + 
        sttype->name() + "' não possui membro '" + id->getText() + "'");
    return _typeBuilder->errorType();
  }

  return (*it).type();
}


void BaseSemanticWalker::setCurrentScope(RefPortugolAST id, 
                                         const SymbolList& params) {
  _currentScopeSymbol = 
      _symtable->getSymbol(id->getText(), params.typeList());

  _symtable->setScope(_currentScopeSymbol);
}


Type* BaseSemanticWalker::evalMatrixSubscript(RefPortugolAST id, Type* type,
                                              int dimensions) {

  if (type->isError()) {
    return type;
  }

  if (type->isMatrix() && (type->dimensions().size() >= (unsigned int) dimensions)) {
    return type->evalTypeFromSubscript(dimensions);
  } else if (type->isMatrix()) {
    std::stringstream s;
    s << id->getText() 
    << " tem " << type->dimensions().size() 
    << " dimensões"; //dimensão / dimensões
    report(id->getLine(), id->getColumn(), s.str());
  } else {
    report(id->getLine(), id->getColumn(), "'" + id->getText() + "' não é uma matriz");
  }
  return _typeBuilder->errorType();
}

void  BaseSemanticWalker::evalMatrixSubscriptType(RefPortugolAST exp,Type* type) {
  if (type->isError()) {
    return;
  }

  if (!type->equals(PortugolTokenTypes::T_INTEIRO)) {
    report(exp->getLine(), exp->getColumn(), 
      "subscrito da matriz deve ser do tipo inteiro. Encontrado tipo " + type->name());
  }
}

void BaseSemanticWalker::declare(const IDList& ids, Type* type) {
  IDList::const_iterator it;
  for (it = ids.begin(); it != ids.end(); ++it) {
    try {
      _symtable->insertSymbol(
        Symbol((*it)->getText(),
              type,
              _symtable->currentScope(),
              _symtable->unit(),
              (*it)->getLine(),
              (*it)->getColumn()));
    } catch (RedeclarationException e) {
      report((*it)->getLine(), (*it)->getColumn(),
            std::string("redeclaração: ") + e.symbol().lexeme());
    }
  }
}

void BaseSemanticWalker::declareProc(RefPortugolAST id,
                                     SymbolList& params) {

  Type *type = 
    _typeBuilder->subprogramType(
        params.typeList(), 
        _typeBuilder->primitiveType(PortugolTokenTypes::T_NULO));

  Symbol s(id->getText(),
            type,
            _symtable->globalScope(),
            _symtable->unit(),
            id->getLine(),
            id->getColumn());

  declareProc(s, params);
}

void BaseSemanticWalker::declareProc(RefPortugolAST id,
                                     SymbolList& params,
                                     Type *rettype) {

  Type *type = _typeBuilder->subprogramType(params.typeList(), rettype);

  Symbol s(id->getText(),
            type,
            _symtable->globalScope(),
            _symtable->unit(),
            id->getLine(),
            id->getColumn());

  declareProc(s, params);

}

void BaseSemanticWalker::declareProc(const Symbol& s, 
                                     SymbolList& params) {
  try {
    _symtable->insertSymbol(s);
  } catch (RedeclarationException e) {
    report(e.symbol().line(), e.symbol().column(),
           std::string("redeclaração: ") + e.symbol().lexeme());
    _symtable->setIgnoreScope();
    return;
  }  

  _symtable->setScope(s);
  params.setScope(s.identifier());

  try {
    _symtable->insertSymbols(params);
  } catch (RedeclarationException e) {
    report(e.symbol().line(), e.symbol().column(),
           std::string("redeclaração: ") + e.symbol().lexeme());
  }  
}

void BaseSemanticWalker::declareStruct(RefPortugolAST id,
                                       const SymbolList& fieldList) {

  try {
    _symtable->insertType(id->getText(), fieldList, id->getLine());
  } catch (RedefinedTypeException e) {
    report(id->getLine(), id->getColumn(), 
        std::string("redefinição do tipo '") + e.typeName() + "'");
  } catch (RedeclarationException e) {
    report(e.symbol().line(), e.symbol().column(),
        std::string("redeclaração: ") + e.symbol().lexeme());
  }
}

Type* BaseSemanticWalker::evalInitStruct(const InitStructList& stc) {
  SymbolList flist;
  InitStructList::const_iterator it;
  for (it = stc.begin(); it != stc.end(); ++it) {
    flist.push_back(Symbol(it->first->getText(), 
                    it->second, 
                    _symtable->globalScope(), 
                    _symtable->unit(),
                    it->first->getLine(),
                    it->first->getColumn()));
  }
  return _typeBuilder->structType(flist);
}

Type* BaseSemanticWalker::evalInitMatrix(RefPortugolAST node, const InitMatrixList& mtx) {
  if (!mtx.hasUniformDimensions()) {
    std::stringstream s;
    s << "valores da matriz não podem ter dimensões diferentes";

    report(node->getLine(), node->getColumn(), s.str());
    return _typeBuilder->errorType();
  }


  Type *ptype = mtx.front().second;
  Type *dtype;
  if (dtype = mtx.elementsDivergeFrom(ptype)) {
    report(node->getLine(), node->getColumn(), 
                 std::string("matriz heterogenea: ")
                 + ptype->name()
                 + " e "
                 + dtype->name());
    return _typeBuilder->errorType();
  }

  Type *ret = _typeBuilder->matrixType(
        mtx.evaluatedElementType(ptype), mtx.dimensions());

  return ret;
}



void BaseSemanticWalker::evalAttribution(RefPortugolAST lastId,
                                         Type* ltype, Type* rtype) {
  if (ltype->isError() || rtype->isError()) {
    return;
  }

  if (!ltype->isLValueFor(rtype)) {
    report(lastId->getLine(), lastId->getColumn(), 
            string("variável do tipo '") + ltype->name() 
            + "' não pode receber valor do tipo '" + rtype->name() + "'");
  }
}

void BaseSemanticWalker::evalAttribution(const ExpressionReturn& l, 
                                         const ExpressionReturn& r) {
  if (l.second->isError() || r.second->isError()) {
    return;
  }

  if (!l.second->isLValueFor(r.second)) {
    report(r.first->getLine(), r.first->getColumn(), 
            "variável do tipo '" + l.second->name() 
            + "' não pode receber valor do tipo '" + r.second->name() + "'");
  }
}

Type* BaseSemanticWalker::evalCall(RefPortugolAST id,
                                   const TypeList& paramTypes) {
  //TODO
  //-proibicao de consts passados para parametros "ref" sem constness

// - Testar avaliacao de chamada de subprogramas (com e sem reticencias)
//   -Avaliacao de qtd e tipos de parametros

  if (paramTypes.hasErrorType()) {
    return _typeBuilder->errorType();
  }

  try {
    return 
      _symtable->getSymbol(id->getText(), paramTypes).type()->returnType();
  } catch (UndeclaredSymbolException e) {
    report(id->getLine(), id->getColumn(), 
        "função não encontrada: '" + id->getText() + "'");
    return _typeBuilder->errorType();
  } catch (UnmatchedException e) {
    report(id->getLine(), id->getColumn(),
           "Função compatível com '" + id->getText() 
           + "(" + paramTypes.toString() + ")' não encontrada");
    return _typeBuilder->errorType();
  }
}

void BaseSemanticWalker::evalRetorne(RefPortugolAST ret,Type* type) {
  if (type->isError()) {
    return;
  }

  if (_symtable->isInGlobalScope()) {
    report(ret->getLine(), ret->getColumn(), 
        "não há retorno em escopo global");
  } else if(!_currentScopeSymbol.type()->returnType()->isLValueFor(type)) {
    report(ret->getLine(), ret->getColumn(), "tipo de retorno (" 
           + type->name() + ") incompatível com tipo da função (" 
           + _currentScopeSymbol.type()->returnType()->name() +")");
  }
}


void BaseSemanticWalker::evalCondicional(const ExpressionReturn& ex) {
  if (ex.second->isError()) {
    return;
  }

  if (!ex.second->equals(PortugolTokenTypes::T_LOGICO)) {
    report(ex.first->getLine(), ex.first->getColumn(),
      "enunciado exige expressão lógica. Tipo '" 
      + ex.second->name() + "' encontrado.");
  }
}

/************************ EXPR ********************************************/

Type*
BaseSemanticWalker::evalExpr_OU(RefPortugolAST,Type* left, Type* right) {
  //TODO: refatorar os copy/pastes dos metodos evalExpr

  return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
}

Type*
BaseSemanticWalker::evalExpr_E(RefPortugolAST,Type* left, Type* right) {
  return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
}


Type*
BaseSemanticWalker::evalExpr_BIT_OU(RefPortugolAST op,Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(), 
      std::string("ilegal: ") + left->name() + " | " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_OUX(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " ^ " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_E(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " & " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_IGUAL(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(right)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " = " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_DIFERENTE(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(right)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " <> " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MAIOR(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " > " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MENOR(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " < " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MAIOR_EQ(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " >= " + right->name());
    return _typeBuilder->errorType();
  }
}



Type*
BaseSemanticWalker::evalExpr_MENOR_EQ(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " <= " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_SHIFT_LEFT(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " << " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_SHIFT_RIGHT(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " >> " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MAIS(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " + " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else if (ret = left->caracOrLit(right)) {
    return ret;
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " + " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MENOS(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " - " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " - " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_DIV(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " / " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " / " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MULTIP(RefPortugolAST op, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " * " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " * " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MOD(RefPortugolAST op, Type* left, Type* right) {
  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + left->name() + " % " + right->name());
    return _typeBuilder->errorType();
  }
}



Type*
BaseSemanticWalker::evalExpr_UN_NEGATIVO(RefPortugolAST op, Type* left) {
  if (left->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) ||
      left->equals(PortugolTokenTypes::T_REAL)) {
    return _typeBuilder->primitiveType(left->primitiveType());
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + " - " + left->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_UN_POSITIVO(RefPortugolAST op, Type* left) {
  if (left->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) ||
      left->equals(PortugolTokenTypes::T_REAL)) {
    return _typeBuilder->primitiveType(left->primitiveType());
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + " + " + left->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_NAO(RefPortugolAST, Type* left) {
  return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
}


Type*
BaseSemanticWalker::evalExpr_BIT_NAO(RefPortugolAST op, Type* left) {
  if (left->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(op->getLine(), op->getColumn(),
      std::string("ilegal: ") + " ~ " + left->name());
    return _typeBuilder->errorType();
  }
}


/********************************************************************/


using std::cerr;
using std::endl;


void BaseSemanticWalker::report(int line, int col, const std::string& msg) {
  //<file>:<line>: <message>
  //.............<source code>
  //...................^

  std::string space = "                ";
  std::cerr << _filepath << ":" << line << ": " << msg << std::endl;
  std::cerr << space << _sourcelines.at(line-1) << std::endl;

  for (unsigned int i = 1; i < col + space.length(); i++) {
    std::cerr << " ";
  }
  std::cerr << "^" << std::endl;
}

void BaseSemanticWalker::report(int line, const std::string& s) {
  cerr << "linha " << line << " - " << s << endl;
}
