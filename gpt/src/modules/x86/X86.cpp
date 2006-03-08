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

#include "X86.hpp"
#include "GPTDisplay.hpp"

#include <stdlib.h>

X86SubProgram::X86SubProgram()
  : SizeofDWord(sizeof(int)), //4
    _param_offset(8),         //starts at +8
    _local_offset(4) {        //starts at -4

}

X86SubProgram::X86SubProgram(const X86SubProgram& other)
    : SizeofDWord(sizeof(int)),
    _param_offset(other._param_offset),
    _local_offset(other._local_offset),
    _name(other._name),
    _params(other._params),
    _locals(other._locals) {

  _head << other._head.str();
  _txt  << other._txt.str();
  _init << other._init.str();
}

X86SubProgram::~X86SubProgram() {
}

void X86SubProgram::writeTEXT(const string& str) {
  _txt << str << endl;
}

void X86SubProgram::init(const string& name) {
  _name = name;
}

string X86SubProgram::name() {
  return _name;
}

void X86SubProgram::declareLocal(const string& local_var, int msize, bool minit) {
  if(msize == 0) {
    _head << "%define " << X86::makeID(local_var) << " ebp-" << _local_offset << endl;

    if(minit) {
      _init << "mov dword [" << X86::makeID(local_var) << "], 0" << endl;
    }

    _local_offset += SizeofDWord;
  } else {
    _head << "%define " << X86::makeID(local_var) << " ebp-" << (_local_offset+(msize*SizeofDWord)-SizeofDWord) << endl;
    if(minit) {
      writeMatrixInitCode(local_var, msize);
    }
    _local_offset += (msize*SizeofDWord);
  }
}

void X86SubProgram::declareParam(const string& param, int type, int msize) {

  if(msize == 0) {
    _head << "%define " << X86::makeID(param) << " ebp+" << _param_offset << endl;
  } else {
    _head << "%define _p_" << X86::makeID(param) << " ebp+" << _param_offset << endl;
    declareLocal(param, msize, false);
    writeMatrixCopyCode(param, type, msize);
  }
  _param_offset += SizeofDWord;
}

void X86SubProgram::writeMatrixCopyCode(const string& param, int type, int msize) {
  _init << "lea eax, [" << X86::makeID(param) << "]" << endl;
  _init << "addarg dword [_p_" << X86::makeID(param) << "]" << endl;
  _init << "addarg eax" << endl;
  _init << "addarg " << (type == TIPO_LITERAL) << endl;
  _init << "addarg " << (msize*SizeofDWord) << endl;
  _init << "call matrix_cpy" << endl;
  _init << "clargs 4" << endl;
}

void X86SubProgram::writeMatrixInitCode(const string& varname, int size) {
  _init << "lea ebx, [" << X86::makeID(varname) << "]" << endl;
  _init << "addarg ebx" << endl;
  _init << "addarg " << size << " * SIZEOF_DWORD" << endl;
  _init << "call matrix_init" << endl;
  _init << "clargs 2" << endl;

//     _init << "addarg " << size << " * SIZEOF_DWORD" << endl;
//     _init << "call __malloc" << endl;
//     _init << "clargs 1" << endl;
//     _init << "lea ebx, [" << varname << "]" << endl;
//     _init << "mov dword [ebx], eax" << endl;
//     _init << "addarg dword [ebx]" << endl;
//     _init << "addarg " << size << " * SIZEOF_DWORD" << endl;
//     _init << "call matrix_init__" << endl;
//     _init << "clargs 2" << endl;
}

string X86SubProgram::source() {
  stringstream s;
  
  if(_name != X86::EntryPoint) {
    s << X86::makeID(_name) << ":" << endl;
  } else {
    s << _name << ":" << endl;
  }
  s << _head.str();

  if(name() != X86::EntryPoint) {
    s << "begin " << (_local_offset-SizeofDWord) << endl;//locals starts at -4
  }

  s << _init.str();

  s << _txt.str();

  return s.str();
}






////////--------------------------------------------------------




string X86::EntryPoint = "start";

string X86::makeID(const string& str) {
  return string("_") + str;
}

X86::X86(SymbolTable& st)
 : _stable(st) {

}

X86::~X86() {
}

void X86::init(const string& name) {

    _head << "; algoritmo: " << name << "\n\n";

    #ifdef WIN32
      #include <asm_win32.h>
    #else
      #include <asm_elf.h>
    #endif

    _bss << "section .bss\n"
           "    mem    resb  MEMORY_SIZE\n\n";

    _data << "section .data\n"
            "              data_no equ $\n"
            "    mem_ptr         dd 0\n"
            "    aux             dd 0\n"
            "    str_true        db 'verdadeiro',0\n"
            "    str_false       db 'falso',0\n"
            "    str_no_mem_left db 'Não foi possível alocar memória.',0\n\n";


  createScope(SymbolTable::GlobalScope);
}

void X86::writeBSS(const string& str) {
  _bss << str << endl;
}

void X86::writeDATA(const string& str) {
  _data << str << endl;
}

void X86::writeTEXT(const string& str) {
  _subprograms[_currentScope].writeTEXT(str);;

}

void X86::createScope(const string& scope) {
  _currentScope = scope;
  if(scope == SymbolTable::GlobalScope) {
    _subprograms[scope].init(X86::EntryPoint);
  } else {
//     Symbol symb = _stable.getSymbol(SymbolTable::GlobalScope, scope, true);
    _subprograms[scope].init(scope);
//       symb.param.symbolList().size(), _stable.getSymbols(scope).size());
  }
}

string X86::currentScope() {
  return _currentScope;
}

void X86::writeExit() {
  writeTEXT("exit 0");
}

void X86::declarePrimitive(int decl_type, const string& name, int type) {
  stringstream s;
  if(decl_type == VAR_GLOBAL) {
    //all sizes have 32 bits (double word)
    switch(type) {
      case TIPO_INTEIRO:
      case TIPO_REAL:
      case TIPO_CARACTERE:
      case TIPO_LITERAL:
      case TIPO_LOGICO:
        s << X86::makeID(name) << " dd 0";
        writeDATA(s.str());
        break;
      default:
        GPTDisplay::self()->showError("Erro interno: tipo nao suportado (X86::declarePrimitive).");
        exit(1);
    }
  } else if(decl_type == VAR_PARAM) {
    _subprograms[currentScope()].declareParam(name, type);
  } else if(decl_type == VAR_LOCAL) {
    _subprograms[currentScope()].declareLocal(name);
  } else {
    GPTDisplay::self()->showError("Erro interno: X86::declarePrimitive).");
    exit(1);
  }
}

void X86::declareMatrix(int decl_type, int type, string name, list<string> dims) {
  int size = 1;
  for(list<string>::iterator it = dims.begin(); it != dims.end(); it++) {
    size *= atoi((*it).c_str());
  }


  if(decl_type == VAR_GLOBAL) {
    stringstream s;
    switch(type) {
      case TIPO_INTEIRO:
      case TIPO_REAL:
      case TIPO_CARACTERE:
      case TIPO_LOGICO:
      case TIPO_LITERAL:
        s << X86::makeID(name) << " times " << size << " dd 0";
        break;
      default:
        GPTDisplay::self()->showError("Erro interno: tipo nao suportado (X86::declarePrimitive).");
        exit(1);
    }
    writeDATA(s.str());
  } else if(decl_type == VAR_PARAM) {
    _subprograms[currentScope()].declareParam(name, type, size);
  } else if(decl_type == VAR_LOCAL) {
    _subprograms[currentScope()].declareLocal(name, size);
  } else {
    GPTDisplay::self()->showError("Erro interno: X86::declareMatrix).");
    exit(1);
  }
}


string X86::toNasmString(string str) {
  string ret;
  for(unsigned int i = 0; i < str.length(); i++) {
    if(str[i] == '\\') {
      i++;
      switch(str[i]) {
        case '0':
          ret += "',0,'";
          break;
        case 'n':
          ret += "',10,'";
          break;
        case 'r':
          ret += "',13,'";
          break;
        default:
          ret += str[i];
      }
    } else {
      ret += str[i];
    }
  }
  return ret;
}

string X86::addGlobalLiteral(string str) {
  stringstream s;
  string lb = createLabel(false, "str");
  s << lb << " db '" << toNasmString(str) << "',0";
  writeDATA(s.str());
  return lb;
}

string X86::translateFuncLeia(const string& id, int type) {
  switch(type) {
    case TIPO_REAL:
      return "leia_real";
    case TIPO_LITERAL:
      return "leia_literal";
    case TIPO_CARACTERE:
      return "leia_caractere";
    case TIPO_LOGICO:
      return "leia_logico";
    case TIPO_INTEIRO:
      return "leia_inteiro";
    default:
      GPTDisplay::self()->showError("Erro interno: tipo nao suportado (x86::translateFuncLeia).");
      exit(1);
  }
}

string X86::translateFuncImprima(const string& id, int type) {
  switch(type) {
    case TIPO_REAL:
      return "imprima_real";
    case TIPO_LITERAL:
      return "imprima_literal";
    case TIPO_CARACTERE:
      return "imprima_caractere";
    case TIPO_LOGICO:
      return "imprima_logico";
    case TIPO_INTEIRO:
      return "imprima_inteiro";
    default:
      GPTDisplay::self()->showError("Erro interno: tipo nao suportado (x86::translateFuncImprima).");
      exit(1);
  }
}

string X86::createLabel(bool local, string tmpl) {
  static int c = 0;
  stringstream s;
  if(local) {
    s << ".__" << tmpl << "_" << c;
  } else {
    s << "__" << tmpl << "_" << c;
  }
  c++;
  return s.str();
}


string X86::source() {
  stringstream str;

  //.data footer
  writeDATA("datasize   equ     $ - data_no");

  str << _head.str()
      << _bss.str()
      << _data.str();

  //.text header
  str << "section .text" << endl;
  str << "start_no equ $" << endl;

  string sss;
  for(map<string, X86SubProgram>::iterator it = _subprograms.begin(); it != _subprograms.end(); ++it) {
    sss = it->second.source();
    str << it->second.source();
  }

  str  << _lib.str();

  return str.str();
}

void X86::writeCast(int e1, int e2) {
  //casts
  if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) {
    //int to float
    writeTEXT("mov dword [aux], eax");
    writeTEXT("fild dword [aux]");
    writeTEXT("fstp dword [aux]");
    writeTEXT("mov eax, dword [aux]");
  } else if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) {
    writeTEXT("mov dword [aux], eax");
    writeTEXT("fld dword [aux]");
    writeTEXT("fistp dword [aux]");
    writeTEXT("mov eax, dword [aux]");
  }
}

void X86::writeAttribution(int e1, int e2, pair<pair<int, bool>, string>& lv) {
  writeTEXT("pop eax");
  writeTEXT("pop ecx");

  writeCast(e1, e2);

  stringstream s;

  Symbol symb = _stable.getSymbol(currentScope(), lv.second, true);
  s << "lea edx, [" << X86::makeID(lv.second) << "]";
  writeTEXT(s.str());

  s.str("");
  s << "lea edx, [edx + ecx * SIZEOF_DWORD]";
  writeTEXT(s.str());
  s.str("");
  s << "mov [edx], eax";
  writeTEXT(s.str());
}


void X86::writeOuExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("cmp eax, 0");
  writeTEXT("setne al");
  writeTEXT("and eax, 0xff");
  writeTEXT("cmp ebx, 0");
  writeTEXT("setne bl");
  writeTEXT("and ebx, 0xff");
  writeTEXT("or al, bl");

  writeTEXT("push eax");
}

void X86::writeEExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("cmp eax, 0");
  writeTEXT("setne al");
  writeTEXT("and eax, 0xff");
  writeTEXT("cmp ebx, 0");
  writeTEXT("setne bl");
  writeTEXT("and ebx, 0xff");
  writeTEXT("and al, bl");

  writeTEXT("push eax");
}

void X86::writeBitOuExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("or eax, ebx");

  writeTEXT("push eax");
}

void X86::writeBitXouExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("xor eax, ebx");

  writeTEXT("push eax");
}

void X86::writeBitEExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("and eax, ebx");

  writeTEXT("push eax");
}

void X86::writeIgualExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("cmp eax, ebx");
  writeTEXT("sete al");
  writeTEXT("and eax, 0xff");

  writeTEXT("push eax");
}

void X86::writeDiferenteExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("cmp eax, ebx");
  writeTEXT("setne al");
  writeTEXT("and eax, 0xff");

  writeTEXT("push eax");
}

void X86::writeMaiorExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  //fcomp assumes ST0 is left-hand operand aways
  //flags after comp:
  //5.0 @ 4 : ax -> 0
  //5.0 @ 5 : ax -> 0x4000
  //4.0 @ 6 : ax -> 0x100
  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0");
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0x100");
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("fcomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0");
    }
    writeTEXT("sete al");
    writeTEXT("and eax, 0xff");
  } else {
    writeTEXT("cmp eax, ebx");
    writeTEXT("setg al");
    writeTEXT("and eax, 0xff");
  }

  writeTEXT("push eax");
}

void X86::writeMenorExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0x100");
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0");
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("fcomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0x100");
    }
    writeTEXT("sete al");
    writeTEXT("and eax, 0xff");
  } else {
    writeTEXT("cmp eax, ebx");
    writeTEXT("setl al");
    writeTEXT("and eax, 0xff");
  }

  writeTEXT("push eax");
}

void X86::writeMaiorEqExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0");
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0x100");
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("fcomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0");
    }
    writeTEXT("sete bl");
    writeTEXT("and ebx, 0xff");

    writeTEXT("cmp ax, 0x4000");
    writeTEXT("sete al");
    writeTEXT("and eax, 0xff");
    writeTEXT("or eax, ebx");
  } else {
    writeTEXT("cmp eax, ebx");
    writeTEXT("setge al");
    writeTEXT("and eax, 0xff");
  }

  writeTEXT("push eax");
}

void X86::writeMenorEqExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0x100");
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      writeTEXT("ficomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0");
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      writeTEXT("fcomp dword [aux]");
      writeTEXT("fstsw ax");
      writeTEXT("cmp ax, 0x100");
    }
    writeTEXT("sete bl");
    writeTEXT("and ebx, 0xff");

    writeTEXT("cmp ax, 0x4000");
    writeTEXT("sete al");
    writeTEXT("and eax, 0xff");
    writeTEXT("or eax, ebx");
  } else {
    writeTEXT("cmp eax, ebx");
    writeTEXT("setle al");
    writeTEXT("and eax, 0xff");
  }

  writeTEXT("push eax");
}

void X86::writeMaisExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    string addpop;
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      addpop = "fiadd dword [aux]";
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      addpop = "fiadd dword [aux]";
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      addpop = "fadd dword [aux]";
    }
      writeTEXT(addpop);
      writeTEXT("fstp dword [aux]");
      writeTEXT("mov eax, dword [aux]");
  } else {
    writeTEXT("add eax, ebx");
  }

  writeTEXT("push eax");
}

void X86::writeMenosExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    string subop;
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      subop = "fisub dword [aux]";
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      subop = "fisub dword [aux]";
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      subop = "fsub dword [aux]";
    }
      writeTEXT(subop);
      writeTEXT("fstp dword [aux]");
      writeTEXT("mov eax, dword [aux]");
  } else {
    writeTEXT("sub eax, ebx");
  }

  writeTEXT("push eax");
}

void X86::writeDivExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    string divpop;
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      divpop = "fidiv dword [aux]";
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      divpop = "fidivr dword [aux]";
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      divpop = "fdiv dword [aux]";
    }
      writeTEXT(divpop);
      writeTEXT("fstp dword [aux]");
      writeTEXT("mov eax, dword [aux]");
  } else {
    writeTEXT("xor edx, edx");
    writeTEXT("idiv ebx");
  }

  writeTEXT("push eax");
}

void X86::writeMultipExpr(int e1, int e2) {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  if((e1 == TIPO_REAL) || (e2 == TIPO_REAL)) {
    string mulpop;
    writeTEXT("fninit");
    if((e1 == TIPO_REAL) && (e2 != TIPO_REAL)) { //float/integer
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      mulpop = "fimul dword [aux]";
    } else if((e1 != TIPO_REAL) && (e2 == TIPO_REAL)) { //integer/float
      writeTEXT("mov [aux], ebx");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], eax");
      mulpop = "fimul dword [aux]";
    } else { //float/float
      writeTEXT("mov [aux], eax");
      writeTEXT("fld dword [aux]");
      writeTEXT("mov [aux], ebx");
      mulpop = "fmul dword [aux]";
    }
      writeTEXT(mulpop);
      writeTEXT("fstp dword [aux]");
      writeTEXT("mov eax, dword [aux]");
  } else {
    writeTEXT("imul eax, ebx");
  }

  writeTEXT("push eax");
}

void X86::writeModExpr() {
  writeTEXT("pop ebx");
  writeTEXT("pop eax");

  writeTEXT("xor edx, edx");
  writeTEXT("idiv ebx");
  writeTEXT("mov eax, edx");

  writeTEXT("push eax");
}

void X86::writeUnaryNeg(int etype) {
  writeTEXT("pop eax");

  stringstream s;
  if(etype == TIPO_REAL) {
    s << "or eax ,0x80000000";
  } else {
    s << "neg eax";
  }
  writeTEXT(s.str());

  writeTEXT("push eax");
}

void X86::writeUnaryNot() {
  writeTEXT("pop eax");

  writeTEXT("mov ebx, eax");
  writeTEXT("xor eax, eax");
  writeTEXT("cmp ebx, 0");
  writeTEXT("sete al");

  writeTEXT("push eax");
}

void X86::writeUnaryBitNotExpr() {
  writeTEXT("pop eax");
  writeTEXT("not eax");

  writeTEXT("push eax");
}


void X86::writeLiteralExpr(const string& src) {
  stringstream s;
  s << "push " << src;
  writeTEXT(s.str());
}

void X86::writeLValueExpr(pair< pair<int, bool>, string>& lv) {
  stringstream s;

  s << "lea edx, [" << X86::makeID(lv.second) << "]";
  writeTEXT(s.str());
  writeTEXT("lea edx, [edx + ecx * SIZEOF_DWORD]");

  if(lv.first.second) { //using matrix (ie mat), oush matrix address
                        //(probably passing mat to a function f(mm[])
    writeTEXT("push edx");
  } else { //not using matrix (ie. mat[1] or x), push the value of var/index
    writeTEXT("push dword [edx]");
  }
}

string X86::toChar(const string& str) {
  if(str.length() == 0) {
    return "0";
  } else if(str[0] != '\\') {
    return str;
  } else {
    string ret;
    switch(str[1]) {
      case '0':
        ret = "0";
        break;
      case 'n':
        ret = "10";
        break;
      case 'r':
        ret += "13";
        break;
      default:
        ret = str[1];
    }
    return ret;
  }
}

string X86::toReal(const string& str) {
  stringstream s;
  //get the content of a float variable to integer.
  float fvalue; //sizeof(float) should be 4
  long  *fvaluep; //sizeof(long) should be 4
  fvalue = atof(str.c_str());
  fvaluep = (long*) &fvalue;
  s << *fvaluep;
  return s.str();
}

