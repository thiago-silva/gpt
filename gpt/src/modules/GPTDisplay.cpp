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

string GPTDisplay::toLatin1(const string& str) {
	char c = 0;
	string ret;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == 0xffffffc3) {
			c = 1;
			continue;
		}
		if (c) {
			ret += str[i] | 0x40;
		} else {
			ret += str[i];
		}
		c = 0;
	}
	return ret;
}

string GPTDisplay::toOEM(const string& str) {
  #ifdef WIN32
    string ret;
    char buffer [str.length()];
    CharToOem(toLatin1(str.c_str()).c_str(), buffer);
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
  errors_map_t::reverse_iterator it;
  for(it = _errors.rbegin(); it != _errors.rend(); ++it) {
    for(map<int, list<ErrorMsg> >::iterator ll = it->second.begin(); ll != it->second.end(); ++ll) {
      for(list<ErrorMsg>::iterator lit = ll->second.begin(); lit != ll->second.end(); ++lit) {
        showError((*lit));
        if(_showTips && (*lit).hasTip) {
          showTip((*lit));
        }
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

void GPTDisplay::addFileName(const string& str)
{
  static int c = 0;
  _file_map[str] = c++;
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
  err.file = _currentFile;

  //_errors[line].push_back(err);
  _errors[_file_map[_currentFile]][line].push_back(err);

  return _errors[_file_map[_currentFile]][line].size();
  //return _errors[line].size();
}

void GPTDisplay::showError(ErrorMsg& err) {
  stringstream s;
  s << err.file << ":" << err.line << " - " << err.msg << "." << endl;
  showError(s);
}


void GPTDisplay::showTip(ErrorMsg& err) {
  stringstream s;
  s << "\tDica: " << err.tip << "." << endl;
  showError(s);
}

void GPTDisplay::addTip(const string& msg, int line, int cd) {
  //list<ErrorMsg>::iterator it = _errors[line].begin();
  list<ErrorMsg>::iterator it = _errors[_file_map[_currentFile]][line].begin();

  for(int i = 0; i < cd-1; ++i,++it);
  (*it).hasTip = true;
  (*it).tip = msg;
}

void GPTDisplay::setCurrentFile(const string& file)
{
  _currentFile = file;
}

string GPTDisplay::getCurrentFile()
{
  return _currentFile;
}

GPTDisplay::ErrorMsg GPTDisplay::getFirstError() {
  return *(_errors.begin()->second.begin()->second.begin());
}

void GPTDisplay::showTips(bool value) {
  _showTips = value;
}

void GPTDisplay::clear() {
  _errors.clear();
  _totalErrors = 0;
}
