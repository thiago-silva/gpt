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


#include "debuggerbreakpoint.h"
#include <kdebug.h>

DebuggerBreakpoint::DebuggerBreakpoint()
  : m_id(0), m_line(0), m_status(0), m_hitCount(0), m_skipHits(0)
{
}

DebuggerBreakpoint::DebuggerBreakpoint(int id, const KURL& url
                     , int line
                     , int status
                     , const QString& conditionExpr
                     , int hitCount
                     , int skipHits)

  : m_id(id), m_url(url), m_line(line),
    m_status(status), m_conditionExpr(conditionExpr),
    m_hitCount(hitCount), m_skipHits(skipHits)
{
}

DebuggerBreakpoint::DebuggerBreakpoint(const DebuggerBreakpoint& bp)
 : m_id(bp.m_id), m_url(bp.m_url), m_line(bp.m_line),
   m_status(bp.m_status), m_conditionExpr(bp.m_conditionExpr),
   m_hitCount(bp.m_hitCount), m_skipHits(bp.m_skipHits)
{
}

DebuggerBreakpoint::~DebuggerBreakpoint()
{
}

void DebuggerBreakpoint::setId(int id) {
  m_id = id;
}

void DebuggerBreakpoint::setURL(const KURL& url)
{
  m_url = url;
}

void DebuggerBreakpoint::setLine(int line)
{
  m_line = line;
}

void DebuggerBreakpoint::setCondition(const QString& expression)
{
  m_conditionExpr = expression;
}

void DebuggerBreakpoint::setStatus(int status)
{
  m_status = status;
}

void DebuggerBreakpoint::setHitCount(int hitCount)
{
  m_hitCount = hitCount;
}

void DebuggerBreakpoint::setSkipHits(int skipHits)
{
  m_skipHits = skipHits;
}

int DebuggerBreakpoint::id() {
  return m_id;
}

const KURL& DebuggerBreakpoint::url()
{
  return m_url;
}

int DebuggerBreakpoint::line()
{
  return m_line;
}

const QString& DebuggerBreakpoint::condition()
{
  return m_conditionExpr;
}

int DebuggerBreakpoint::status()
{
  return m_status;
}

int DebuggerBreakpoint::hitCount()
{
  return m_hitCount;
}

int DebuggerBreakpoint::skipHits()
{
  return m_skipHits;
}

bool DebuggerBreakpoint::compare(DebuggerBreakpoint* bp) {
  return (m_url == bp->m_url) &&
         (m_line     == bp->m_line);
}

bool DebuggerBreakpoint::compare(const KURL& url, int line) {
  return (m_url      == url) &&
         (m_line     == line);
}
