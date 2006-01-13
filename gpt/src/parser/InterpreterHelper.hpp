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

#include <string>
#include <sstream>
#include <list>
#include <map>
#include <stack>
#include <iostream>

using namespace std;


class ExprValue {
  public:
    void setValue(string str) {
      value = str;
    }
    void setValue(const stringstream& s) {
      value = s.str();
    }

    bool ifTrue() {
      if(type == TIPO_LITERAL) {
        return (value.length() > 0)?true:false;
      } else {
        return (atof(value.c_str()))?true:false;
      }
    }

  string value;
  map<string, string> values; //map<keys, value>. ex: matrix["10:1"] == matrix[10][1]
  int type;
};

class Variable {
  public:
  string name;
  int type;

  bool isPrimitive;
  string primitiveValue;

  bool checkBounds(list<string>& d) {
    list<int>::iterator it = dimensions.begin();
    list<string>::iterator ot = d.begin();
    
    //no need for dim.size() == d.size(). Semantic check wouldn't allow
    
    int num;
    for( ; it != dimensions.end(); ++it, ++ot) {      
      num = atoi((*ot).c_str());

      if((num < 0) || (num >= (*it))) {
        return false;
      }
    }
    return true;
  }

  string getValue(list<string>& d) {
    stringstream sub;
    string colon;
    for(list<string>::iterator it = d.begin(); it != d.end(); ++it) {
      sub << colon << *it;
      colon = ":";
    }
    return values[sub.str()];
  }

  void setValue(string value) {
    primitiveValue = castVal(value);
  }

  void setValue(list<string>& d, string value) {
    stringstream sub;
    string colon;
    for(list<string>::iterator it = d.begin(); it != d.end(); ++it) {
      sub << colon << *it;
      colon = ":";
    }

    values[sub.str()] = castVal(value);
  }

  string castVal(string value) {
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


  map<string, string> values; //map<keys, value>. ex: matrix["10:1"] == matrix[10][1]
  list<int>    dimensions; //dim configuration
};

class LValue {
  public:
    void addMatrixIndex(ExprValue& e) {
      dims.push_back(e.value);
    }        

  string dimsToString() {
    stringstream sub;
    for(list<string>::iterator it = dims.begin(); it != dims.end(); ++it) {
      sub << "[" << *it << "]";
    }    
    return sub.str();
  }

  string name;
  list<string> dims;//0,2,3 == X[0][2][3]
};


class Variables {
public:
  void setupGlobals() { //init glovals
    //currentVars must have all global vars at this point
    globalVars = currentVars;
  }

  void pushState() {
    varstates.push(currentVars);
    currentVars.clear();
  }

  void add(Variable& v) {
    currentVars[v.name] = v;
  }

  Variable& get(const string& name) {
    if(varstates.size() == 0) {
      return globalVars[name];
    }

    if(currentVars.find(name) == currentVars.end()) {
      if(globalVars.find(name) == globalVars.end()) {
        cerr << "BUG: variável " << name << " não encontrada." << endl;
        exit(1);
      } else {        
        return globalVars[name];
      }
    } else {
      return currentVars[name];
    }
  }

  void popState() {
    if(varstates.size() == 1) {
      currentVars = globalVars;
    } else {
      currentVars = varstates.top();      
    }
    varstates.pop();
  }

  map<string, Variable>& getLocals() {
    return currentVars;
  }

  map<string, Variable>& getGlobals() {
    return globalVars;
  }

private:
  typedef map<string, Variable> VariableState_t;

  stack<VariableState_t> varstates;
  map<string, Variable> currentVars;//map<varname, Variable>
  map<string, Variable> globalVars;
};


#include <sys/socket.h>
#include <netinet/in.h>
//#include <errno.h>
#include <netdb.h>
//#include <unistd.h> //close

class Net {
  public:
  Net() : clientsock(-1){}

  void init() {
    struct sockaddr_in name;

    char* hostname = "localhost";
    int port = 5555;   

    struct hostent *hostinfo = NULL;    
      
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    hostinfo = gethostbyname (hostname);
  
    if (hostinfo == NULL)
    {
      //cerr << "client: unknow host : "  << hostname << endl;
      return;
    }

    name.sin_addr = *(struct in_addr *) hostinfo->h_addr;    
    
    clientsock = socket(PF_INET, SOCK_STREAM, 0);
    if(clientsock < 0) {
      cerr << "client: could not create socket\n";
      return;
    }

    if(connect(clientsock, (struct sockaddr*) &name,sizeof(name)) != 0) { 
      cerr << "client: unable to connect" << endl;
      //unable to connect
      closeSock();
    }
  }

  void sendData(int line, Variables& v) {
    if(clientsock < 0) return;

    map<string, Variable> globals = v.getGlobals();

    stringstream s;
    s << "<data><line number=\"" << line << "\"/>";

    s << "<vars>";
    for(map<string, Variable>::iterator it = globals.begin(); it != globals.end(); ++it) {
      s << "<var name=\"" << (*it).second.name << "\" type=\"" << (*it).second.type
        << "\" value=\"" <<  (*it).second.primitiveValue << "\"/>";
    }
    s << "</vars></data>";

    if(send(clientsock, s.str().c_str(), sizeof(char)* (s.str().length()+1), 0) < 0) {//error
      cerr << "client:error sending data\n";
    }
  }

  void receiveCmd() {
    char buffer[1024];
    int received;

    if(clientsock < 0) return;

    received = recv(clientsock, buffer, sizeof(buffer), 0);
    if(received < 0) {
      cerr << "client: error receiving\n";
    }

    cerr << "client: received: " << received << ":\"" << buffer << "\"" << endl;
  }

  void signalNextCmd(int line, Variables& v) {
//     1: send stack info
//     2: send variables
//     3: wait for command
    sendData(line, v);
    receiveCmd();
  }

  void closeSock() {
    shutdown(clientsock, SHUT_RDWR);
    close(clientsock);
    clientsock = -1;
  }

private:
  int clientsock;
};



//------------------------------------------------------------------------



class PrivateInterpreter {
  public:

  PrivateInterpreter(SymbolTable& st) : stable(st), skipCmd(false) {}
  
  void init() {
    list<Symbol> globals = stable.getSymbols(SymbolTable::GlobalScope);

    for(list<Symbol>::iterator it = globals.begin(); it != globals.end(); ++it) {
      if((*it).isFunction) {
        continue;
      }
      Variable v;
      v.name = (*it).lexeme;
      v.type = (*it).type.primitiveType();
      v.isPrimitive = (*it).type.isPrimitive();
      v.dimensions = (*it).type.dimensions();
      variables.add(v);
    }

    variables.setupGlobals();
//     variables.pushState();

    net.init();
  }

  void execAttribution(LValue& lvalue, ExprValue& v) {
    //1: get the symbol
    //3: set the v.value to the variable in the current scope
    Variable& var = variables.get(lvalue.name);

    if(var.isPrimitive) { //primitive 
      var.setValue(v.value);
    } else {
      if(var.checkBounds(lvalue.dims)) {
        var.setValue(lvalue.dims, v.value);        
      } else {
        cerr << "Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lvalue.name 
             << lvalue.dimsToString() << "\". Abortando..." << endl;
        exit(1);
      }
    } 
  }

  void execPasso(LValue& lvalue, int passo) {
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
        cerr << "Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lvalue.name 
             << lvalue.dimsToString() << "\". Abortando..." << endl;
        exit(1);
      }
    }
  }

  ExprValue evaluateOu(ExprValue& left, ExprValue& right) {
    //1 || 1
    //"ou" || "eu"
    // "" || 'a'

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

  ExprValue evaluateE(ExprValue& left, ExprValue& right) {
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

  ExprValue  evaluateBitOu(ExprValue& left, ExprValue& right) {
    ExprValue v;
    v.type = TIPO_INTEIRO;

    stringstream s;
    s << (atoi(left.value.c_str()) | atoi(right.value.c_str()));
    v.setValue(s);
    return v;
  }

  ExprValue evaluateBitXou(ExprValue& left, ExprValue& right) {
    ExprValue v;
    v.type = TIPO_INTEIRO;

    stringstream s;
    s << (atoi(left.value.c_str()) ^ atoi(right.value.c_str()));
    v.setValue(s);
    return v;
  }

  ExprValue evaluateBitE(ExprValue& left, ExprValue& right) {
    ExprValue v;
    v.type = TIPO_INTEIRO;

    stringstream s;
    s << (atoi(left.value.c_str()) & atoi(right.value.c_str()));
    v.setValue(s);
    return v;
  }

  ExprValue evaluateIgual(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateDif(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateMaior(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateMaiorEq(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateMenor(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateMenorEq(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateMais(ExprValue& left, ExprValue& right) {
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
    
  ExprValue evaluateMenos(ExprValue& left, ExprValue& right) {
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


  ExprValue evaluateDiv(ExprValue& left, ExprValue& right) {
    ExprValue v;

    if(atof(right.value.c_str()) == 0) {
      cerr << "Erro de execução próximo a linha " << currentLine 
           << " - Divisão por 0 é ilegal. Abortando..." << endl;
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

  ExprValue evaluateMultip(ExprValue& left, ExprValue& right) {
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

  ExprValue evaluateMod(ExprValue& left, ExprValue& right) {
    ExprValue v;

    stringstream s;

    s << (atoi(left.value.c_str()) % atoi(right.value.c_str()));
    v.type = TIPO_INTEIRO;

    v.setValue(s);
    return v;
  }

 
  ExprValue evaluateUnNeg(ExprValue& v) {
    stringstream s;
    s << -(atof(v.value.c_str()));
        
    v.setValue(s);
    return v;
  }

  ExprValue evaluateUnPos(ExprValue& v) {
    stringstream s;
    s << +(atof(v.value.c_str()));
    v.setValue(s);
    return v;
  }

  ExprValue evaluateUnNot(ExprValue& v) {
    v.type = TIPO_LOGICO;

    if(v.type == TIPO_LITERAL) {
      v.value = (v.value.length()>0)?"0":"1";
    } else {
      v.value = atof(v.value.c_str())?"0":"1";
    }

    return v;
  }

  ExprValue evaluateUnBNot(ExprValue& v) {
    stringstream s;
    s << ~(atoi(v.value.c_str()));
    v.setValue(s);
    return v;
  }


  ExprValue getLValueValue(LValue& l) {
    ExprValue value;

    Variable var = variables.get(l.name);
    value.type = var.type;
    if(var.isPrimitive) {
      value.value = var.primitiveValue;
    } else {
      if(var.checkBounds(l.dims)) {
        value.value = var.getValue(l.dims);
        value.values = var.values;
      } else {
        cerr << "Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << l.name 
             << l.dimsToString() << "\". Abortando..." << endl;
        exit(1);
      }
    }
    return value;
  }

  bool execLowerEq(LValue& lv, ExprValue& ate) {
    Variable var = variables.get(lv.name);

    if(var.isPrimitive) {
      return atoi(var.primitiveValue.c_str()) <= atoi(ate.value.c_str());
    } else {
      if(var.checkBounds(lv.dims)) {
        string val = var.getValue(lv.dims);
        return atoi(val.c_str()) <= atoi(ate.value.c_str());
      } else {
        cerr << "Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lv.name 
             << lv.dimsToString() << "\". Abortando..." << endl;
        exit(1);
      }
    }
  }

  bool execBiggerEq(LValue& lv, ExprValue& ate) {
    Variable var = variables.get(lv.name);

    if(var.isPrimitive) {
      return atoi(var.primitiveValue.c_str()) >= atoi(ate.value.c_str());
    } else {
      if(var.checkBounds(lv.dims)) {
        string val = var.getValue(lv.dims);
        return atoi(val.c_str()) <= atoi(ate.value.c_str());
      } else {
        cerr << "Erro de execução próximo a linha " << currentLine << " - Overflow em \"" << lv.name 
             << lv.dimsToString() << "\". Abortando..." << endl;
        exit(1);
      }
    }
  }

  void initFCall(const string& fname, list<ExprValue>& args) {
    variables.pushState();

    //setup local vars

    list<Symbol> globals = stable.getSymbols(fname);

    for(list<Symbol>::iterator it = globals.begin(); it != globals.end(); ++it) {
      Variable v;
      v.name = (*it).lexeme;
      v.type = (*it).type.primitiveType();
      v.isPrimitive = (*it).type.isPrimitive();
      v.dimensions =  (*it).type.dimensions();
      variables.add(v);
    }

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
  }
  
  void finishLastFCall() {
    variables.popState();
  }

  bool isBuiltInFunction(const string& fname) {
    return stable.getSymbol(SymbolTable::GlobalScope, fname).isBuiltin;    
  }

  ExprValue execBuiltInFunction(const string& fname, list<ExprValue>& args) {
    ExprValue v;
    if(fname == "leia") {
      return executeLeia();
    } else if(fname == "imprima") {
      executeImprima(args);
      return v;//empty value
    } else {
      cerr << "BUG: No built-in function called \"" << fname << "\"" << endl;
      return v;
    }
  }

//----------- Debugger -------------------------

  void nextCmd(int line) {
//     cerr << "line: " << line << endl;
    //notes: 
    //    mat[leia()] := f() + f2(); //is a single step
    //   se f() então // single step

    currentLine = line;
    if(!skipCmd) {
      net.signalNextCmd(line, variables);
    }
  }

  void beginFCall() {
//     if(net.currentCmd == CMD_STEP_INTO) {
//       skipCmd = false;
//     } else {
//       skipCmd = true;
//     }
  }

  void endFCall() {
    skipCmd = false;
  }

  private:
    ExprValue executeLeia() {
      ExprValue ret;
      cin >> ret.value;
      return ret;
    }

    void executeImprima(list<ExprValue>& args) {      
      for(list<ExprValue>::iterator it = args.begin(); it != args.end(); ++it) {
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
              cout << "verdadeiro";
            } else {
              cout << "falso";
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
    }

    Variables variables;
//     typedef map<string, Variable> VariableState_t;
//     stack<VariableState_t> varstates;
//     map<string, Variable> currentVars;//map<varname, Variable>
    SymbolTable& stable;

  bool skipCmd;
  Net net;
  int currentLine;
};

#endif
