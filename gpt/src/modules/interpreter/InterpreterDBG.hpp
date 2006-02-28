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

#ifndef NET_HPP
#define NET_HPP
#include <list>
#include <string>
#include <sstream>
#include <map>

#ifdef WIN32
  #include <winsock.h>
#endif

using namespace std;

class Variables;
class Variable;

class InterpreterDBG {
public:

  enum { CMDStepInto, CMDStepOver, CMDStepOut, CMDContinue };

  void init(string host, int port);

  int nextCmd(int line, Variables& v, list<pair<string, int> >& stk);

  static InterpreterDBG* self();

  void closeSock();

  bool breakOnLine(int line);

private:
  InterpreterDBG();

  static InterpreterDBG* singleton;

  void sendStackInfo(list<pair<string, int> >& stk);
  void sendVariables(map<string, Variable> globals, list<pair<string, int> >& stk, bool globalScope);
  int receiveCmd();

  string receiveIncomingData();

  void processBreakpointCMD(string& cmd);
  void addBreakpoint(string& cmd);

  void sendData(stringstream& s);
  void removeBreakpoint(string& cmd);

  string matrixValuesNodes(unsigned int level, int dimsize,
    map<string, string>& values, list<int>& dims, int type, string vindex = "");

#ifndef WIN32
  int clientsock;
#else
  SOCKET clientsock;
#endif

  list<int> breakpoints;
  string currentCmd;
};

#endif
