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
#include "Display.hpp"

#ifdef WIN32
  #include <windows.h>
#endif

#include <iostream>

using namespace std;

Display* Display::_self = 0L;

Display::Display()
  : _hasError(false), _stopOnError(false)
{
}

Display::~Display()
{
}

string Display::toOEM(const string& str) {
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

void Display::showError(stringstream& s) {
  cerr << toOEM(s.str());
}

void Display::showError(const string& str) {
  cerr << toOEM(str) << endl;
}

void Display::showMessage(stringstream& s) {
  cout << toOEM(s.str());
  cout.flush();
}


void Display::stopOnError(bool val) {
  _stopOnError = val;
}

bool Display::hasError() {
  return _hasError;
}

void Display::showErrors(bool showTips) {
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

Display* Display::self() {
  if(!Display::_self) {
    Display::_self = new Display();
  }
  return Display::_self;
}

int Display::add(const string& msg, int line) {
  _hasError = true;
  if(_stopOnError) throw UniqueErrorException(msg, line);


  ErrorMsg err;
  err.line = line;
  err.msg = msg;

  _errors[line].push_back(err);

  return _errors[line].size();
}

void Display::showError(ErrorMsg& err) {
  stringstream s;
  s << "Linha: " << err.line << " - " << err.msg << "." << endl;
  showError(s);
}


void Display::showTip(ErrorMsg& err) {
  stringstream s;
  s << "\tDica: " << err.tip << "." << endl;
  showError(s);
}

void Display::addTip(const string& msg, int line, int cd) {
  list<ErrorMsg>::iterator it = _errors[line].begin();

  for(int i = 0; i < cd-1; ++i,++it);
  (*it).hasTip = true;
  (*it).tip = msg;
}

Display::ErrorMsg Display::getFirstError() {
  return *(_errors.begin()->second.begin());
}

void Display::clear() {
  _errors.clear();
  _hasError = false;
}
