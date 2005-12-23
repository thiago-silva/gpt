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
#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <string>
#include <list>

using namespace std;

enum {
  TIPO_NULO,    //void
  TIPO_INTEIRO,
  TIPO_REAL,
  TIPO_CARACTERE,
  TIPO_LITERAL,
  TIPO_LOGICO,    
  TIPO_ALL   //para funcoes *mix* que fazem cast automatico de primitivos (ex: leia())
};

class SymbolType {
public:
  SymbolType();
  SymbolType(int type);

  void setPrimitiveType(int type);
  int primitiveType() const;
  
  void setPrimitive(bool);
  bool isPrimitive() const;

  void setDimensions(const list<int>&);
  list<int>& dimensions();


  string toString() const;
protected:
  bool _isPrimitive;
  int _primitiveType;  
  list<int>    _dimensions; //conjunto/matrizASTRef  
};

#include <iostream>

class ParameterSig {
public:
  ParameterSig()
  : variable_params(false)  {}

  void add(int type) {
    SymbolType s(type);
    params.push_back(s); 
  }

  void add(pair<int, list<int> >& p) {
    SymbolType s(p.first);
    s.setPrimitive(false);
    s.setDimensions(p.second);
    params.push_back(s);
  }

  int paramType(int idx) {
    if(isVariable()) {
      return TIPO_ALL;
    }
  
    int c = 0;
    for(list<SymbolType>::iterator it = params.begin(); it != params.end(); it++) {
      if(c == idx) {
        return (*it).primitiveType();
      }
    }
    return TIPO_NULO; //throw exception?
  }

  list<SymbolType>& symbolList() { return params; }
  bool isVariable() { return variable_params;}
  void setVariable(bool val) { variable_params = val;}
protected:
  bool variable_params; //f(...)
  list<SymbolType> params;   //parâmetros de funcao    
};

class Symbol {  
public:
  Symbol();
  
  Symbol(const string& scope_, const string& lexeme_, int line_, bool isfunction_);

  Symbol(const string& scope_, const string& lexeme_, int line_, bool isfunction_, int type_);

  Symbol(const string& scope_, const string& lexeme_, int line_, bool isfunction_, int type_,
    const list<int>& dimensions);

  bool isValid() const;
     
  static string typeToString(int type);


  //attrs

  

  string scope;
  string lexeme;
  int line;
  
  SymbolType type;  

  
  bool isFunction;
  bool isBuiltin;
  
  ParameterSig param;
  
};

#endif
