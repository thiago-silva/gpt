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
  BaseSemanticWalker(SymbolTable*);

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
  Type* evalInitMatrix(int, const InitMatrixList& mtx);

  Type* evalMatrixSubscript(RefPortugolAST, Type*, int);

  void evalAttribution(int,Type*, Type*);

  Type* evalCall(RefPortugolAST id, const TypeList& paramTypes);

  void evalRetorne(int,Type*);

  void evalCondicional(int, Type*);

  Type* evalExpr_OU(int,Type* left, Type* right);
  Type* evalExpr_E(int,Type* left, Type* right);
  Type* evalExpr_BIT_OU(int,Type* left, Type* right);
  Type* evalExpr_BIT_OUX(int,Type* left, Type* right);
  Type* evalExpr_BIT_E(int,Type* left, Type* right);
  Type* evalExpr_IGUAL(int,Type* left, Type* right);
  Type* evalExpr_DIFERENTE(int,Type* left, Type* right);
  Type* evalExpr_MAIOR(int,Type* left, Type* right);
  Type* evalExpr_MENOR(int,Type* left, Type* right);
  Type* evalExpr_MAIOR_EQ(int,Type* left, Type* right);
  Type* evalExpr_MENOR_EQ(int,Type* left, Type* right);
  Type* evalExpr_BIT_SHIFT_LEFT(int,Type* left, Type* right);
  Type* evalExpr_BIT_SHIFT_RIGHT(int,Type* left, Type* right);
  Type* evalExpr_MAIS(int,Type* left, Type* right);
  Type* evalExpr_MENOS(int,Type* left, Type* right);
  Type* evalExpr_DIV(int,Type* left, Type* right);
  Type* evalExpr_MULTIP(int,Type* left, Type* right);
  Type* evalExpr_MOD(int,Type* left, Type* right);
  Type* evalExpr_UN_NEGATIVO(int,Type*);
  Type* evalExpr_UN_POSITIVO(int,Type*);
  Type* evalExpr_NAO(int,Type*);
  Type* evalExpr_BIT_NAO(int,Type*);

  void report(int, const std::string&);

  SymbolTable* _symtable;
  TypeBuilder* _typeBuilder;
  Symbol       _currentScopeSymbol;
};

#endif
