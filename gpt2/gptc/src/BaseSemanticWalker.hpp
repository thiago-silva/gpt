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

class PortugolAST;
class SymbolTable;
class SymbolList;
class Type;
class TypeList;

typedef std::list<PortugolAST*> IDList;

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

  Type* getSymbolType(const std::string&);

  void evalFCall(const std::string&, const TypeList&);

  void evalMatrixSubscript(Type*);

  void evalAttribution(Type*, Type*);

  template<int>
  Type* evalExpr(Type*, Type*);

  template<int>
  Type* evalExpr(Type*);

  SymbolTable* _symtable;
};

#endif