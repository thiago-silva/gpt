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
#include <antlr/TokenStreamSelector.hpp>
#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
#include "Portugol2CWalker.hpp"
#include "SemanticWalker.hpp"
#include "InterpreterWalker.hpp"
#include "X86Walker.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

GPT* GPT::_self = 0;

GPT::GPT()
  :/* _usePipe(false),*/ _printParseTree(false)
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
  GPTDisplay::self()->showTips(value);
}

void GPT::printParseTree(bool value)
{
  _printParseTree = value;
}

// void GPT::usePipe(bool value) 
// {
//   _usePipe = value;
// }

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
  s << "Modo de uso: " << PACKAGE << " [opções] algoritmos...\n\n"
          "Opções:\n"
          "   -v   mostra versão do programa\n"
          "   -h   mostra esse texto\n"
          "   -c   cria código em linguagem C\n"
          "   -s   cria código em linguagem assembly\n"
          "   -i   interpreta o algoritmo\n"
          "\n"
          "   -d   exibe dicas no relatório de erros\n\n"
          "   Maiores informações no manual.\n";

  GPTDisplay::self()->showMessage(s);
}

void GPT::showVersion() {
  stringstream s;
  s << "GPT - Compilador G-Portugol\n"
          "Versão  : " << VERSION << "\n"
          "Website : http://gpt.berlios.de\n"
          "Copyright (C) 2003-2006 Thiago Silva <thiago.silva@kdemail.net>\n\n";
  GPTDisplay::self()->showMessage(s);
}

bool GPT::prologue(const list<string>& ifnames) {
  stringstream s;
  bool success = false;

//   if(_usePipe) { //shell pipe (stdin)
//     if(cin.rdbuf()->in_avail() == 0) {
//       s << PACKAGE << ": não existem dados na entrada padrão" << endl;
//       GPTDisplay::self()->showError(s);
//       goto bail;
//     }
// 
//     if(!parse(cin)) {
//       goto bail;
//     }
//   } else {
  list<istream*> istream_list;
  for(list<string>::const_iterator it = ifnames.begin(); it != ifnames.end(); ++it) {
    ifstream *fi = new ifstream((*it).c_str());
    //fi.open((*it).c_str(), ios_base::in);
    if(!*fi) {
      s << PACKAGE << ": não foi possível abrir o arquivo: \"" << (*it) << "\"" << endl;
      GPTDisplay::self()->showError(s);
      goto bail;
    }
    istream_list.push_back(fi);
  }

  if(!parse(istream_list)) {
    goto bail;
  }
    
  success = true;

  bail:
    return success;
}

bool GPT::compile(const list<string>& ifnames, bool genBinary) {  
  bool success = false;
  stringstream s;

  if(!prologue(ifnames)) {
    return false;
  }

  string ofname;
  if(!genBinary) {
    ofname = _nomeAlgorimto + ".asm";
  } else {
    #ifdef WIN32
    ofname = _nomeAlgorimto + ".exe";
    #else
    ofname = _nomeAlgorimto;
    #endif
  }

  X86Walker x86(_stable);
  string asmsrc = x86.algoritmo(_astree);

  string ftmpname = createTmpFile();
  ofstream fo;

  if(!genBinary) { //salva assembly code
    fo.open(ofname.c_str(), ios_base::out);
    if(!fo) {
      s << PACKAGE << ": não foi possível abrir o arquivo: \"" << ofname << "\"" << endl;
      GPTDisplay::self()->showError(s);
      goto bail;
    }
    fo << asmsrc;
    fo.close();
  } else { //compile
    fo.open(ftmpname.c_str(), ios_base::out);
    if(!fo) {
      s << PACKAGE << ": erro ao processar arquivo temporário" << endl;
      GPTDisplay::self()->showError(s);
      goto bail;
    }
    fo << asmsrc;
    fo.close();

    stringstream cmd;
    cmd << "nasm -O1 -fbin -o " << ofname << " " << ftmpname;

    if(system(cmd.str().c_str()) == -1) {
      s << PACKAGE << ": não foi possível invocar gcc." << endl;
      GPTDisplay::self()->showError(s);
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

bool GPT::translate2C(const list<string>& ifnames) {
  bool success = false;
  stringstream s;

  if(!prologue(ifnames)) {
    return false;
  }

  string ofname = _nomeAlgorimto + ".c";

  Portugol2CWalker pt2c(_stable);
  string c_src = pt2c.algoritmo(_astree);

  ofstream fo;
  fo.open(ofname.c_str(), ios_base::out);
  if(!fo) {
    s << PACKAGE << ": não foi possível abrir o arquivo: \"" << ofname << "\"" << endl;
    GPTDisplay::self()->showError(s);
    goto bail;
  }
  fo << c_src;
  fo.close();

  success = true;

  bail:
    return success;
}

bool GPT::interpret(const list<string>& ifnames, const string& host, int port) {
  if(!prologue(ifnames)) {
    return false;
  }

  InterpreterWalker interpreter(_stable, host, port);
  interpreter.algoritmo(_astree);

  return true;
}



bool GPT::parse(list<istream*>& istream_list) {
  stringstream s;
  
  try {
    TokenStreamSelector*  selector = new TokenStreamSelector;
    
    PortugolLexer* lexer;
    PortugolLexer* first = 0;
    int c = 0;
    for(list<istream*>::iterator it = istream_list.begin(); it != istream_list.end(); ++it, c++) {
      lexer = new PortugolLexer(*(*it), selector);
      //setFileName

      //messy but works!!
      selector->addInputStream(lexer, "name");
      selector->select(lexer);
      selector->push("name");
      if(!first) {
        first = lexer;
      }
    }
    first->setTotalLexers(c);
    selector->select(first);
    PortugolParser parser(*selector);

    ASTFactory ast_factory(PortugolAST::TYPE_NAME,&PortugolAST::factory);
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    parser.algoritmo();
    _nomeAlgorimto = parser.nomeAlgoritmo();

    if(GPTDisplay::self()->hasError()) {
      GPTDisplay::self()->showErrors();
      return false;
    }

    _astree = parser.getPortugolAST();

    if(!_astree) {
      s << PACKAGE << ": erro interno: no parse tree" << endl;
      GPTDisplay::self()->showError(s);
      return false;
    }

    if(_printParseTree) {
      std::cerr << _astree->toStringList() << std::endl << std::endl;
    }

    SemanticWalker semantic(_stable);
    semantic.algoritmo(_astree);

    if(GPTDisplay::self()->hasError()) {
      GPTDisplay::self()->showErrors();
      return false;
    }
    return true;
  }
  catch(ANTLRException& e) {
    s << PACKAGE << ": erro interno: " << e.toString() << endl;
    GPTDisplay::self()->showError(s);
    return false;
  }
  catch(exception& e) {
    s << PACKAGE << ": erro interno: " << e.what() << endl;
    GPTDisplay::self()->showError(s);
    return false;
  }

  s << "GPT::parse: bug, nao deveria executar essa linha!" << endl;
  GPTDisplay::self()->showError(s);
  return false;
}
