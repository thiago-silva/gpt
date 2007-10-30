/***************************************************************************
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
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

#include "Symbol.hpp"

#include <sstream>

SymbolType::SymbolType() 
  : _isPrimitive(true)
  , _primitiveType(TIPO_NULO)
  {}
  
SymbolType::SymbolType(int type) 
  : _isPrimitive(true), _primitiveType(type)
  {
}

void SymbolType::setPrimitiveType(int type) { 
  _primitiveType=type;
  _isPrimitive=true;
  _dimensions.clear();
}

int SymbolType::primitiveType() const {
  return _primitiveType;
}
  
void SymbolType::setPrimitive(bool val) {
  _isPrimitive = val;
}

bool SymbolType::isPrimitive() const {
  return _isPrimitive;
}

void SymbolType::setDimensions(const list<int>& d) {
  _isPrimitive = false;
  _dimensions = d;
}

list<int>& SymbolType::dimensions() {
  return _dimensions;
}

string SymbolType::toString() const {
  stringstream str;
//   int c = _dimensions.size();
  str << Symbol::typeToString(_primitiveType);
  if(!_isPrimitive) {    
    list<int>::const_iterator it;
    for(it = _dimensions.begin(); it != _dimensions.end(); ++it) {
      str << "[" << *it << "]";
      
    }
  }
  return str.str();
}

//------------------------------------------------------------------------------------//

Symbol::Symbol() 
  : cd(-1), scope(), lexeme(), line(-1), type(TIPO_NULO), isFunction(false), isBuiltin(false)
  , param()
{

}

Symbol::Symbol(const string& scope_, const string& lexeme_, int line_, bool isfunction_) 
  : cd(-1), scope(scope_), lexeme(lexeme_), line(line_)
  , type(TIPO_NULO), isFunction(isfunction_), isBuiltin(false)
  , param() {}


Symbol::Symbol(const string& scope_, const string& lexeme_, int line_, bool isfunction_, int type_) 
    : cd(-1), scope(scope_), lexeme(lexeme_), line(line_)
    , type(), isFunction(isfunction_), isBuiltin(false)
    , param() {

  type.setPrimitive(true);
  type.setPrimitiveType(type_);
}

Symbol::Symbol(const string& scope_, const string& lexeme_, int line_, bool isfunction_, int type_,
  const list<int>& dimensions) 
    : cd(-1), scope(scope_)
    , lexeme(lexeme_)
    , line(line_) 
    , type()
    , isFunction(isfunction_) 
    , isBuiltin(false) {    

  type.setPrimitiveType(type_);
  type.setDimensions(dimensions);
  if(type.dimensions().size()) {
    type.setPrimitive(false);
  } else {
    type.setPrimitive(true);
  }
}

bool Symbol::isValid() const {
  return lexeme.length() > 0;
}

string Symbol::typeToString(int type) {
  string str;
  switch(type) {
    case TIPO_NULO:      str = "nulo"; break;
    case TIPO_INTEIRO:   str = "inteiro"; break;
    case TIPO_REAL:      str = "real"; break;
    case TIPO_CARACTERE: str = "caractere"; break;
    case TIPO_LITERAL:   str = "literal"; break;
    case TIPO_LOGICO:    str = "lógico"; break;    
    case TIPO_ALL:       str = "@all"; break;
    default:             str = "desconhecido"; break;
  }
  return str;
}
