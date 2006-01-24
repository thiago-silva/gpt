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
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "antlr/AST.hpp"
#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
#include "Portugol2CWalker.hpp"
#include "SemanticWalker.hpp"
#include "SymbolTable.hpp"
#include "PortugolAST.hpp"
#include "ErrorHandler.hpp"
#include "InterpreterWalker.hpp"

#define DEFAULT_PORT "7680"

using namespace std;
using namespace antlr;

bool debug_flag = false;

bool translate_only = false;
bool interpret_only = false;
bool show_tips = false;
string ifilepath; //pt source
string ofilepath; //binary output
string trpath;    //translated C source
char cfilepath[] = "/tmp/c__XXXXXX"; //tmp translated C source

//conexao com debugger
string host;
string port;

bool frompipe = false;

void showhelp(void) {
  cout << PACKAGE << " [opções] arquivo\n"
          "Opções:\n"
          "   -v            mostra versão do programa\n"
          "   -h            mostra esse texto\n"
          "   -d            exibe dicas no relatório de erros\n"
          "   -o <arquivo>  compila e salva executável como <arquivo>\n"
          "   -t <arquivo>  traduz para C e salva como <arquivo>\n"
          "   -i            não compila, apenas interpreta\n"
          "   -H <host>     host do client debugger (deve ser usado com -i)\n"
          "   -P <porta>    porta do client debugger (deve ser usado com -i)\n"
          "\n";
}

void showversion(void) {
  cout << "GPT - Compilador G-Portucol\n"
          "versão  : " << VERSION << "\n"
          "Website : http://codigolivre.org/compuclass\n"
          "Copyright (C) 2003-2005 Thiago Silva <thiago.silva@kdemail.net>\n\n";
}

bool init(int argc, char** argv) {

  if(argc == 1) {
    showhelp();
    return false;
  }

  int c;
  opterr = 0;  

	ofilepath = "a.out";
  port = DEFAULT_PORT;

#ifdef DEBUG
    while((c = getopt(argc, argv, "o:t:H:P:hvipdD")) != -1) {
#else
    while((c = getopt(argc, argv, "o:t:H:P:hvid")) != -1) {
#endif
    switch(c) {
#ifdef DEBUG
      case 'D':
        debug_flag = true;
        break;
#endif
       case 'd':
        show_tips = true;
        break;
       case 't':
        translate_only = true;
        if(optarg) {
          trpath = optarg;
        }
        break;
      case 'i':
        interpret_only = true;
        break;
      case 'H':
        if(optarg) {
          host = optarg;
        }
        break;
      case 'P':
        if(optarg) {
          port = optarg;
        }
        break;
      case 'p':
        frompipe = true;
        break;
      case 'v':
        showversion();
        exit(EXIT_SUCCESS);
      case 'h':
        showhelp();
        exit(EXIT_SUCCESS);
      case 'o':
        if(optarg) {
          ofilepath = optarg;
        } 
        break;
        case '?':
           if((optopt == 'o') || (optopt == 't')) {
            cerr << PACKAGE << ": faltando argumento para opção -" << (char)optopt << endl;
           } else {
             cerr << PACKAGE << ": opção inválida: -" <<  char(optopt) << endl;
           }
           return false;
     default:
         cerr << PACKAGE << ": erro interno." << endl;
        return false;
    }
  }

  if(!frompipe) {
    if(optind < argc) {
      ifilepath = argv[optind];
    } else {
      cerr << PACKAGE << ": nenhum arquivo especificado." << endl;
      return false;
    }
  }

  if(interpret_only) {
    if((port != DEFAULT_PORT) && (atoi(port.c_str()) == 0)) {      
      cerr << PACKAGE << ": porta de conexão inválida: \"" << port << "\"" << endl;
      return false;
    }
  }
  
  return true;
}

bool do_parse(istream& in, ostream& out) {

  DEBUG_PARSER = false;

  try
  {
    PortugolLexer lexer(in);
    PortugolParser parser(lexer);
    
    ASTFactory ast_factory(PortugolAST::TYPE_NAME,&PortugolAST::factory);
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    parser.algoritmo();
//     ofilepath = parser.getAlgName();

    if(ErrorHandler::self()->hasError()) {
      ErrorHandler::self()->showErrors(show_tips);
      return false;
    }
    
    RefPortugolAST tree = parser.getPortugolAST();
    if(tree)
    {
      if(debug_flag) {cerr << tree->toStringList() << endl << endl;} 

      SymbolTable stable;
      SemanticWalker semantic(stable);
      semantic.algoritmo(tree);

      if(ErrorHandler::self()->hasError()) {
        ErrorHandler::self()->showErrors(show_tips);
        return false;
      }      

      //parsing complete!

      if(interpret_only) {
        InterpreterWalker interpreter(stable, host, atoi(port.c_str()));
        interpreter.algoritmo(tree);
      } else {
        Portugol2CWalker pt2c(stable);        
        string c_src = pt2c.algoritmo(tree);

        out << c_src << endl;

        if(debug_flag) {
          cerr << "BEGIN SOURCE >>>> \n";
          cerr << c_src << endl;
          cerr << "<<<<<< END SOURCE\n";          
        }        
      }
      return true;
    }
    else {
      cerr << PACKAGE << ": erro interno: No tree produced" << endl;
      return false;
    }
  }
  catch(ANTLRException& e)
  {
    cerr << PACKAGE << ": erro interno: " << e.toString() << endl;
    return false;
  }
  catch(exception& e)
  {
    cerr << PACKAGE << ": erro interno: " << e.what() << endl;
    return false;
  }

  cerr << "main::do_parse: bug, nao deveria executar essa linha!" << endl;
  return true;
}

bool parse(void) {
  bool success = false;
  ofstream fout;

  int fd = mkstemp(cfilepath);
  close(fd);
  
  fout.open(cfilepath, ios_base::out);
  
  if(!fout) {
    cerr << PACKAGE << ": não foi possível abrir o arquivo: \"" << cfilepath << "\"" << std::endl;
    goto end;
  }

  if(frompipe) {
    if(cin.rdbuf()->in_avail() == 0) { 
      cerr << PACKAGE << ": não existem dados na entrada padrão" << std::endl;
      goto end;
    } else {
      success = do_parse(cin, fout);
      goto end;
    }
  } else {
    ifstream fin;
    fin.open(ifilepath.c_str(), ios_base::in);
    if(!fin) {
      cerr << PACKAGE << ": não foi possível abrir o arquivo: \"" << ifilepath << "\"" << std::endl;
      goto end;
    } else {
      success = do_parse(fin, fout);
      goto end;
    }
  }

  end:
    fout.close();     
    return success;
}

bool compile(void) {
  stringstream cmd;
  cmd << "gcc -ansi -x c -o " << ofilepath << " " << cfilepath;
    
  if(debug_flag) {cerr << "cmd: " << cmd.str() << endl;}

  if(system(cmd.str().c_str()) == -1) {
    cerr << PACKAGE << ": não foi possível invocar gcc." << endl;
    return false;
  }
  return true;
}

bool copy_source() {
  ofstream out(trpath.c_str());
  ifstream in(cfilepath);

//   out.open(, ios_base::out);  
//   in.open(cfilepath, ios_base::in);

  if(!out || !in) {
    cerr << PACKAGE << ": não foi possível criar arquivo fonte." << endl;
    return  false;
  }

  char c;
  while(in.get(c)) {
    out.put(c);
  }

  if(!in.eof() || !out) {
    cerr << PACKAGE << ": ocorreu um erro ao criar arquivo fonte." << endl;
    return false;
  }

  in.close();
  out.close();  
  return true;
}

int main(int argc, char** argv)
{ 
  int success = EXIT_SUCCESS;

  if(!init(argc, argv)) {
    success = EXIT_FAILURE;
    goto the_end;
  }

  if(parse()) {
    if(translate_only) {
      success = copy_source()?EXIT_SUCCESS:EXIT_FAILURE;
    } else if(!interpret_only) {
      success = compile()?EXIT_SUCCESS:EXIT_FAILURE;
    }
  } else {
    success = EXIT_FAILURE;
  }

  the_end:
    unlink(cfilepath);
    return success;
}
