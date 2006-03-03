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

#include "Display.hpp"
#include "GPT.hpp"

#include <sstream>
#include <unistd.h>

#define DEFAULT_PORT "7680"

using namespace std;

enum {
  FLAG_PIPE  = 0x1,
  FLAG_DICA  = 0x2
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
string _ifilename;
string _host;
string _port = DEFAULT_PORT;

#ifdef WIN32
  string _ofilename = "a.exe";
#else
  string _ofilename = "a.out";
#endif

//----- Options -----

int init(int argc, char** argv) {

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

  while((c = getopt(argc, argv, "o:t:s:H:P:idvh")) != -1) {
    switch(c) {
      case 'o':
        if(optarg) {
          _ofilename = optarg;
        }
        break;
      case 't':
        count_cmds++;
        cmd = CMD_GPT_2_C;
        if(optarg) {
          _ofilename = optarg;
        }
        break;
      case 's':
        count_cmds++;
        cmd = CMD_GPT_2_ASM;
        if(optarg) {
          _ofilename = optarg;
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
      case 'p':
        _flags |= FLAG_PIPE;
        break;
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
           Display::self()->showError(s);
           goto bail;
     default:
        s << PACKAGE << ": erro interno." << endl;
        Display::self()->showError(s);
        goto bail;
    }
  }

  if(count_cmds > 1) {
    s << PACKAGE << ": mais de um comando selecionado." << endl;
    Display::self()->showError(s);
    goto bail;
  }

  if(!(_flags & FLAG_PIPE)) { //no pipe
    if(optind < argc) {
      _ifilename = argv[optind];
    } else {
      s << PACKAGE << ": nenhum arquivo especificado." << endl;
      Display::self()->showError(s);
      goto bail;
    }
  }

  if(CMD_INTERPRET == cmd) {
    if((_port != DEFAULT_PORT) && (atoi(_port.c_str()) == 0)) {
      s << PACKAGE << ": porta de conexão inválida: \"" << _port << "\"" << endl;
      Display::self()->showError(s);
      goto bail;
    }
  }

  return cmd;

  bail:
    return CMD_INVALID;
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

  switch(cmd) {
    case CMD_SHOW_VERSION:
      GPT::self()->showVersion();
      break;
    case CMD_SHOW_HELP:
      GPT::self()->showHelp();
      break;
    case CMD_COMPILE:
      success = GPT::self()->compile(_ifilename, _ofilename);
      break;
    case CMD_GPT_2_C:
      success = GPT::self()->translate2C(_ifilename, _ofilename);
      break;
    case CMD_GPT_2_ASM:
      success = GPT::self()->compile(_ifilename, _ofilename, false);
      break;
    case CMD_INTERPRET:
      success = GPT::self()->interpret(_ifilename, _host, atoi(_port.c_str()));
      break;
    case CMD_INVALID:
      break;
  }

  exit(success?EXIT_SUCCESS:EXIT_FAILURE);
}
