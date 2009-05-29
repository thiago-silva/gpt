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

#ifndef INTERPRETERHELPER_HPP
#define INTERPRETERHELPER_HPP

#include "Symbol.hpp"
#include "SymbolTable.hpp"

#include <string>
#include <sstream>
#include <list>
#include <stack>
#include <map>
#include <iostream>
#include <stdlib.h>

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
  void init(map<string, Variable>&);

  void pushLocalContext(map<string, Variable>&);
  void add(Variable& v);

  Variable& get(const string& name);

  void popContext();

  map<string, Variable>& getLocals();

  map<string, Variable>& getGlobals();

private:
  typedef map<string, Variable> VariableState_t;

  list<VariableState_t*> varstates;
  map<string, Variable> *currentVars;//map<varname, Variable>
  map<string, Variable> *globalVars;
};



//------------------------------------------------------------------------



class InterpreterEval {
  public:

  InterpreterEval(SymbolTable& st, string host, int port);
  
  void init(const string&);
 
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

  void beginFunctionCall(const string& file, const string& fname, list<ExprValue>& args, int line);
  void endFunctionCall();

  bool isBuiltInFunction(const string& fname);
  ExprValue execBuiltInFunction(const string& fname, list<ExprValue>& args);
 
  void setReturnExprValue(ExprValue& v);
  ExprValue getReturnExprValue(const string&);
  
  int getReturning();
  
//----------- Debugger -------------------------

  void nextCmd(const string& file, int line);
  
private:
  string castLeiaChar(Variable& var, ExprValue& v);

  ExprValue executeLeia();

  void executeImprima(list<ExprValue>& args);

  
  SymbolTable& stable;
  string dbg_host;
  int dbg_port;
  int currentLine;

  bool currentSkip;
  bool globalSkip;

  stack<bool> skipStack;

  Variables variables;  
  typedef pair<string, int> context_t;
  typedef pair<string, pair<string, int> > stack_entry_t; //pair<file, pair<context, line> >
  list< stack_entry_t > program_stack;

  ExprValue retExpr;
};

#endif
