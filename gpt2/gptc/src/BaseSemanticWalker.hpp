/***************************************************************************
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
 *   tsilva@sourcecraft.info                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef BASESEMANTICWALKER_H
#define BASESEMANTICWALKER_H

#include <antlr/TreeParser.hpp>
#include <list>
#include <vector>
#include <string>

#include "PortugolAST.hpp"
#include "Symbol.hpp"

class SymbolTable;
class Type;
class TypeList;
class TypeBuilder;

typedef std::list<RefPortugolAST>                IDList;

//list<pair<field, type>>
typedef std::list<std::pair<RefPortugolAST,Type*> > InitStructList;

typedef std::pair<RefPortugolAST,Type*>  ExpressionReturn;

//list<pair<dimsize,type>>
class InitMatrixList : public std::list<std::pair<int,Type*> > {
public:
  bool              hasUniformDimensions() const;
  Type*             elementsDivergeFrom(Type*) const;
  Type*             evaluatedElementType(Type* from) const;
  int               divergentDimensionFrom(int) const;
  int               dimensions() const;
};

class BaseSemanticWalker : public antlr::TreeParser {

public:
  BaseSemanticWalker(SymbolTable*, const std::string&);

protected:
  void useLib(const std::string&);

  Type* getStructType(RefPortugolAST);

  Type* getSymbolType(RefPortugolAST);

  Type* getSymbolType(RefPortugolAST,Type*, RefPortugolAST);

  void setCurrentScope(RefPortugolAST id, const SymbolList& params);

  void declare(const IDList&, Type*);

  void declareProc(RefPortugolAST, SymbolList&);
  void declareProc(RefPortugolAST, SymbolList&, Type*);
  void declareProc(const Symbol&,  SymbolList&);

//   void defineStruct(RefPortugolAST, const SymbolList&);
  void declareStruct(RefPortugolAST, const SymbolList& );


  Type* evalInitStruct(const InitStructList& stc);
  Type* evalInitMatrix(RefPortugolAST, const InitMatrixList& mtx);

  Type* evalMatrixSubscript(RefPortugolAST, Type*, int);
  void  evalMatrixSubscriptType(RefPortugolAST,Type*);

  void evalAttribution(RefPortugolAST,Type*, Type*);
  void evalAttribution(const ExpressionReturn&, const ExpressionReturn&);

  Type* evalCall(RefPortugolAST id, const TypeList& paramTypes);

  void evalRetorne(RefPortugolAST,Type*);

  void evalCondicional(const ExpressionReturn&);

  Type* evalExpr_OU(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_E(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_BIT_OU(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_BIT_OUX(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_BIT_E(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_IGUAL(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_DIFERENTE(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MAIOR(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MENOR(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MAIOR_EQ(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MENOR_EQ(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_BIT_SHIFT_LEFT(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_BIT_SHIFT_RIGHT(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MAIS(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MENOS(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_DIV(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MULTIP(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_MOD(RefPortugolAST,Type* left, Type* right);
  Type* evalExpr_UN_NEGATIVO(RefPortugolAST,Type*);
  Type* evalExpr_UN_POSITIVO(RefPortugolAST,Type*);
  Type* evalExpr_NAO(RefPortugolAST,Type*);
  Type* evalExpr_BIT_NAO(RefPortugolAST,Type*);

  void buildSourceLines();

  void report(int, int, const std::string&);
  void report(int, const std::string&);
  

  SymbolTable*                 _symtable;
  TypeBuilder*                 _typeBuilder;
  std::string                  _filepath;
  std::vector<std::string>     _sourcelines;
  Symbol                       _currentScopeSymbol;
};

#endif
