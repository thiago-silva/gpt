
#include "BaseSemanticWalker.hpp"
#include "PortugolTokenTypes.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Types.hpp"
#include "SymbolTableExceptions.hpp"

#include <iostream>
#include <sstream>
using std::cerr;
using std::endl;



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
    if (!(*it).second->isError() && !from->promotion((*it).second)) {
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
      ret = ret->promotion((*it).second);
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



BaseSemanticWalker::BaseSemanticWalker(SymbolTable* symtable)
 : antlr::TreeParser(),
  _symtable(symtable),
  _typeBuilder(symtable->typeBuilder()) {
}

void BaseSemanticWalker::useLib(const std::string& lib) {
  cerr << "Using " << lib << endl;
}

Type* BaseSemanticWalker::getStructType(RefPortugolAST node) {
  try {
    return _symtable->getType(node->getText());
  } catch (UndeclaredTypeException e) {
    report(node->getLine(),
           std::string("Undefined type: ") + node->getText());
    return _typeBuilder->errorType();
  }
}

Type* BaseSemanticWalker::getSymbolType(RefPortugolAST node) {
  try {
    return
      _symtable->getSymbol(node->getText()).type();
  } catch (UndeclaredSymbolException e) {
    report(node->getLine(),
           std::string("Undeclared symbol: ") + node->getText());
    return _typeBuilder->errorType();
  }
}

Type* BaseSemanticWalker::getSymbolType(RefPortugolAST parent,
                                        Type* sttype, RefPortugolAST id) {
  if (sttype->isError()) {
    return sttype;
  }

  if (!sttype->isStruct()) {
    report(id->getLine(), parent->getText() + " não é uma estrutura");
    return _typeBuilder->errorType();
  }

  SymbolList::const_iterator it =
      sttype->fields().findFirstByLexeme(id->getText());

  if (it == sttype->fields().end()) {
    report(id->getLine(),
        sttype->name() + " não possui membro " + id->getText());
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
    s << id->getText() << " tem " << type->dimensions().size() << " dimensões";
    report(id->getLine(), s.str());
  } else {
    report(id->getLine(), id->getText() + " não é uma matriz");
  }
  return _typeBuilder->errorType();
}

void BaseSemanticWalker::declare(const IDList& ids, Type* type) {
  try {
    IDList::const_iterator it;
    for (it = ids.begin(); it != ids.end(); ++it) {
      _symtable->insertSymbol(
        Symbol((*it)->getText(),
              type,
              _symtable->currentScope(),
              _symtable->unit(),
              (*it)->getLine()));
    }
  } catch (RedeclarationException e) {
    report(ids.back()->getLine(),
           std::string("Redeclaration: ") + e.symbol().toString());
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
            id->getLine());

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
            id->getLine());

  declareProc(s, params);

}

void BaseSemanticWalker::declareProc(const Symbol& s, 
                                     SymbolList& params) {
  try {
    _symtable->insertSymbol(s);
  } catch (RedeclarationException e) {
    report(e.symbol().line(),
           std::string("Redeclaration: ") + e.symbol().toString());
    _symtable->setIgnoreScope();
    return;
  }  

  _symtable->setScope(s);
  params.setScope(s.identifier());

  try {
    _symtable->insertSymbols(params);
  } catch (RedeclarationException e) {
    report(e.symbol().line(),
           std::string("Redeclaration: ") + e.symbol().toString());
  }  
}

void BaseSemanticWalker::declareStruct(RefPortugolAST id,
                                       const SymbolList& fieldList) {

  try {
    _symtable->insertType(id->getText(), fieldList, id->getLine());
  } catch (RedefinedTypeException e) {
    report(id->getLine(), std::string("Redefinition: ") + e.typeName());
  } catch (RedeclarationException e) {
    report(id->getLine(), std::string("Redeclaration: ") + e.symbol().toString());
  }
}

// void BaseSemanticWalker::defineStruct(RefPortugolAST id,
//                                       const SymbolList& fieldList) {
// 
//   try {
//     _symtable->insertType(id->getText(), fieldList, id->getLine());
//   } catch (RedefinedTypeException e) {
//     report(id->getLine(), std::string("Redefinition: ") + e.typeName());
//   } catch (RedeclarationException e) {
//     report(id->getLine(), std::string("Redeclaration: ") + e.symbol().toString());
//   }
// }

Type* BaseSemanticWalker::evalInitStruct(const InitStructList& stc) {
  SymbolList flist;
  InitStructList::const_iterator it;
  for (it = stc.begin(); it != stc.end(); ++it) {
    flist.push_back(Symbol(it->first->getText(), 
                    it->second, 
                    _symtable->globalScope(), 
                    _symtable->unit(),
                    it->first->getLine()));
  }
  return _typeBuilder->structType(flist);
}

Type* BaseSemanticWalker::evalInitMatrix(int line,const InitMatrixList& mtx) {
  if (!mtx.hasUniformDimensions()) {
    std::stringstream s;
    s << "Matriz tem valores em dimensões diferentes";

    report(line, s.str());
    return _typeBuilder->errorType();
  }


  Type *ptype = mtx.front().second;
  Type *dtype;
  if (dtype = mtx.elementsDivergeFrom(ptype)) {
    report(line, std::string("Matriz heterogenea: ")
                 + ptype->name()
                 + " e "
                 + dtype->name());
    return _typeBuilder->errorType();
  }

  Type *ret = _typeBuilder->matrixType(
        mtx.evaluatedElementType(ptype), mtx.dimensions());

  return ret;
}



void BaseSemanticWalker::evalAttribution(int line,
                                         Type* ltype, Type* rtype) {
  if (ltype->isError() || rtype->isError()) {
    return;
  }

  if (!ltype->isLValueFor(rtype)) {
    report(line, string("ilegal: ") + ltype->name() + " := " + rtype->name());
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
    report(id->getLine(), std::string("Undeclared '") + id->getText());
    return _typeBuilder->errorType();
  } catch (UnmatchedException e) {
    report(id->getLine(),
           std::string("No matching call for '") + id->getText() 
           + "(" + paramTypes.toString() + ")'");
    return _typeBuilder->errorType();
  }
}

void BaseSemanticWalker::evalRetorne(int line,Type* type) {
  if (_symtable->isInGlobalScope()) {
    report(line, "Não há retorno em escopo global");
  } else if(!_currentScopeSymbol.type()->returnType()->isLValueFor(type)) {
    report(line, "Tipo de retorno (" 
           + type->name() + ") incompatível com tipo da função (" 
           + _currentScopeSymbol.type()->returnType()->name() +")");
  }
}


void BaseSemanticWalker::evalCondicional(int line, Type* type) {
  if (!type->equals(PortugolTokenTypes::T_LOGICO)) {
    report(line, 
      std::string("Enunciado exige expressão lógica. Tipo ") 
      + type->name() + " encontrado.");
  }
}

/************************ EXPR ********************************************/

Type*
BaseSemanticWalker::evalExpr_OU(int,Type* left, Type* right) {
  //TODO: refatorar os copy/pastes dos metodos evalExpr

  return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
}

Type*
BaseSemanticWalker::evalExpr_E(int,Type* left, Type* right) {
  return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
}


Type*
BaseSemanticWalker::evalExpr_BIT_OU(int line,Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " | " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_OUX(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " ^ " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_E(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " & " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_IGUAL(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(right)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " = " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_DIFERENTE(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(right)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " <> " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MAIOR(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " > " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MENOR(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " < " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MAIOR_EQ(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " >= " + right->name());
    return _typeBuilder->errorType();
  }
}



Type*
BaseSemanticWalker::evalExpr_MENOR_EQ(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if ((left->isPrimitive() && right->isPrimitive()) &&
      (left->equals(right) || left->intOrReal(right))) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " <= " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_SHIFT_LEFT(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " << " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_BIT_SHIFT_RIGHT(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " >> " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MAIS(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(line, 
      std::string("ilegal: ") + left->name() + " + " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else if (ret = left->caracOrLit(right)) {
    return ret;
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " + " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MENOS(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(line, 
      std::string("ilegal: ") + left->name() + " - " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " - " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_DIV(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(line, 
      std::string("ilegal: ") + left->name() + " / " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " / " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MULTIP(int line, Type* left, Type* right) {
  if (left->isError() || right->isError()) {
    return _typeBuilder->errorType();
  }

  if (!left->isPrimitive() || !right->isPrimitive()) {
    report(line, 
      std::string("ilegal: ") + left->name() + " * " + right->name());
    return _typeBuilder->errorType();
  }

  Type* ret;
  if (ret = left->intOrReal(right)) {
    return ret;
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " * " + right->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_MOD(int line, Type* left, Type* right) {
  if (left->equals(PortugolTokenTypes::T_INTEIRO) &&
      right->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + left->name() + " % " + right->name());
    return _typeBuilder->errorType();
  }
}



Type*
BaseSemanticWalker::evalExpr_UN_NEGATIVO(int line, Type* left) {
  if (left->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) ||
      left->equals(PortugolTokenTypes::T_REAL)) {
    return _typeBuilder->primitiveType(left->primitiveType());
  } else {
    report(line, 
      std::string("ilegal: ") + " - " + left->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_UN_POSITIVO(int line, Type* left) {
  if (left->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO) ||
      left->equals(PortugolTokenTypes::T_REAL)) {
    return _typeBuilder->primitiveType(left->primitiveType());
  } else {
    report(line, 
      std::string("ilegal: ") + " + " + left->name());
    return _typeBuilder->errorType();
  }
}


Type*
BaseSemanticWalker::evalExpr_NAO(int, Type* left) {
  return _typeBuilder->primitiveType(PortugolTokenTypes::T_LOGICO);
}


Type*
BaseSemanticWalker::evalExpr_BIT_NAO(int line, Type* left) {
  if (left->isError()) {
    return _typeBuilder->errorType();
  }

  if (left->equals(PortugolTokenTypes::T_INTEIRO)) {
    return _typeBuilder->primitiveType(PortugolTokenTypes::T_INTEIRO);
  } else {
    report(line, 
      std::string("ilegal: ") + " ~ " + left->name());
    return _typeBuilder->errorType();
  }
}


/********************************************************************/




void BaseSemanticWalker::report(int line, const std::string& s) {
  cerr << "linha " << line << " - " << s << endl;
}
