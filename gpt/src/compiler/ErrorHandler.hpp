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

#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <map>
#include <list>
#include <string>
#include <sstream>

using namespace std;

class ErrorHandler {
public:
  ~ErrorHandler();

  static ErrorHandler* self();

  int add(const string& msg, int line);  
//   int add(const string& msg);  
  int add(const stringstream& msg, int line);
//   int add(const stringstream& msg);

  void addTip(const string& msg, int line, int cd);

  void addFatal(const string&);
  void addFatal(const stringstream&);

  void showErrors();
  bool hasError();
private:
  ErrorHandler();

  static ErrorHandler* _self;
  bool _hasError;

  typedef map<int, list<string> > errors_map_t;
  errors_map_t _errors;
};

#endif
