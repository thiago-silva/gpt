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

#ifndef INTERPRETERHELPER_HPP
#define INTERPRETERHELPER_HPP

#include "Symbol.hpp"
#include "SymbolTable.hpp"
#include "Net.hpp"

#include <string>
#include <sstream>
#include <list>
#include <map>
#include <iostream>

using namespace std;

class ExprValue {
public:

  void setValue(string str);
  void setValue(const stringstream& s);
  bool ifTrue();

  string value;
  map<string, string> values; //map<keys, value>. ex: matrix["10:1"] == matrix[10][1]
  int type;
};

class Variable {
public:
  bool checkBounds(list<string>& d);

  string getValue(list<string>& d);

  void setValue(string value);
  void setValue(list<string>& d, string value);
  string castVal(string value);


  string name;
  int type;

  bool isPrimitive;
  string primitiveValue;


  map<string, string> values; //map<keys, value>. ex: matrix["10:1"] == matrix[10][1]
  list<int>    dimensions; //dim configuration
};

class LValue {
public:
  void addMatrixIndex(ExprValue& e);
  string dimsToString();

  string name;
  list<string> dims;//0,2,3 == X[0][2][3]
};


class Variables {
public:
  void setupGlobals();
  void pushState();
  void add(Variable& v);

  Variable& get(const string& name);

  void popState();

  map<string, Variable>& getLocals();

  map<string, Variable>& getGlobals();

private:
  typedef map<string, Variable> VariableState_t;

  list<VariableState_t> varstates;
  map<string, Variable> currentVars;//map<varname, Variable>
  map<string, Variable> globalVars;
};



//------------------------------------------------------------------------



class PrivateInterpreter {
  public:

  PrivateInterpreter(SymbolTable& st);
  
  void init();
 
  ExprValue evaluateOu(ExprValue& left, ExprValue& right);
  ExprValue evaluateE(ExprValue& left, ExprValue& right);
  ExprValue evaluateBitOu(ExprValue& left, ExprValue& right);
  ExprValue evaluateBitXou(ExprValue& left, ExprValue& right);
  ExprValue evaluateBitE(ExprValue& left, ExprValue& right);
  ExprValue evaluateIgual(ExprValue& left, ExprValue& right);
  ExprValue evaluateDif(ExprValue& left, ExprValue& right);
  ExprValue evaluateMaior(ExprValue& left, ExprValue& right);
  ExprValue evaluateMaiorEq(ExprValue& left, ExprValue& right);
  ExprValue evaluateMenor(ExprValue& left, ExprValue& right);
  ExprValue evaluateMenorEq(ExprValue& left, ExprValue& right);
  ExprValue evaluateMais(ExprValue& left, ExprValue& right);
  ExprValue evaluateMenos(ExprValue& left, ExprValue& right);
  ExprValue evaluateDiv(ExprValue& left, ExprValue& right);
  ExprValue evaluateMultip(ExprValue& left, ExprValue& right);
  ExprValue evaluateMod(ExprValue& left, ExprValue& right); 
  ExprValue evaluateUnNeg(ExprValue& v);
  ExprValue evaluateUnPos(ExprValue& v);
  ExprValue evaluateUnNot(ExprValue& v);
  ExprValue evaluateUnBNot(ExprValue& v);

  ExprValue getLValueValue(LValue& l);

  void execPasso(LValue& lvalue, int passo);
  bool execLowerEq(LValue& lv, ExprValue& ate);
  bool execBiggerEq(LValue& lv, ExprValue& ate);

  void execAttribution(LValue& lvalue, ExprValue& v);  

  void beginFunctionCall(const string& fname, list<ExprValue>& args, int line);
  void endFunctionCall();

  bool isBuiltInFunction(const string& fname);
  ExprValue execBuiltInFunction(const string& fname, list<ExprValue>& args);
 
  void setReturnExprValue(ExprValue& v);
  ExprValue getReturnExprValue();
//----------- Debugger -------------------------

  void nextCmd(int line);
  
private:
  string castValue(Variable& var, ExprValue& v);

  ExprValue executeLeia();

  void executeImprima(list<ExprValue>& args);

  Variables variables;
  SymbolTable& stable;

  list<pair<string, int> > program_stack;//pair<context, line>
  
  int currentLine;
  bool skipCmd;
  ExprValue retExpr;
};

#endif
