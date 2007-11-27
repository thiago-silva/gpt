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

class SymbolTable;
class SymbolList;
class Type;
class TypeList;

typedef std::list<RefPortugolAST> IDList;

typedef std::list<std::pair<std::string,Type*> > InitStructList;
typedef std::list<std::pair<int,Type*> >         InitMatrixList;

class BaseSemanticWalker : public antlr::TreeParser {

public:
  BaseSemanticWalker(SymbolTable*);

protected:
  void useLib(const std::string&);

  void declare(const IDList&, Type*, bool);

  void defineStruct(const std::string&, const SymbolList&, int);

  void declareProc(const std::string&, const SymbolList&, int, Type* = 0);

  Type* getType(const std::string&);
  Type* getType(int);

  Type* createAnonymousStructFor();

  Type* getSymbolType(const std::string&);

  Type* evalFCall(const std::string&, const TypeList&);

  void evalMatrixSubscript(Type*);

  Type* evalAttribution(Type*, Type*);
  Type* evalAttribution(Type*, const InitMatrixList&);
  Type* evalAttribution(Type*, const InitStructList&);

  Type* createAnonymousStructFor(const InitStructList&);

  Type* evalHomogeneity(const InitMatrixList& mtx);

  Type* evalExpr_OU(Type* left, Type* right);
  Type* evalExpr_E(Type* left, Type* right);
  Type* evalExpr_BIT_OU(Type* left, Type* right);
  Type* evalExpr_BIT_OUX(Type* left, Type* right);
  Type* evalExpr_BIT_E(Type* left, Type* right);
  Type* evalExpr_IGUAL(Type* left, Type* right);
  Type* evalExpr_DIFERENTE(Type* left, Type* right);
  Type* evalExpr_MAIOR(Type* left, Type* right);
  Type* evalExpr_MENOR(Type* left, Type* right);
  Type* evalExpr_MAIOR_EQ(Type* left, Type* right);
  Type* evalExpr_MENOR_EQ(Type* left, Type* right);
  Type* evalExpr_BIT_SHIFT_LEFT(Type* left, Type* right);
  Type* evalExpr_BIT_SHIFT_RIGHT(Type* left, Type* right);
  Type* evalExpr_MAIS(Type* left, Type* right);
  Type* evalExpr_MENOS(Type* left, Type* right);
  Type* evalExpr_DIV(Type* left, Type* right);
  Type* evalExpr_MULTIP(Type* left, Type* right);
  Type* evalExpr_MOD(Type* left, Type* right);
  Type* evalExpr_UN_NEGATIVO(Type*);
  Type* evalExpr_UN_POSITIVO(Type*);
  Type* evalExpr_NAO(Type*);
  Type* evalExpr_BIT_NAO(Type*);

  SymbolTable* _symtable;
};

#endif
