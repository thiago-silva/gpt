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

#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include "Symbol.hpp"

#include <map>
#include <list>
#include <string>

using namespace std;

class SymbolTableException {
public:
  SymbolTableException(const string& msg_) : msg(msg_) {}

  const string& getMessage() { return msg;}
  
  string msg;
};

class SymbolTable{
public:
  static string GlobalScope;//@global

  SymbolTable();
  ~SymbolTable();

  void declareVar(const string& scope, const string& lexeme, int line, int type);

  void declareVar(const string& scope, const string& lexeme, int line, int type
      , const list<int>& dimensions);

  void insertSymbol(Symbol& s, const string& scope);

  Symbol& getSymbol(const string& scope, const string& lexeme, bool searchGlobal = false);

  list<Symbol> getSymbols(const string& scope);

protected:
  void SymbolTable::registrarLeia();
  void SymbolTable::registrarImprima();

  int currentCod;
  map<string,list<Symbol> > symbols;//map<scope, symbols>
};

#endif
