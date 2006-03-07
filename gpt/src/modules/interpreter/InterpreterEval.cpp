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

#include "config.h"

#include "InterpreterEval.hpp"
#include "InterpreterDBG.hpp"
#include "GPTDisplay.hpp"

void ExprValue::setValue(string str) {
  value = str;
}
void ExprValue::setValue(const stringstream& s) {
  value = s.str();
}

bool ExprValue::ifTrue() {
  if(type == TIPO_LITERAL) {
    return (value.length() > 0)?true:false;
  } else {
    return (atof(value.c_str()))?true:false;
  }
}

//-------------------------------------------------------------------------------



bool Variable::checkBounds(list<string>& d) {
  list<int>::iterator it = dimensions.begin();
  list<string>::iterator ot = d.begin();

  int num;
  for( ; it != dimensions.end(); ++it, ++ot) {
    num = atoi((*ot).c_str());

    if((num < 0) || (num >= (*it))) {
      return false;
    }
  }
  return true;
}

string Variable::getValue(list<string>& d) {
  stringstream sub;
  string colon;
  for(list<string>::iterator it = d.begin(); it != d.end(); ++it) {
    sub << colon << *it;
    colon = ":";
  }
  return values[sub.str()];
}

void Variable::setValue(string value) {
  primitiveValue = castVal(value);
}

void Variable::setValue(list<string>& d, string value) {
  stringstream sub;
  string colon;
  for(list<string>::iterator it = d.begin(); it != d.end(); ++it) {
    sub << colon << *it;
    colon = ":";
  }

  values[sub.str()] = castVal(value);
}

string Variable::castVal(string value) {
  stringstream ss;
  switch(type) {
    case TIPO_INTEIRO:
      ss << atoi(value.c_str());
      return ss.str();
    case TIPO_LOGICO:
      if((value.length() == 0)||(value == "falso") || (value == "0")) {
        return "0";
      } else {
        return "1";
      }
    default:
      return value;
  }
}


//------------------------------------------------------------------------



void LValue::addMatrixIndex(ExprValue& e) {
  dims.push_back(e.value);
}

string LValue::dimsToString() {
  stringstream sub;
  for(list<string>::iterator it = dims.begin(); it != dims.end(); ++it) {
    sub << "[" << *it << "]";
  }
  return sub.str();
}



//------------------------------------------------------------------------



void Variables::init(map<string, Variable>& vars) {
  currentVars = new map<string, Variable>;
  *currentVars = vars;
  globalVars = currentVars;
}

void Variables::pushLocalContext(map<string, Variable>& vars) {
  varstates.push_back(currentVars);
  currentVars = new map<string, Variable>;
  *currentVars = vars;
}

Variable& Variables::get(const string& name) {
  stringstream s;
  if(currentVars->find(name) == currentVars->end()) {
    if(globalVars->find(name) == globalVars->end()) {
      stringstream s;
      s << "BUG: variável " << name << " não encontrada." << endl;
      GPTDisplay::self()->showError(s);
      exit(1);
    } else {
      return (*globalVars)[name];
    }
  } else {
    return (*currentVars)[name];
  }
}

void Variables::popContext() {
  delete currentVars;
  currentVars = varstates.back();
  varstates.pop_back();
}

map<string, Variable>& Variables::getLocals() {
  return *currentVars;
}

map<string, Variable>& Variables::getGlobals() {
  return *globalVars;
}



//------------------------------------------------------------------------

InterpreterEval::InterpreterEval(SymbolTable& st, string host, int port)
  : stable(st), dbg_host(host), dbg_port(port), currentLine(-1)
    , currentSkip(false), globalSkip(false)
{
  skipStack.push(false);
}

void InterpreterEval::init() {
  list<Symbol> globals = stable.getSymbols(SymbolTable::GlobalScope);

  map<string, Variable> vars;
  for(list<Symbol>::iterator it = globals.begin(); it != globals.end(); ++it) {
    if((*it).isFunction) {
      continue;
    }
    Variable v;
    v.name = (*it).lexeme;
    v.type = (*it).type.primitiveType();
    v.isPrimitive = (*it).type.isPrimitive();
    v.dimensions = (*it).type.dimensions();

    if(v.isPrimitive && (v.type != TIPO_LITERAL)) {
      v.primitiveValue = "0";
    }
    vars[v.name] = v;
  }

  variables.init(vars);
  program_stack.push_back(pair<string,int>(SymbolTable::GlobalScope, 0));

  InterpreterDBG::self()->init(dbg_host, dbg_port);
}



ExprValue InterpreterEval::evaluateOu(ExprValue& left, ExprValue& right) {


  ExprValue v;
  v.type = TIPO_LOGICO;

  bool l,r;
  if(left.type == TIPO_LITERAL) {
    l = (left.value.length() != 0)?true:false;
  } else {
    l = atof(left.value.c_str())?true:false;
  }

  if(right.type == TIPO_LITERAL) {
    l = (right.value.length() != 0)?true:false;
  } else {
    l = atof(right.value.c_str())?true:false;
  }

  stringstream s;
  s << (l || r);
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateE(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool l,r;
  if(left.type == TIPO_LITERAL) {
    l = (left.value.length() != 0)?true:false;
  } else {
    l = atof(left.value.c_str())?true:false;
  }

  if(right.type == TIPO_LITERAL) {
    l = (right.value.length() != 0)?true:false;
  } else {
    l = atof(right.value.c_str())?true:false;
  }


  stringstream s;
  s << (l && r);
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateBitOu(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_INTEIRO;

  stringstream s;
  s << (atoi(left.value.c_str()) | atoi(right.value.c_str()));
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateBitXou(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_INTEIRO;

  stringstream s;
  s << (atoi(left.value.c_str()) ^ atoi(right.value.c_str()));
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateBitE(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_INTEIRO;

  stringstream s;
  s << (atoi(left.value.c_str()) & atoi(right.value.c_str()));
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateIgual(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool res;

  if((left.type == TIPO_LITERAL) || (right.type == TIPO_LITERAL)) {
    res = left.value == right.value;
  } else {
    if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
      res = atof(left.value.c_str()) == atof(right.value.c_str());//"0" == "0.000"
    } else { //caractere, inteiro, logico
      res = atoi(left.value.c_str()) == atoi(right.value.c_str());
    }
  }

  stringstream s;
  s << res;
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateDif(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool res;

  if((left.type == TIPO_LITERAL) || (right.type == TIPO_LITERAL)) {
    res = left.value != right.value;
  } else {
    if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
      res = atof(left.value.c_str()) != atof(right.value.c_str());//"0" == "0.000"
    } else { //caractere, inteiro, logico
      res = atoi(left.value.c_str()) != atoi(right.value.c_str());
    }
  }

  stringstream s;
  s << res;
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMaior(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool res;

  if((left.type == TIPO_LITERAL) || (right.type == TIPO_LITERAL)) {
    res = left.value.length() > right.value.length();
  } else {
    if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
      res = atof(left.value.c_str()) > atof(right.value.c_str());//"0" == "0.000"
    } else { //caractere, inteiro, logico
      res = atoi(left.value.c_str()) > atoi(right.value.c_str());
    }
  }

  stringstream s;
  s << res;
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMaiorEq(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool res;

  if((left.type == TIPO_LITERAL) || (right.type == TIPO_LITERAL)) {
    res = left.value.length() >= right.value.length();
  } else {
    if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
      res = atof(left.value.c_str()) >= atof(right.value.c_str());//"0" == "0.000"
    } else { //caractere, inteiro, logico
      res = atoi(left.value.c_str()) >= atoi(right.value.c_str());
    }
  }

  stringstream s;
  s << res;
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMenor(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool res;

  if((left.type == TIPO_LITERAL) || (right.type == TIPO_LITERAL)) {
    res = left.value.length() < right.value.length();
  } else {
    if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
      res = atof(left.value.c_str()) < atof(right.value.c_str());//"0" == "0.000"
    } else { //caractere, inteiro, logico
      res = atoi(left.value.c_str()) < atoi(right.value.c_str());
    }
  }

  stringstream s;
  s << res;
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMenorEq(ExprValue& left, ExprValue& right) {
  ExprValue v;
  v.type = TIPO_LOGICO;

  bool res;

  if((left.type == TIPO_LITERAL) || (right.type == TIPO_LITERAL)) {
    res = left.value.length() <= right.value.length();
  } else {
    if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
      res = atof(left.value.c_str()) <= atof(right.value.c_str());//"0" == "0.000"
    } else { //caractere, inteiro, logico
      res = atoi(left.value.c_str()) <= atoi(right.value.c_str());
    }
  }

  stringstream s;
  s << res;
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMais(ExprValue& left, ExprValue& right) {
  ExprValue v;

  stringstream s;
  if((left.type == TIPO_REAL) || (right.type == TIPO_REAL)) {
    s << (atof(left.value.c_str()) + atof(right.value.c_str()));
    v.type = TIPO_REAL;
  } else {
    s << (atoi(left.value.c_str()) + atoi(right.value.c_str()));
    v.type = TIPO_INTEIRO;
  }

  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMenos(ExprValue& left, ExprValue& right) {
  ExprValue v;

  stringstream s;
  if((left.type == TIPO_REAL) || (left.type == TIPO_REAL)) {
    s << (atof(left.value.c_str()) - atof(right.value.c_str()));
    v.type = TIPO_REAL;
  } else {
    s << (atoi(left.value.c_str()) - atoi(right.value.c_str()));
    v.type = TIPO_INTEIRO;
  }

  v.setValue(s);
  return v;
}


ExprValue InterpreterEval::evaluateDiv(ExprValue& left, ExprValue& right) {
  ExprValue v;

  if(atof(right.value.c_str()) == 0) {
    stringstream s;
    s << PACKAGE << ": Erro de execução próximo a linha " << currentLine
        << " - Divisão por 0 é ilegal. Abortando..." << endl;
    GPTDisplay::self()->showError(s);
    exit(1);
  }

  stringstream s;
  if((left.type == TIPO_REAL) || (left.type == TIPO_REAL)) {
    s << (atof(left.value.c_str()) / atof(right.value.c_str()));
    v.type = TIPO_REAL;
  } else {
    s << (atoi(left.value.c_str()) / atoi(right.value.c_str()));
    v.type = TIPO_INTEIRO;
  }

  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMultip(ExprValue& left, ExprValue& right) {
  ExprValue v;

  stringstream s;
  if((left.type == TIPO_REAL) || (left.type == TIPO_REAL)) {
    s << (atof(left.value.c_str()) * atof(right.value.c_str()));
    v.type = TIPO_REAL;
  } else {
    s << (atoi(left.value.c_str()) * atoi(right.value.c_str()));
    v.type = TIPO_INTEIRO;
  }

  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateMod(ExprValue& left, ExprValue& right) {
  ExprValue v;

  stringstream s;

  s << (atoi(left.value.c_str()) % atoi(right.value.c_str()));
  v.type = TIPO_INTEIRO;

  v.setValue(s);
  return v;
}


ExprValue InterpreterEval::evaluateUnNeg(ExprValue& v) {
  stringstream s;
  s << -(atof(v.value.c_str()));

  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateUnPos(ExprValue& v) {
  stringstream s;
  s << +(atof(v.value.c_str()));
  v.setValue(s);
  return v;
}

ExprValue InterpreterEval::evaluateUnNot(ExprValue& v) {
  v.type = TIPO_LOGICO;

  if(v.type == TIPO_LITERAL) {
    v.value = (v.value.length()>0)?"0":"1";
  } else {
    v.value = atof(v.value.c_str())?"0":"1";
  }

  return v;
}

ExprValue InterpreterEval::evaluateUnBNot(ExprValue& v) {
  stringstream s;
  s << ~(atoi(v.value.c_str()));
  v.setValue(s);
  return v;
}


ExprValue InterpreterEval::getLValueValue(LValue& l) {
  ExprValue value;

  Variable var = variables.get(l.name);
  value.type = var.type;
  if(var.isPrimitive) {
    value.value = var.primitiveValue;
  } else {
    if(l.dims.size()) { //if mat[x][x][x]...
      if(var.checkBounds(l.dims)) {
        value.value = var.getValue(l.dims);
//         value.values = var.values;
      } else {
        stringstream s;
        s << PACKAGE << ": Erro de execução próximo a linha " << currentLine
            << " - Overflow em \"" << l.name
            << l.dimsToString() << "\". Abortando..." << endl;
        GPTDisplay::self()->showError(s);
        exit(1);
      }
    } else { //if func(mat)
      value.values = var.values;
    }
  }
  return value;
}

void InterpreterEval::execPasso(LValue& lvalue, int passo) {
  //1: get the symbol
  //3: set the v.value to the variable in the current scope
  Variable& var = variables.get(lvalue.name);

  stringstream s;
  if(var.isPrimitive) {
    s << (atoi(var.primitiveValue.c_str()) + passo);
    var.primitiveValue = s.str();
  } else {
    if(var.checkBounds(lvalue.dims)) {
      string val = var.getValue(lvalue.dims);
      s << (atoi(val.c_str()) + passo);
      var.setValue(lvalue.dims, s.str());
    } else {
      stringstream err;
      err << PACKAGE << ": Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lvalue.name
          << lvalue.dimsToString() << "\". Abortando..." << endl;
      GPTDisplay::self()->showError(err);
      exit(1);
    }
  }
}

bool InterpreterEval::execLowerEq(LValue& lv, ExprValue& ate) {
  Variable var = variables.get(lv.name);

  if(var.isPrimitive) {
    return atoi(var.primitiveValue.c_str()) <= atoi(ate.value.c_str());
  } else {
    if(var.checkBounds(lv.dims)) {
      string val = var.getValue(lv.dims);
      return atoi(val.c_str()) <= atoi(ate.value.c_str());
    } else {
      stringstream s;
      s << PACKAGE << ": Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lv.name
          << lv.dimsToString() << "\". Abortando..." << endl;
      GPTDisplay::self()->showError(s);
      exit(1);
    }
  }
}

bool InterpreterEval::execBiggerEq(LValue& lv, ExprValue& ate) {
  Variable var = variables.get(lv.name);

  if(var.isPrimitive) {
    return atoi(var.primitiveValue.c_str()) >= atoi(ate.value.c_str());
  } else {
    if(var.checkBounds(lv.dims)) {
      string val = var.getValue(lv.dims);
      return atoi(val.c_str()) <= atoi(ate.value.c_str());
    } else {
      stringstream s;
      s << PACKAGE << ": Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lv.name
          << lv.dimsToString() << "\". Abortando..." << endl;
      GPTDisplay::self()->showError(s);
      exit(1);
    }
  }
}

void InterpreterEval::execAttribution(LValue& lvalue, ExprValue& v) {
  Variable& var = variables.get(lvalue.name);

  if(var.isPrimitive) {
    var.setValue(castLeiaChar(var, v));
  } else {
    if(var.checkBounds(lvalue.dims)) {
      var.setValue(lvalue.dims, v.value);
    } else {
      stringstream s;
      s << PACKAGE << ": Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lvalue.name
          << lvalue.dimsToString() << "\". Abortando..." << endl;
      GPTDisplay::self()->showError(s);
      exit(1);
    }
  }
}

string InterpreterEval::castLeiaChar(Variable& var, ExprValue& v) {
  if((var.type == TIPO_CARACTERE) && (v.type == TIPO_LITERAL)) {

    /*************************
      caractere c := leia();

      se entrar com "1", var.value == 49

      se entrar com "abc"? opces:
        1: var.value = 0
          manter uniformidade com por ex:
            "inteiro i = leia(); //"abc", resultado: i == 0 (interpretado), i=1243249 (compilado/C)
        2: var.value = 'a'
          eh o que acontece em modo compilado/C (scanf(%c))
    *******************************/

/*    if(v.value.length() > 1) {
      return "0";
    }*/

    stringstream s;
    s << (int)v.value[0];
    return s.str();
  } else {
    return v.value;
  }
}

void InterpreterEval::beginFunctionCall(const string& fname, list<ExprValue>& args, int line) {
  //setup local vars

  list<Symbol> globals = stable.getSymbols(fname);

  map<string, Variable> vars;
  for(list<Symbol>::iterator it = globals.begin(); it != globals.end(); ++it) {
    Variable v;
    v.name = (*it).lexeme;
    v.type = (*it).type.primitiveType();
    v.isPrimitive = (*it).type.isPrimitive();
    v.dimensions =  (*it).type.dimensions();

    if(v.isPrimitive && (v.type != TIPO_LITERAL)) {
      v.primitiveValue = "0";
    }

    vars[v.name] = v;
  }
  variables.pushLocalContext(vars);

  //init params
  Symbol func = stable.getSymbol(SymbolTable::GlobalScope,fname);
  list< pair<string,SymbolType> >& params = func.param.symbolList();

  list< pair<string,SymbolType> >::iterator pit = params.begin();
  list<ExprValue>::iterator ait = args.begin();

  while((ait != args.end()) && (pit != params.end())) {
    Symbol pv = stable.getSymbol(fname, (*pit).first);
    Variable& var = variables.get(pv.lexeme);
    if(var.isPrimitive) {
      var.primitiveValue = (*ait).value;
    } else {
      var.values = (*ait).values;
    }

    ++ait;
    ++pit;
  }

  program_stack.push_back(pair<string, int>(fname, line));

  skipStack.push(currentSkip);
}

void InterpreterEval::endFunctionCall() {
  variables.popContext();
  program_stack.pop_back();

  skipStack.pop();
}

bool InterpreterEval::isBuiltInFunction(const string& fname) {
  return stable.getSymbol(SymbolTable::GlobalScope, fname).isBuiltin;
}

ExprValue InterpreterEval::execBuiltInFunction(const string& fname, list<ExprValue>& args) {
  ExprValue v;
  if(fname == "leia") {
    return executeLeia();
  } else if(fname == "imprima") {
    executeImprima(args);
    return v;//empty value
  } else {
    stringstream s;
    s << PACKAGE << ":BUG: No built-in function called \"" << fname << "\"" << endl;
    GPTDisplay::self()->showError(s);
    return v;
  }
}

void InterpreterEval::setReturnExprValue(ExprValue& v) {
  retExpr = v;
}

ExprValue InterpreterEval::getReturnExprValue() {
  return retExpr;
}

//----------- Debugger -------------------------

void InterpreterEval::nextCmd(int line) {
  program_stack.back().second = line;

  currentLine = line;

  if((!skipStack.top() && !globalSkip) || InterpreterDBG::self()->breakOnLine(line)) {
    stringstream s;
    int cmd;
    cmd = InterpreterDBG::self()->nextCmd(currentLine, variables, program_stack);
    switch(cmd) {
      case InterpreterDBG::CMDStepInto:
        currentSkip = false;
        break;
      case InterpreterDBG::CMDStepOver:
        currentSkip = true;
        break;
      case InterpreterDBG::CMDStepOut:
        skipStack.pop();
        skipStack.push(true);
        break;
      case InterpreterDBG::CMDContinue:
        globalSkip = true;
        break;
      default:
        s << PACKAGE << ": BUG: unknown cmd." << endl;
        GPTDisplay::self()->showError(s);
        break;
    }
  }
}

//private

ExprValue InterpreterEval::executeLeia() {
  ExprValue ret;
  ret.type = TIPO_LITERAL;
//   cin >> ret.value;
  std::getline(cin, ret.value);
//   ret.value.getline();
  return ret;
}

void InterpreterEval::executeImprima(list<ExprValue>& args) {
  ios_base::fmtflags old = cout.flags(ios_base::fixed);
  int oldp = cout.precision(2);

  stringstream s;
  for(list<ExprValue>::iterator it = args.begin(); it != args.end(); ++it) {
    ExprValue ss = (*it);
    switch((*it).type) {
      case TIPO_INTEIRO:
        cout << (int) atoi((*it).value.c_str());
        break;
      case TIPO_REAL:        
        cout << (float) atof((*it).value.c_str());
        break;
      case TIPO_CARACTERE:
        cout << (char) atoi((*it).value.c_str());
        break;
      case TIPO_LOGICO:
        if(atoi((*it).value.c_str())) {
          s << "verdadeiro";
        } else {
          s << "falso";
        }
        break;
      case TIPO_LITERAL:
        cout << (*it).value;
        break;
      default:
        cout << (*it).value;
    }
  }
  cout << endl;
  cout.flush();

  cout.flags(old);
  cout.precision(oldp);
}

// void InterpreterEval::parseLiteral(string& lit) {
//
// }
