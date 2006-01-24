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

#include "debuggerstack.h"


DebuggerExecutionPoint::DebuggerExecutionPoint()
  : m_id(0), m_url(), m_line(0), m_function(0)
{
}

DebuggerExecutionPoint::DebuggerExecutionPoint(int id, const KURL& url, int line, const QString& function)
  : m_id(id), m_url(url), m_line(line), m_function(function)
{
}

DebuggerExecutionPoint::~DebuggerExecutionPoint()
{
}

void DebuggerExecutionPoint::setId(int id)
{
  m_id = id;
}

int DebuggerExecutionPoint::id() const
{
  return m_id;
}

void DebuggerExecutionPoint::setURL(const KURL& url)
{
  m_url = url;
}

KURL DebuggerExecutionPoint::url() const
{
  return m_url;
}

void DebuggerExecutionPoint::setLine(int line)
{
  m_line = line;
}

int DebuggerExecutionPoint::line() const 
{
  return m_line;
}

void DebuggerExecutionPoint::setFunction(const QString& function)
{
  m_function = function;
}

QString DebuggerExecutionPoint::function() const
{
  return m_function;
}

//-----------------------------------------------------------

DebuggerStack::DebuggerStack()
{
}

DebuggerStack::~DebuggerStack()
{
  DebuggerExecutionPoint *execPoint;
  for(execPoint = m_execPointList.first(); execPoint; execPoint = m_execPointList.next()) {
    delete execPoint;
  }
}

void DebuggerStack::push(int id, const KURL& url, int line, const QString& function)
{
  DebuggerExecutionPoint *execPoint =
    new DebuggerExecutionPoint(id, url, line, function);
  m_execPointList.prepend(execPoint);
}

void DebuggerStack::push(DebuggerExecutionPoint* execPoint)
{
  m_execPointList.prepend(execPoint);
}

void DebuggerStack::insert(int id, const KURL& url, int line, const QString& function)
{
  DebuggerExecutionPoint *execPoint =
      new DebuggerExecutionPoint(id, url, line, function);
  m_execPointList.append(execPoint);
}

void DebuggerStack::insert(DebuggerExecutionPoint* execPoint)
{
  m_execPointList.append(execPoint);
}

  
DebuggerExecutionPoint* DebuggerStack::bottomExecutionPoint()
{
  return m_execPointList.getLast();
}

DebuggerExecutionPoint* DebuggerStack::topExecutionPoint()
{
  return m_execPointList.getFirst();
}

DebuggerStack::DebuggerExecutionPointList_t DebuggerStack::debuggerExecutionPointList()
{
  return m_execPointList;
}
