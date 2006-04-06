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

#include "GPTDisplay.hpp"
#include "GPT.hpp"

#include <sstream>
#include <string>
#include <list>
#include <unistd.h>

#define DEFAULT_PORT "7680"

using namespace std;

enum {  
  FLAG_DICA  = 0x1,
  FLAG_PRINT_AST = 0x2
  //FLAG_PIPE  = 0x1,
};

enum {
  CMD_SHOW_VERSION,
  CMD_SHOW_HELP,
  CMD_COMPILE,
  CMD_GPT_2_C,
  CMD_GPT_2_ASM,
  CMD_INTERPRET,
  CMD_INVALID
};

//----- globals ------

int    _flags = 0;
list<string> _ifilenames;

string _host;
string _port = DEFAULT_PORT;

string _csource;
string _asmsource;
string _binprogram;

//----- Options -----

static int init(int argc, char** argv) {

  if(argc == 1) {
    return CMD_SHOW_HELP;
  }

  stringstream s;
  int cmd = CMD_COMPILE;
  opterr = 0;
  int c;
  int count_cmds = 0;

/*
  Opcoes:  o: <output>,  t: <output>,  s: <output>, H: <host>,  P: <port>,  h[help]
           v[ersion],  i[nterpret],  p[ipe],  d[ica]
*/

#ifndef DEBUG
  while((c = getopt(argc, argv, "o:c:s:H:P:idvh")) != -1) {
    switch(c) {
#else
  while((c = getopt(argc, argv, "o:t:s:H:P:idvhD")) != -1) {
    switch(c) {
      case 'D':
        _flags |= FLAG_PRINT_AST;
        break;
#endif
      case 'o':
        count_cmds++;
        cmd = CMD_COMPILE;
        if(optarg) {
          _binprogram = optarg;
        }
        break;        
      case 't':
        count_cmds++;
        cmd = CMD_GPT_2_C;
        if(optarg) {
          _csource = optarg;
        }
        break;
      case 's':
        count_cmds++;
        cmd = CMD_GPT_2_ASM;
        if(optarg) {
          _asmsource = optarg;
        }
      case 'H':
        if(optarg) {
          _host = optarg;
        }
        break;
      case 'P':
        if(optarg) {
          _port = optarg;
        }
        break;
      case 'i':
        count_cmds++;
        cmd = CMD_INTERPRET;
        break;
      case 'd':
        _flags |= FLAG_DICA;
        break;
//       case 'p':
//         _flags |= FLAG_PIPE;
//         break;
      case 'v':
        return CMD_SHOW_VERSION;
        break;
      case 'h':
        return CMD_SHOW_HELP;
        case '?':
           if((optopt == 'o') || (optopt == 't')) {
            s << PACKAGE << ": faltando argumento para opção -" << (char)optopt << endl;
           } else {
            s << PACKAGE << ": opção inválida: -" <<  char(optopt) << endl;
           }
           GPTDisplay::self()->showError(s);
           goto bail;
     default:
        s << PACKAGE << ": erro interno." << endl;
        GPTDisplay::self()->showError(s);
        goto bail;
    }
  }

  if(count_cmds > 1) {
    s << PACKAGE << ": mais de um comando selecionado." << endl;
    GPTDisplay::self()->showError(s);
    goto bail;
  }

//   if(!(_flags & FLAG_PIPE)) { //no pipe
    while(optind < argc) {
      _ifilenames.push_back(argv[optind++]);
    }

    if(_ifilenames.size() == 0) {
      s << PACKAGE << ": nenhum arquivo especificado." << endl;
      GPTDisplay::self()->showError(s);
      goto bail;
    }
//   }

  if(CMD_INTERPRET == cmd) {
    if((_port != DEFAULT_PORT) && (atoi(_port.c_str()) == 0)) {
      s << PACKAGE << ": porta de conexão inválida: \"" << _port << "\"" << endl;
      GPTDisplay::self()->showError(s);
      goto bail;
    }
  }

  return cmd;

  bail:
    return CMD_INVALID;
}


static void appendDefaultFiles() {
  string inc;

  char* env = getenv("GPT_INCLUDE");  

  if(!env || strlen(env) == 0) {
    return;
  }

  inc = env;

  string filename;
  string::size_type c = 0;
  string::size_type b = 0;
  while((c = inc.find(":", b)) != string::npos) {
    filename = inc.substr(b, c);
    if(filename.length()) {
      _ifilenames.push_back(filename);
    }
    b = c+1;
  }
  filename = inc.substr(b);
  if(filename.length()) {
    _ifilenames.push_back(filename);
  }
}

int main(int argc, char** argv) {
  int cmd = init(argc, argv);
  bool success = false;
  stringstream s;

  if(_flags & FLAG_DICA) {
    GPT::self()->reportDicas(true);
  } else {
    GPT::self()->reportDicas(false);
  }

  if(_flags & FLAG_PRINT_AST) {
    GPT::self()->printParseTree(true);
  }

//   if(_flags & FLAG_PIPE) {
//     GPT::self()->usePipe(true);
//   }

  appendDefaultFiles();


  switch(cmd) {
    case CMD_SHOW_VERSION:
      GPT::self()->showVersion();
      break;
    case CMD_SHOW_HELP:
      GPT::self()->showHelp();
      break;
    case CMD_COMPILE:
      GPT::self()->setOutputFile(_binprogram);
      success = GPT::self()->compile(_ifilenames);
      break;
    case CMD_GPT_2_C:
      GPT::self()->setOutputFile(_csource);
      success = GPT::self()->translate2C(_ifilenames);
      break;
    case CMD_GPT_2_ASM:
      GPT::self()->setOutputFile(_asmsource);
      success = GPT::self()->compile(_ifilenames, false);
      break;
    case CMD_INTERPRET:
      success = GPT::self()->interpret(_ifilenames, _host, atoi(_port.c_str()));
      break;
    case CMD_INVALID:
      break;
  }

  exit(success?EXIT_SUCCESS:EXIT_FAILURE);
}

/*
static void figureOutputFileName() {
  if(!_ifilename.empty()) { //temos nome do arquivo de entrada
    if(_ofilename.empty()) { //arquivo de saida nao foi setado
      string fname;
      string::size_type dirsep;
      
      #ifdef WIN32
        if((dirsep = _ifilename.rfind('\\')) == string::npos) {
          dirsep = _ifilename.rfind('/');
        }
      #else
        dirsep = _ifilename.rfind('/');
      #endif
      
      if(dirsep == string::npos) {
        fname = _ifilename;
      } else {
        fname = _ifilename.substr(dirsep+1);
      }
          
      string::size_type dotpos;
      if((dotpos = fname.rfind('.')) != string::npos) {
        #ifdef WIN32
          _ofilename = fname.substr(0, dotpos) + ".exe";
        #else
          _ofilename = fname.substr(0, dotpos);
        #endif
      } else {
        #ifdef WIN32
          _ofilename = fname + ".exe";
        #else
          _ofilename = fname + ".out";
        #endif
      }
    } //no else
  } else if(_ofilename.empty()) { //nao temos nome de arquivo de entrada, nem de saida
    //colocar defaults
    #ifdef WIN32
      string _ofilename = "a.exe";
    #else
      string _ofilename = "a.out";
    #endif
  }
}*/
