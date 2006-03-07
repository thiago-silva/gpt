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
#include "GPTDisplay.hpp"

#ifdef WIN32
  #include <windows.h>
#endif

#include <iostream>

using namespace std;

GPTDisplay* GPTDisplay::_self = 0L;

GPTDisplay::GPTDisplay()
  : MAX_ERRORS(10), _totalErrors(0), _stopOnError(false), _showTips(false)
{
}

GPTDisplay::~GPTDisplay()
{
}

int GPTDisplay::totalErrors() {
  return _totalErrors;
}

string GPTDisplay::toOEM(const string& str) {
  #ifdef WIN32
    string ret;
    char buffer [str.length()];
    CharToOem(str.c_str(), buffer);
    ret = buffer;
    return ret;
  #else
    return str;
  #endif
}

void GPTDisplay::showError(stringstream& s) {
  cerr << toOEM(s.str());
}

void GPTDisplay::showError(const string& str) {
  cerr << toOEM(str) << endl;
}

void GPTDisplay::showMessage(stringstream& s) {
  cout << toOEM(s.str());
  cout.flush();
}


void GPTDisplay::stopOnError(bool val) {
  _stopOnError = val;
}

bool GPTDisplay::hasError() {
  return _totalErrors > 0;
}

void GPTDisplay::showErrors() {
  errors_map_t::iterator it;
  for(it = _errors.begin(); it != _errors.end(); ++it) {
    for(list<ErrorMsg>::iterator lit = it->second.begin(); lit != it->second.end(); ++lit) {
      showError((*lit));
      if(_showTips && (*lit).hasTip) {
        showTip((*lit));
      }
    }
  }
}

GPTDisplay* GPTDisplay::self() {
  if(!GPTDisplay::_self) {
    GPTDisplay::_self = new GPTDisplay();
  }
  return GPTDisplay::_self;
}

int GPTDisplay::add(const string& msg, int line) {
  _totalErrors++;
  if(_stopOnError) throw UniqueErrorException(msg, line);

  if(totalErrors() > MAX_ERRORS) {
    showErrors();
    exit(1);
  }

  ErrorMsg err;
  err.line = line;
  err.msg = msg;

  _errors[line].push_back(err);

  return _errors[line].size();
}

void GPTDisplay::showError(ErrorMsg& err) {
  stringstream s;
  s << "Linha: " << err.line << " - " << err.msg << "." << endl;
  showError(s);
}


void GPTDisplay::showTip(ErrorMsg& err) {
  stringstream s;
  s << "\tDica: " << err.tip << "." << endl;
  showError(s);
}

void GPTDisplay::addTip(const string& msg, int line, int cd) {
  list<ErrorMsg>::iterator it = _errors[line].begin();

  for(int i = 0; i < cd-1; ++i,++it);
  (*it).hasTip = true;
  (*it).tip = msg;
}

GPTDisplay::ErrorMsg GPTDisplay::getFirstError() {
  return *(_errors.begin()->second.begin());
}

void GPTDisplay::showTips(bool value) {
  _showTips = value;
}

void GPTDisplay::clear() {
  _errors.clear();
  _totalErrors = 0;
}
