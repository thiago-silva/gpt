/***************************************************************************
 *   Copyright (C) 2005 by Thiago Silva                                    *
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
#include "ErrorHandler.hpp"

#include <iostream>

ErrorHandler* ErrorHandler::_self = 0L;

ErrorHandler::ErrorHandler()
  : _hasError(false)
{
}

ErrorHandler::~ErrorHandler()
{
}

bool ErrorHandler::hasError() {
  return _hasError;
}

void ErrorHandler::showErrors() {
  errors_map_t::iterator it;
  for(it = _errors.begin(); it != _errors.end(); ++it) {
    for(list<string>::iterator lit = it->second.begin(); lit != it->second.end(); ++lit) {
      cerr << (*lit);
    }
  }
}

ErrorHandler* ErrorHandler::self() {
  if(!ErrorHandler::_self) {
    ErrorHandler::_self = new ErrorHandler();
  }
  return ErrorHandler::_self;
}

int ErrorHandler::add(const string& msg, int line) {
  _hasError = true;

  stringstream s;
  s << "Linha: " << line << " - " << msg << "." << endl;
  _errors[line].push_back(s.str());
  return _errors[line].size();
}

int ErrorHandler::add(const stringstream& msg, int line) {
  _hasError = true;
  stringstream s;
  s << "Linha: " << line << " - " <<  msg.str() << "." << endl;
  _errors[line].push_back(s.str());

  return _errors[line].size();
}

void ErrorHandler::addTip(const string& msg, int line, int cd) {  
  stringstream s;
  s << "\tDica: " << msg << "." << endl;
  list<string>::iterator it = _errors[line].begin();

   for(int i = 0; i < cd; ++i,++it);
//    ++it;

  _errors[line].insert(it,s.str());
}

void ErrorHandler::addFatal(const string& msg) {
  cerr << msg << "." << endl;
}

void ErrorHandler::addFatal(const stringstream& msg) {
  cerr << msg.str() << "." << endl;
}
