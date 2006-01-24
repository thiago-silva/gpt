/***************************************************************************
 *   Copyright (C) 2005 by Thiago Silva                                    *
 *   thiago.silva@kdemal.net                                               *
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


#include "SymbolTable.hpp"
#include <iostream>

string SymbolTable::GlobalScope = "@global";

SymbolTable::SymbolTable()
  : currentCod(0)
{
  //builtin functions
  registrarLeia();
  registrarImprima();
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::registrarLeia() {
  Symbol f(SymbolTable::GlobalScope, "leia", 0, true, TIPO_ALL);
  f.cd = currentCod++;
  f.isBuiltin = true;
  insertSymbol(f, SymbolTable::GlobalScope);
}

void SymbolTable::registrarImprima() {
  Symbol f(SymbolTable::GlobalScope, "imprima", 0, true, TIPO_NULO);
  f.cd = currentCod++;
  f.param.setVariable(true);
  f.isBuiltin = true;
  insertSymbol(f, SymbolTable::GlobalScope);  
}

void SymbolTable::declareVar(const string& scope, const string& lexeme, int line, int type) {
  Symbol s(scope, lexeme, line, false, type);
  s.cd = currentCod++;
  symbols[scope].push_back(s);
}

void SymbolTable::declareVar(const string& scope, const string& lexeme, int line, int type
      , const list<int>& dimensions) {

  Symbol s(scope, lexeme, line, false, type, dimensions);
  s.cd = currentCod++;
  symbols[scope].push_back(s);  
}


void SymbolTable::insertSymbol(Symbol& s, const string& scope) {
  s.cd = currentCod++;
  symbols[scope].push_back(s);
}

Symbol& SymbolTable::getSymbol(const string& scope, const string& lexeme, bool searchGlobal) {

  list<Symbol>::iterator it;
  list<Symbol>::iterator end = symbols[scope].end();

  for(it = symbols[scope].begin(); it != end; ++it) {
    if((*it).lexeme == lexeme) {
      return (*it);
    }
  }

  end = symbols[SymbolTable::GlobalScope].end();

  if(searchGlobal) {
    for(it = symbols[SymbolTable::GlobalScope].begin(); it != end; ++it) {
      if((*it).lexeme == lexeme) {
        return (*it);
      }
    }
  }

  throw SymbolTableException("no symbol found");
}

list<Symbol> SymbolTable::getSymbols(const string& scope) {
  return symbols[scope];
}
