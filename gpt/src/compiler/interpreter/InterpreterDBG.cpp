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

#include "config.h"

#include "InterpreterDBG.hpp"
#include "InterpreterEval.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
//#include <errno.h>
#include <netdb.h>
//#include <unistd.h> //close
#include <pcrecpp.h>

void sigPipeHandler(int signum) {
  cerr << "Con closed!!\n";
  InterpreterDBG::self()->closeSock();
}

InterpreterDBG* InterpreterDBG::singleton = NULL;

InterpreterDBG::InterpreterDBG() 
: clientsock(-1) {}


InterpreterDBG* InterpreterDBG::self() {
  if(!singleton) {
    singleton = new InterpreterDBG;
  }
  return singleton;
}

void InterpreterDBG::init(string host, int port) {

  if(host.length() == 0) return;

  struct sockaddr_in name;

  struct hostent *hostinfo = NULL;    
    
  name.sin_family = AF_INET;
  name.sin_port = htons (port);
  hostinfo = gethostbyname (host.c_str());

  if (hostinfo == NULL)
  {
    //cerr << "client: unknow host : "  << hostname << endl;
    return;
  }

  name.sin_addr = *(struct in_addr *) hostinfo->h_addr;    
  
  clientsock = socket(PF_INET, SOCK_STREAM, 0);
  if(clientsock < 0) {
    cerr << PACKAGE << ": não foi possível criar socket\n";
    return;
  }

  if(connect(clientsock, (struct sockaddr*) &name,sizeof(name)) != 0) { 
    //cerr << PACKAGE << ": unable to connect" << endl;
    //unable to connect
    closeSock();
  } else {
    if(SIG_ERR == signal(SIGPIPE, sigPipeHandler)) {
      cerr << PACKAGE << ": erro interno em InterpreterDBG::init" << endl;
    }
  }
}

int InterpreterDBG::nextCmd(int line, Variables& v, list<pair<string, int> >& stk) {
  
  sendStackInfo(stk);

  sendVariables(v.getGlobals(), stk, true);
  sendVariables(v.getLocals(), stk, false);

  return receiveCmd();  
}

void InterpreterDBG::sendStackInfo(list<pair<string, int> >& stk) {
  if(clientsock < 0) return;

  stringstream s;
  s << "<stackinfo>";
  int id = 0;
  for(list<pair<string, int> >::reverse_iterator it = stk.rbegin(); it != stk.rend(); ++it) {
    s << "<entry id=\"" << id++
      << "\" function=\"" << (*it).first 
      << "\" line=\"" << (*it).second << "\"/>";
  }
  s << "</stackinfo>";

  sendData(s);
}

void InterpreterDBG::sendVariables(map<string, Variable> globals, list<pair<string, int> >& stk, bool globalScope) {
  if(clientsock < 0) return;

  stringstream s;
  s << "<vars for=\"" << (globalScope?"$global":"$local") 
    << "\" scope=\"" << (globalScope?stk.front().first:stk.back().first) << "\">";  

  bool primitive;
  for(map<string, Variable>::iterator it = globals.begin(); it != globals.end(); ++it) {  
    primitive = it->second.isPrimitive;
    s << "<var name=\"" << it->first << "\" type=\"" << Symbol::typeToString(it->second.type) 
      << "\" primitive=\"" << (primitive?"true":"false") << "\"";
    if(primitive) {
      s << " value=\"" << it->second.primitiveValue << "\"/>";
    } else {
      stringstream vv;
      s << "><values>";

//       for(list<int>::iterator dit = it->second.dimensions.begin(); dit != it->second.dimensions.end(); ++dit) {
        s << matrixValuesNodes(0, it->second.dimensions.size(), it->second.values, it->second.dimensions, it->second.type);
//         level++;
//       }
      s << "</values></var>";

/*    s << ">"
      << "<dimensions>";
      int level = 0;
      for(list<int>::iterator dit = it->second.dimensions.begin(); dit != it->second.dimensions.end(); ++dit) {
        s << "<dimension level=\"" << level << "\" size=\"" << (*dit) << "\"/>";
        level++;
      }
      s << "</dimensions><values>";
      
      for(map<string, string>::iterator vit = it->second.values.begin(); vit != it->second.values.end(); vit++) {
        s << "<value index=\"" <<   vit->first << "\" value=\"" << vit->second << "\"/>";  
      }
      s << "</values></var>";*/
    }
  }
  s << "</vars>";

  sendData(s);
}

string InterpreterDBG::matrixValuesNodes(uint level, int dimsize, 
    map<string, string>& values, list<int>& dims, int type, string vindex /*= "" */)
{
  list<int>::iterator lsize;
  uint idx = 0;
  for(lsize = dims.begin(); lsize != dims.end(); lsize++, idx++) {
    if(idx == level) {
      break;
    }
  }


  stringstream s;
  stringstream vs;  
  for(int i = 0; i < (*lsize); i++) {
    vs.str("");
    vs << vindex;
    if(vindex.length()==0) {
      vs << i;
    } else {
      vs << ":" << i;
    }
    
    if(level == (dims.size()-1))  {
			string val = values[vs.str()];
			if(val.length()==0) {
				switch(type) {
					case TIPO_INTEIRO:
					case TIPO_REAL:
					case TIPO_CARACTERE:
				  case TIPO_LOGICO:
						val = "0";
						break;
				}
			}
      s << "<var name=\"" << i << "\" primitive=\"true\" value=\"" << val << "\"/>";
    } else {


      s << "<var name=\"" << i << "\" primitive=\"false\"><values>";
      s << matrixValuesNodes(level+1, dimsize, values, dims, type, vs.str());
      s << "</values></var>";
    }
  }
  return s.str();
}

void InterpreterDBG::sendData(stringstream& s) {
  if(clientsock < 0) return;

  string str = s.str();
  s.rdbuf()->str("");
  s << str.length() << '\0' << str;
  if(send(clientsock, s.str().c_str(), sizeof(char)* (s.str().length()+1), 0) < 0) {//error
    if(clientsock > 0) {
      cerr << PACKAGE << ": erro ao enviar dados." << endl;
    } //else, server closed con
  }
}

string InterpreterDBG::receiveIncomingData() {
  char buffer[1024];
  int received = 0;

	buffer[1023] = '\0';

	string cmd;
	int msg_size;

	//retrieve msg size!
	int idx = 0;
	int rec;
	while(true) {
		rec = recv(clientsock, &buffer[idx], sizeof(char), 0);
		received += rec;

		if((rec == 0) || (rec < 1)) {
			return cmd;
		}

		if(buffer[idx] == '\0') {
			break;
		}
		idx++;
	}

// 	cerr << "MESSAGE SIZE: " << buffer << ", received : " << received << endl;
	
	msg_size = atoi(buffer) * sizeof(char);

	received = recv(clientsock, buffer, msg_size, 0);
	if(received < 0) {
		cerr << PACKAGE << ": erro ao receber dados." << endl;
		return cmd;
	} else if((received == 0) || (received < msg_size)) {
// 		cerr << "insuficient! received: " << received << ", msg_size: " << msg_size << endl;
		closeSock();
		return cmd;
	}

// 	cmd.assign(buffer, received);
	cmd = buffer;
// 	cerr << "MESSAGE : " << cmd << ", assigned: " << received << endl;

	return cmd;
}

int InterpreterDBG::receiveCmd() {
  if(clientsock < 0) {
    return CMDContinue;
  }

	string cmd = receiveIncomingData();

  int ret;

  if(cmd == "step_into") {
    ret = CMDStepInto;
  } else if(cmd == "step_over") {
    ret = CMDStepOver;
  } else if(cmd == "step_out") {  
    ret = CMDStepOut;
  } else if(cmd == "continue") {
    ret = CMDContinue;
	} else if(cmd.find("breakpoint") != string::npos) {
		processBreakpointCMD(cmd);
		ret = receiveCmd();
	} else {
    cerr << PACKAGE << ": comando desconhecido: \"" << cmd << "\"" << endl;
    ret = CMDStepInto;
  }

  return ret;
}

void InterpreterDBG::processBreakpointCMD(string& cmd) {
// 	cerr << "process breakpoint " << cmd << endl;

	string what;
  int line;
	pcrecpp::RE re("breakpoint cmd=(add|remove).*line=(\\d+)");
	if(!re.FullMatch(cmd, &what, &line)) {
		cerr << PACKAGE << ": comando invalido (2): \"" << cmd << "\"" << endl;
		return;
	}

	if(what == "add") {
		breakpoints.push_back(line);
	} else if(what == "remove") {
		breakpoints.remove(line);		
	} else {
		cerr << PACKAGE << ": comando invalido: (3) \"" << cmd << "\"" << endl;
		return;
	}
}

bool InterpreterDBG::breakOnLine(int line) {	
  for(list<int>::iterator it = breakpoints.begin(); it != breakpoints.end(); ++it) {
// 		cerr << "breakon checkin for " << *it << " on " << line << endl;
    if(*it == line) {
// 			cerr << "breakon BREAK on " << *it << endl;
      return true;
    }
  }

// 	cerr << "breakon nops on " << line << endl;
  return false;
}

void InterpreterDBG::closeSock() {
  shutdown(clientsock, SHUT_RDWR);
  close(clientsock);
  clientsock = -1;
}
