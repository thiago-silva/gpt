
#include "BaseSemanticWalker.hpp"
#include "PortugolTokenTypes.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Types.hpp"
#include "PortugolAST.hpp"
#include "SymbolTableExceptions.hpp"

#include <iostream>

BaseSemanticWalker::BaseSemanticWalker(SymbolTable* symtable)
 : antlr::TreeParser(), _symtable(symtable) {}

void BaseSemanticWalker::useLib(const  std::string& lib) {
  //TODO: sanitize lib: "bla\ bla" -> "bla bla"

  std::cerr << "Using lib:" << lib << std::endl;
}

void BaseSemanticWalker::declare(const IDList& idlist,
                                 Type* type,
                                 bool isConst) {

  try {
    IDList::const_iterator it;
    for (it = idlist.begin(); it != idlist.end(); ++it) {
      _symtable->declare(_symtable->newSymbol(
            (*it)->getText(), type, (*it)->getLine(), isConst));
    }
  } catch (RedeclarationException e) {
    cerr << "catched! Redeclaration: " << e.symbol().toString() <<"\n";
  }
}

void BaseSemanticWalker::defineStruct(const std::string& name,
                                      const SymbolList& fieldList,
                                      int line) {

  try {
    _symtable->defineStruct(name, fieldList, line);
  } catch (RedefinedTypeException e) {
    cerr << "catched! Redeclared type: " << e.typeName() << "\n";
  } catch (RedeclarationException e) {
    cerr << "catched! Redeclaration: " << e.symbol().toString() << "\n";
  }
}

void BaseSemanticWalker::declareProc(const std::string& name,
                                     const SymbolList& params,
                                     int line,
                                     Type* ret) {

  SubprogramType* type = new SubprogramType(
        params.toTypeList(),
        (ret == 0) ? _symtable->getType(PortugolTokenTypes::T_NULO) : ret);

  try {
    _symtable->declare(_symtable->newSymbol(name, type,line));
    _symtable->declare(params, name);
  } catch (RedeclarationException e) {
    cerr << "catched! Redeclaration: " << e.symbol().toString() << std::endl;
    delete type;
  }
}

Type* BaseSemanticWalker::getType(const std::string& name) {
  try {
    return _symtable->getType(name);
  } catch (UndeclaredTypeException e) {
    std::cerr << "catched! Undeclared type: " << e.typeName() << std::endl;
    return _symtable->getType(PortugolTokenTypes::T_NULO);
  }
}

Type* BaseSemanticWalker::getType(int id) {
  try {
    return _symtable->getType(id);
  } catch (UndeclaredTypeException e) {
    std::cerr << "catched! Undeclared type: " << id << std::endl;
    return _symtable->getType(PortugolTokenTypes::T_NULO);
  }
}

Type* BaseSemanticWalker::getSymbolType(const std::string& name) {
  try {
    return _symtable->getSymbol(name).type();
  } catch (UndeclaredSymbolException e) {
    std::cerr << "catched! Undeclared symbol: " << e.lexeme() << std::endl;
    return _symtable->getType(PortugolTokenTypes::T_NULO);
  }
}

void BaseSemanticWalker::evalFCall(const std::string& fname,
                                   const TypeList& paramTypes) {
  //TODO
  //-proibicao de consts passados para parametros "ref" sem constness

// - Testar avaliacao de chamada de subprogramas (com e sem reticencias)
//   -Avaliacao de qtd e tipos de parametros
}

void BaseSemanticWalker::evalMatrixSubscript(Type* type) {
}

void BaseSemanticWalker::evalAttribution(Type*, Type*) {
  //-proibicao de consts como lvalues
}


template<int>
Type* BaseSemanticWalker::evalExpr(Type*, Type*) {
  //nao deve ser chamado!
  throw;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_OU>(Type* left,
                                                       Type* right) {
  std::cerr <<  left->name() << " OU " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_E>(Type* left,
                                                      Type* right) {
  std::cerr <<  left->name() << " E " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_BIT_OU>(Type* left,
                                                           Type* right) {
  std::cerr <<  left->name() << " | " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_BIT_OUX>(Type* left,
                                                            Type* right) {
  std::cerr <<  left->name() << " ^ " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_BIT_E>(Type* left,
                                                          Type* right) {
  std::cerr <<  left->name() << " & " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_IGUAL>(Type* left,
                                                          Type* right) {
  std::cerr <<  left->name() << " = " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_DIFERENTE>(Type* left,
                                                              Type* right) {
  std::cerr <<  left->name() << " = " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MAIOR>(Type* left,
                                                          Type* right) {
  std::cerr <<  left->name() << " > " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MENOR>(Type* left,
                                                          Type* right) {
  std::cerr <<  left->name() << " < " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MAIOR_EQ>(Type* left,
                                                             Type* right) {
  std::cerr <<  left->name() << " >= " << right->name() << endl;
  return left;
}


template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MENOR_EQ>(Type* left,
                                                             Type* right) {
  std::cerr <<  left->name() << " <= " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_BIT_SHIFT_LEFT>(
                                                    Type* left, Type* right) {
  std::cerr <<  left->name() << " << " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_BIT_SHIFT_RIGHT>(
                                                    Type* left, Type* right) {
  std::cerr <<  left->name() << " >> " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MAIS>(Type* left,
                                                         Type* right) {
  std::cerr <<  left->name() << " + " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MENOS>(Type* left,
                                                          Type* right) {
  std::cerr <<  left->name() << " - " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_DIV>(Type* left,
                                                        Type* right) {
  std::cerr <<  left->name() << " / " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MULTIP>(Type* left,
                                                           Type* right) {
  std::cerr <<  left->name() << " * " << right->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_MOD>(Type* left,
                                                        Type* right) {
  std::cerr <<  left->name() << " % " << right->name() << endl;
  return left;
}


template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_UN_NEGATIVO>(Type* left) {
  std::cerr <<  " - " << left->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_UN_POSITIVO>(Type* left) {
  std::cerr <<  " + " << left->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_NAO>(Type* left) {
  std::cerr <<  " não " << left->name() << endl;
  return left;
}

template<>
Type*
BaseSemanticWalker::evalExpr<PortugolTokenTypes::T_BIT_NAO>(Type* left) {
  std::cerr <<  " ~ " << left->name() << endl;
  return left;
}

