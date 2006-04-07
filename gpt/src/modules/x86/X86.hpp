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

#ifndef X86_HPP
#define X86_HPP

#include "SymbolTable.hpp"
#include <string>
#include <sstream>
#include <list>
#include <map>

using namespace std;

class X86SubProgram {
public:
  X86SubProgram();
  X86SubProgram(const X86SubProgram&);
  ~X86SubProgram();

  void declareLocal(const string&, int = 0, bool minit = true);
  void declareParam(const string&, int type, int = 0);

  void writeTEXT(const string&);

  void init(const string&, int = 0);
  string name();

  string source();

private:
  void writeMatrixInitCode(const string& varname, int size);
  void writeMatrixCopyCode(const string& param, int type, int msize);

  const  int   SizeofDWord;
  int          _param_offset;
  int          _local_offset;
  string       _name;
  list<string> _params; 
  list<string> _locals;

  stringstream _head; //%definitions
  stringstream _init; //init commands
  stringstream _txt;  //body
};




class X86 {
public:
  enum {
    VAR_GLOBAL,
    VAR_PARAM,
    VAR_LOCAL
  };

  static string EntryPoint;
  static string makeID(const string&);  

  X86(SymbolTable&);
  ~X86();


  void init(const string&);
  string source();

  void writeBSS(const string&);
  void writeDATA(const string&);
  void writeTEXT(const string&);

  void createScope(const string&);
  string currentScope();

  void declarePrimitive(int decl_type, const string& name, int type);
  void declareMatrix(int decl_type, int type, string name, list<string> dims);

  string addGlobalLiteral(string str);
  string translateFuncLeia(const string& id, int type);
  string translateFuncImprima(const string& id, int type);
  string createLabel(bool local, string tmpl);

  void writeExit();

  void writeAttribution(int e1, int e2, pair<pair<int, bool>, string>&);
  void writeOuExpr();
  void writeEExpr();
  void writeBitOuExpr();
  void writeBitXouExpr();
  void writeBitEExpr();
  void writeIgualExpr(int e1, int e2);
  void writeDiferenteExpr(int e1, int e2);
  void writeMaiorExpr(int e1, int e2);
  void writeMenorExpr(int e1, int e2);
  void writeMaiorEqExpr(int e1, int e2);
  void writeMenorEqExpr(int e1, int e2);
  void writeMaisExpr(int e1, int e2);
  void writeMenosExpr(int e1, int e2);
  void writeDivExpr(int e1, int e2);
  void writeMultipExpr(int e1, int e2);
  void writeModExpr();
  void writeUnaryNeg(int etype);
  void writeUnaryNot();
  void writeUnaryBitNotExpr();
  void writeLiteralExpr(const string& src);
  void writeLValueExpr(pair< pair<int, bool>, string>&);

  void writeCast(int e1, int e2);

  string toChar(const string&);
  string toReal(const string&);
private:
  string toNasmString(string str);

  SymbolTable& _stable;

  string       _currentScope;  

  stringstream _head;
  stringstream _bss;
  stringstream _data;  
  stringstream _lib;  

  map<string, X86SubProgram> _subprograms;
};

#endif
