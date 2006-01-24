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

#ifndef DEBUGGERSTACK_H
#define DEBUGGERSTACK_H

#include <qptrlist.h>
#include <qstring.h>

#include <kurl.h>

/* represents a point of execution (file/line) with extra data such as functionName...
   Should be expanded in the future
*/

class DebuggerExecutionPoint {
public:
  DebuggerExecutionPoint();
  DebuggerExecutionPoint(int id, const KURL& filePath, int line, const QString& function);
  ~DebuggerExecutionPoint();

  void setId(int);
  int  id() const;

  void setURL(const KURL&);
  KURL url() const;

  void setLine(int);
  int line() const;

  void setFunction(const QString&);
  QString function() const;
private:
  int m_id;
  KURL m_url;
  int m_line;
  QString m_function;
};

//---------------------------------------------------------------------

/* represents all data in a backtrace.
   Its used mainly by the Stack ComboBox, and also is responsable to inform
   where is the active point of execution.
*/
class DebuggerStack{
public:
  typedef QPtrList<DebuggerExecutionPoint> DebuggerExecutionPointList_t;
  DebuggerStack();
  ~DebuggerStack();

  void push(int id, const KURL& url, int line, const QString& function);
  void push(DebuggerExecutionPoint*);

  void insert(int id, const KURL& url, int line, const QString& function);
  void insert(DebuggerExecutionPoint*);
  
  DebuggerExecutionPoint*      bottomExecutionPoint();
  DebuggerExecutionPoint*      topExecutionPoint();
  DebuggerExecutionPointList_t debuggerExecutionPointList();
private:
  DebuggerExecutionPointList_t m_execPointList;
};

#endif
