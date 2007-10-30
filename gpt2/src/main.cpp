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

using namespace std;

enum {
  FLAG_DICA  = 0x1
};

enum {
  CMD_SHOW_VERSION,
  CMD_SHOW_HELP,
  CMD_COMPILE,
  CMD_INVALID
};

//----- globals ------

int    _flags = 0;
list<string> _ifilenames;

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
#endif
      case 'o':
        count_cmds++;
        cmd = CMD_COMPILE;
        if(optarg) {
          _binprogram = optarg;
        }
        break;
      case 'd':
        _flags |= FLAG_DICA;
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

    c = optind;
    while(c < argc) {
      _ifilenames.push_back(argv[c++]);
    }

    if(_ifilenames.size() == 0) {
      s << PACKAGE << ": nenhum arquivo especificado." << endl;
      GPTDisplay::self()->showError(s);
      goto bail;
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

  appendDefaultFiles();


  switch(cmd) {
    case CMD_SHOW_VERSION:
      GPT::self()->showVersion();
      break;
    case CMD_SHOW_HELP:
      GPT::self()->showHelp();
      break;
    case CMD_COMPILE:
      if(!_binprogram.empty()) {
        GPT::self()->setOutputFile(_binprogram);
      }
      success = GPT::self()->compile(_ifilenames);
      break;
    case CMD_INVALID:
      break;
  }

  return success?EXIT_SUCCESS:EXIT_FAILURE;
}

