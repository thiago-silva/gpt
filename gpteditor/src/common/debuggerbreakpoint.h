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


#ifndef DEBUGGERBREAKPOINT_H
#define DEBUGGERBREAKPOINT_H
#include <qstring.h>
#include <kurl.h>

class DebuggerBreakpoint{
public:
  enum { ENABLED, DISABLED, UNRESOLVED };

  DebuggerBreakpoint();
  DebuggerBreakpoint(const DebuggerBreakpoint&);

  DebuggerBreakpoint(int id, const KURL& url
                     , int line
                     , int status = ENABLED
                     , const QString& conditionExpr = ""
                     , int hitCount = 0
                     , int skipHits = 0);

  virtual ~DebuggerBreakpoint();

  virtual void setId(int);
  virtual void setURL(const KURL& url);
  virtual void setLine(int line);
  virtual void setCondition(const QString& expression);
  virtual void setStatus(int status);
  virtual void setHitCount(int hitCount);
  virtual void setSkipHits(int skipHits);

  virtual int id();
  virtual const KURL& url();
  virtual int line();
  virtual const QString& condition();
  virtual int status();
  virtual int hitCount();
  virtual int skipHits();

  //compare only relevant members: line/file
  virtual bool compare(DebuggerBreakpoint*);
  virtual bool compare(const KURL&, int);

protected:
  
  //ID is not currently used (only internaly by the debugger clients that needs it)
  
  int     m_id; 
  KURL    m_url;
  int     m_line;
  int     m_status;
  QString m_conditionExpr;
  int     m_hitCount;
  int     m_skipHits;
};

#endif
