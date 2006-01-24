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

#include "breakpointlistviewitem.h"
#include "breakpointlistview.h"
#include "debuggerbreakpoint.h"
#include <kiconloader.h>
#include <klocale.h>

BreakpointListViewItem::BreakpointListViewItem(KListView *parent)
    : KListViewItem(parent), m_breakpoint(0), m_offlineState(DebuggerBreakpoint::ENABLED)
{
  init();
}

BreakpointListViewItem::BreakpointListViewItem(KListView *parent, DebuggerBreakpoint* bp)
    : KListViewItem(parent), m_breakpoint(0), m_offlineState(DebuggerBreakpoint::ENABLED)
{
  init();
  setBreakpoint(bp);
}

void BreakpointListViewItem::init() {

//   setRenameEnabled(BreakpointListView::ConditionCol, true);
//   setRenameEnabled(BreakpointListView::SkipHitsCol,  true);

  KIconLoader *loader = KGlobal::iconLoader();
  m_enabled    = loader->loadIcon("checkboxon", KIcon::Small);
  m_disabled   = loader->loadIcon("checkboxoff", KIcon::Small);
  m_unresolved = loader->loadIcon("info", KIcon::Small);
}

BreakpointListViewItem::~BreakpointListViewItem()
{
  delete m_breakpoint;
}

void BreakpointListViewItem::setBreakpoint(DebuggerBreakpoint* bp)
{
  //TODO, make the pixmap members of the class (we are loading everytime...)

  delete m_breakpoint;
  m_breakpoint = bp;
  showBreakpoint();
}

void BreakpointListViewItem::showBreakpoint() {
  switch(m_breakpoint->status()) {
      case DebuggerBreakpoint::ENABLED:
      setText(BreakpointListView::StatusTextCol, i18n("Enabled"));
      setPixmap(BreakpointListView::StatusIconCol, m_enabled);
      break;

      case DebuggerBreakpoint::DISABLED:
      setText(BreakpointListView::StatusTextCol, i18n("Disabled"));
      setPixmap(BreakpointListView::StatusIconCol, m_disabled);
      break;

      case DebuggerBreakpoint::UNRESOLVED:
      default:
      setText(BreakpointListView::StatusTextCol, i18n("Unresolved"));
      setPixmap(BreakpointListView::StatusIconCol, m_unresolved);
      break;
  }

  setText(BreakpointListView::FileNameCol,  m_breakpoint->url().fileName());
  setText(BreakpointListView::FilePathCol,  m_breakpoint->url().prettyURL());
  setText(BreakpointListView::LineCol,      QString::number(m_breakpoint->line()));
//   setText(BreakpointListView::ConditionCol, m_breakpoint->condition());
//   setText(BreakpointListView::SkipHitsCol,  QString::number(m_breakpoint->skipHits()));
//   setText(BreakpointListView::HitCountCol,  QString::number(m_breakpoint->hitCount()));
}

void BreakpointListViewItem::setUserBpStatus(int status) {
  m_breakpoint->setStatus(status);
  m_offlineState = status;
}

void BreakpointListViewItem::reset() {

  m_breakpoint->setId(0);
  //if(m_breakpoint->status() == DebuggerBreakpoint::UNRESOLVED) {
  //  m_breakpoint->setStatus(DebuggerBreakpoint::ENABLED);
  //}
  m_breakpoint->setStatus(m_offlineState);
  showBreakpoint();
}

DebuggerBreakpoint* BreakpointListViewItem::breakpoint()
{
  return m_breakpoint;
}
