#include "config.h"

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "antlr/AST.hpp"
#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
#include "Portugol2C.hpp"
#include "SymbolTable.hpp"
#include "SemanticCheck.hpp"
#include "PortugolAST.hpp"
#include "ErrorHandler.hpp"
#include "Interpreter.hpp"

  ANTLR_USING_NAMESPACE(std)
  ANTLR_USING_NAMESPACE(antlr)

bool debug_flag = false;

bool interpret_only = false;

string ifilepath;
string ofilepath;
char* cfilepath;

bool frompipe = false;

void showhelp(void) {
  cout << PACKAGE << " [opções] arquivo\n"
          "Opções:\n"
          "   -v            mostra versão do programa\n"
          "   -h            mostra essa ajuda\n"          
          "   -o <arquivo>  compila e salva executável como <arquivo>\n"
          "   -i            não compila, apenas interpreta\n"
          "\n";
}

void showversion(void) {
  cout << "GPT - Compilador G-Portucol\n"
          "versão  : " << VERSION << "\n"
          "Website : http://codigolivre.org/compuclass\n"
          "Copyright (C) 2003-2005 Thiago Silva <thiago.silva@kdemail.net>\n\n";
}

bool init(int argc, char** argv) {
  int c;
  opterr = 0;  
#ifdef DEBUG
    while((c = getopt(argc, argv, "o:ihvpd")) != -1) {
#else
    while((c = getopt(argc, argv, "o:ihvp")) != -1) {
#endif
    switch(c) {
#ifdef DEBUG
      case 'd':
        debug_flag = true;
        break;
#endif
      case 'i':
        interpret_only = true;
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
           if(optopt == 'o') {
            cerr << PACKAGE << ": faltando argumento para opção -o." << endl;
            return false;
           } else {
             cerr << PACKAGE << ": opção inválida: -" <<  char(optopt) << endl;
           }
           break;
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
    ofilepath = parser.getAlgName();

    if(ErrorHandler::self()->hasError()) {
      ErrorHandler::self()->showErrors();
      return false;
    }
    
    RefPortugolAST tree = parser.getPortugolAST();
    if(tree)
    {
      if(debug_flag) {cerr << tree->toStringList() << endl << endl;} 

      SymbolTable stable;
      SemanticCheck semantic(stable);
      semantic.algoritmo(tree);

      if(ErrorHandler::self()->hasError()) {
        ErrorHandler::self()->showErrors();
        return false;
      }      

      //parsing complete!

      if(interpret_only) {
        Interpreter interpreter(stable);
        interpreter.algoritmo(tree);
      } else {
        Portugol2C pt2c(stable);
        if(debug_flag) {
          cerr << "BEGIN SOURCE >>>> \n";
          cerr << pt2c.algoritmo(tree) << endl;
          cerr << "<<<<<< END SOURCE\n";          
        } else {
          out << pt2c.algoritmo(tree) << endl;          
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

  cfilepath = tempnam(0L, "c__");

  fout.open(cfilepath, ios_base::out);
  if(!fout) {
    cerr << PACKAGE << ": não foi possível abrir o arquivo: \"" << ofilepath << "\"" << std::endl;
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

  if(debug_flag) {cerr << cmd.str();}

  if(system(cmd.str().c_str()) == -1) {
    cerr << PACKAGE << ": não foi possível invocar gcc." << endl;
    return false;
  }
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
    if(!debug_flag && !interpret_only) {
      success = compile()?EXIT_SUCCESS:EXIT_FAILURE;
    }
  } else {
    success = EXIT_FAILURE;
  }

  the_end:
    unlink(cfilepath);
    free(cfilepath);
    return success;
}
