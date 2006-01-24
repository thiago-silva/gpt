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

#ifndef BREAKPOINTLISTVIEWITEM_H
#define BREAKPOINTLISTVIEWITEM_H

#include <klistview.h>
#include <qpixmap.h>

class DebuggerBreakpoint;

class BreakpointListViewItem : public KListViewItem
{
public:
  BreakpointListViewItem(KListView *parent);
  BreakpointListViewItem(KListView *parent, DebuggerBreakpoint*);
  ~BreakpointListViewItem();

  void setBreakpoint(DebuggerBreakpoint*);

  void reset();

  void showBreakpoint();
  DebuggerBreakpoint* breakpoint();

  void setUserBpStatus(int);
private:
  void init();

  QPixmap m_enabled;
  QPixmap m_disabled;
  QPixmap m_unresolved;

  DebuggerBreakpoint* m_breakpoint;
  int m_offlineState;
};

#endif
