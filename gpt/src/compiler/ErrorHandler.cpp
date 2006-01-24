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
#include "ErrorHandler.hpp"

#include <iostream>

using namespace std;

ErrorHandler* ErrorHandler::_self = 0L;

ErrorHandler::ErrorHandler()
  : _hasError(false), _stopOnError(false)
{
}

ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::stopOnError(bool val) {
  _stopOnError = val;
}

bool ErrorHandler::hasError() {
  return _hasError;
}

void ErrorHandler::showErrors(bool showTips) {
  errors_map_t::iterator it;
  for(it = _errors.begin(); it != _errors.end(); ++it) {
    for(list<ErrorMsg>::iterator lit = it->second.begin(); lit != it->second.end(); ++lit) {
      showError((*lit));
      if(showTips && (*lit).hasTip) {
        showTip((*lit));
      }           
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
  if(_stopOnError) throw UniqueErrorException(msg, line);


  ErrorMsg err;
  err.line = line;
  err.msg = msg;

  _errors[line].push_back(err);

  return _errors[line].size();
}

void ErrorHandler::showError(ErrorMsg& err) {
  cerr << "Linha: " << err.line << " - " << err.msg << "." << endl;
}


void ErrorHandler::showTip(ErrorMsg& err) {
  cerr << "\tDica: " << err.tip << "." << endl;
}

void ErrorHandler::addTip(const string& msg, int line, int cd) {    
  list<ErrorMsg>::iterator it = _errors[line].begin();

  for(int i = 0; i < cd-1; ++i,++it);
  (*it).hasTip = true;
  (*it).tip = msg;
}

ErrorHandler::ErrorMsg ErrorHandler::getFirstError() {
  return *(_errors.begin()->second.begin());
}

void ErrorHandler::clear() {
  _errors.clear();
  _hasError = false;
}
