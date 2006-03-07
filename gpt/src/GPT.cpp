/***************************************************************************
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
 *   thiago.silva@kdemail.net                                              *
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
#include "GPT.hpp"

#ifdef WIN32
  #include <io.h> //unlink()
#endif

#include <antlr/AST.hpp>
#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
#include "Portugol2CWalker.hpp"
#include "SemanticWalker.hpp"
#include "InterpreterWalker.hpp"
#include "X86Walker.hpp"

#include <fstream>
#include <sstream>

GPT* GPT::_self = 0;

GPT::GPT()
{
}

GPT::~GPT()
{
}

GPT* GPT::self() {
  if(!GPT::_self) {
    GPT::_self = new GPT();
  }
  return GPT::_self;
}

void GPT::reportDicas(bool value) {
  Display::self()->showTips(value);
}

string GPT::createTmpFile() {
  #ifdef WIN32
    string cf = getenv("TEMP");
    cf += "gpt_tmp"; //TODO! criar real tmp file
    return cf;
  #else
    char cfilepath[] = "/tmp/c__XXXXXX";
    int fd = mkstemp(cfilepath);
    close(fd);
    return cfilepath;
  #endif
}


void GPT::showHelp() {
  stringstream s;
  s << "Modo de uso: " << PACKAGE << " [opções] <arquivo>\n\n"
          "Opções:\n"
          "   -v            mostra versão do programa\n"
          "   -h            mostra esse texto\n"
          "   -o <arquivo>  compila e salva executável como <arquivo>\n"
          "   -t <arquivo>  salva o código em linguagem C como <arquivo>\n"
          "   -s <arquivo>  salva o código em linguagem assembly como <arquivo>\n"
          "   -i            interpreta\n"
          "\n"
          "   -d            exibe dicas no relatório de erros\n\n"
          "   Maiores informações no manual.\n";

  Display::self()->showMessage(s);
}

void GPT::showVersion() {
  stringstream s;
  s << "GPT - Compilador G-Portugol\n"
          "Versão  : " << VERSION << "\n"
          "Website : http://gpt.berlios.de\n"
          "Copyright (C) 2003-2006 Thiago Silva <thiago.silva@kdemail.net>\n\n";
  Display::self()->showMessage(s);
}

bool GPT::prologue(const string& ifname) {
  stringstream s;
  bool success = false;

  if(ifname.length() > 0) {
    ifstream fi;
    fi.open(ifname.c_str(), ios_base::in);
    if(!fi) {
      s << PACKAGE << ": não foi possível abrir o arquivo: \"" << ifname << "\"" << endl;
      Display::self()->showError(s);
      goto bail;
    }

    if(!parse(fi)) {
      goto bail;
    }
  } else { //shell pipe
    if(cin.rdbuf()->in_avail() == 0) {
      s << PACKAGE << ": não existem dados na entrada padrão" << endl;
      Display::self()->showError(s);
      goto bail;
    }

    if(!parse(cin)) {
      goto bail;
    }
  }

  success = true;

  bail:
    return success;
}

bool GPT::compile(const string& ifname, const string& ofname, bool genBinary) {
  bool success = false;
  stringstream s;

  if(!prologue(ifname)) {
    return false;
  }

  X86Walker x86(_stable);
  string asmsrc = x86.algoritmo(_astree);

  string ftmpname = createTmpFile();
  ofstream fo;

  if(!genBinary) { //salva assembly code
    fo.open(ofname.c_str(), ios_base::out);
    if(!fo) {
      s << PACKAGE << ": não foi possível abrir o arquivo: \"" << ofname << "\"" << endl;
      Display::self()->showError(s);
      goto bail;
    }
    fo << asmsrc;
    fo.close();
  } else { //compile
    fo.open(ftmpname.c_str(), ios_base::out);
    if(!fo) {
      s << PACKAGE << ": erro ao processar arquivo temporário" << endl;
      Display::self()->showError(s);
      goto bail;
    }
    fo << asmsrc;
    fo.close();

    stringstream cmd;
    cmd << "nasm -O1 -fbin -o " << ofname << " " << ftmpname;

    if(system(cmd.str().c_str()) == -1) {
      s << PACKAGE << ": não foi possível invocar gcc." << endl;
      Display::self()->showError(s);
      goto bail;
    }

    #ifndef WIN32
      cmd.str("");
      cmd << "chmod +x " << ofname;
      system(cmd.str().c_str());
    #endif
  }

  success = true;

  bail:
    if(ftmpname.length()>0) {
       unlink(ftmpname.c_str());
    }
    return success;
}

bool GPT::translate2C(const string& ifname, const string& ofname) {
  bool success = false;
  stringstream s;

  if(!prologue(ifname)) {
    return false;
  }

  Portugol2CWalker pt2c(_stable);
  string c_src = pt2c.algoritmo(_astree);

  ofstream fo;
  fo.open(ofname.c_str(), ios_base::out);
  if(!fo) {
    s << PACKAGE << ": não foi possível abrir o arquivo: \"" << ofname << "\"" << endl;
    Display::self()->showError(s);
    goto bail;
  }
  fo << c_src;
  fo.close();

  success = true;

  bail:
    return success;
}

bool GPT::interpret(const string& ifname, const string& host, int port) {
  if(!prologue(ifname)) {
    return false;
  }

  InterpreterWalker interpreter(_stable, host, port);
  interpreter.algoritmo(_astree);

  return true;
}

bool GPT::parse(istream& fi) {
  stringstream s;
  try {
    PortugolLexer lexer(fi);
    PortugolParser parser(lexer);

    ASTFactory ast_factory(PortugolAST::TYPE_NAME,&PortugolAST::factory);
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    parser.algoritmo();

    if(Display::self()->hasError()) {
      Display::self()->showErrors();
      return false;
    }

    _astree = parser.getPortugolAST();

    if(!_astree) {
      s << PACKAGE << ": erro interno: no parse tree" << endl;
      Display::self()->showError(s);
      return false;
    }
    SemanticWalker semantic(_stable);
    semantic.algoritmo(_astree);

    if(Display::self()->hasError()) {
      Display::self()->showErrors();
      return false;
    }
    return true;
  }
  catch(ANTLRException& e) {
    s << PACKAGE << ": erro interno: " << e.toString() << endl;
    Display::self()->showError(s);
    return false;
  }
  catch(exception& e) {
    s << PACKAGE << ": erro interno: " << e.what() << endl;
    Display::self()->showError(s);
    return false;
  }

  s << "GPT::parse: bug, nao deveria executar essa linha!" << endl;
  Display::self()->showError(s);
  return false;
}
