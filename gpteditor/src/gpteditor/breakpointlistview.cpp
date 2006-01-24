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

#include "breakpointlistview.h"
#include "breakpointlistviewitem.h"
#include "debuggerbreakpoint.h"
#include <klocale.h>

BreakpointListView::BreakpointListView(QWidget *parent, const char *name)
    : KListView(parent, name)
{

  setSorting(-1);
  setAllColumnsShowFocus(true);

  addColumn("");
  addColumn(i18n("Status"));
  addColumn(i18n("File name"));
  addColumn(i18n("File path"));  
  addColumn(i18n("Line"));
//   addColumn(tr2i18n("Condition"));
//   addColumn(tr2i18n("Skip hits"));
//   addColumn(tr2i18n("Hit count"));

  setColumnWidthMode(StatusIconCol, Manual);
  setColumnWidthMode(StatusTextCol, Manual);
  setColumnWidthMode(FileNameCol,  Manual);
  setColumnWidthMode(FilePathCol,  Manual);
  setColumnWidthMode(LineCol,      Manual);
//   setColumnWidthMode(ConditionCol, Manual);
//   setColumnWidthMode(SkipHitsCol,  Manual);
//   setColumnWidthMode(HitCountCol,  Manual);


  setColumnWidth(StatusIconCol, 25);
  setColumnWidth(StatusTextCol, 100);
  setColumnWidth(FileNameCol,  170);
  setColumnWidth(FilePathCol,  220);
  setColumnWidth(LineCol,      40);
//   setColumnWidth(ConditionCol, 90);
//   setColumnWidth(SkipHitsCol,  80);
//   setColumnWidth(HitCountCol,  80);

  connect(this, SIGNAL(clicked(QListViewItem*, const QPoint&, int)),
          this, SLOT(slotCicked(QListViewItem*, const QPoint&, int)));
  connect(this, SIGNAL(doubleClicked(QListViewItem *, const QPoint &, int )),
          this, SLOT(slotDoubleClick( QListViewItem *, const QPoint &, int )));

//   connect(this, SIGNAL(itemRenamed(QListViewItem*, int, const QString&)),
//           this, SLOT(slotItemRenamed(QListViewItem*, int, const QString&)));
}

BreakpointListView::~BreakpointListView()
{}

void BreakpointListView::slotCicked(QListViewItem* qitem, const QPoint&, int col)
{
  BreakpointListViewItem* item = dynamic_cast<BreakpointListViewItem*>(qitem);

  if(col == StatusIconCol)
  {
    if(item->breakpoint()->status() == DebuggerBreakpoint::DISABLED)
    {
      item->setUserBpStatus(DebuggerBreakpoint::ENABLED);
      item->showBreakpoint();
      emit sigBreakpointChanged(item->breakpoint());
    }
    else if(item->breakpoint()->status() == DebuggerBreakpoint::ENABLED)
    {
      item->setUserBpStatus(DebuggerBreakpoint::DISABLED);
      item->showBreakpoint();
      emit sigBreakpointChanged(item->breakpoint());
    }
  }

}

void BreakpointListView::slotDoubleClick(QListViewItem* qitem, const QPoint &, int col)
{
  BreakpointListViewItem* item = dynamic_cast<BreakpointListViewItem*>(qitem);

  switch(col)
  {
    case ConditionCol:
    case SkipHitsCol:
      item->startRename(col);
      break;

    case StatusIconCol:
    case StatusTextCol:
    case FileNameCol:
    case FilePathCol:
    case LineCol:
    case HitCountCol:
      emit sigDoubleClick(item->breakpoint()->url(), item->breakpoint()->line());
      break;
  }
}

// void BreakpointListView::slotItemRenamed(QListViewItem* qitem, int col, const QString& text)
// {
//   BreakpointListViewItem* item = dynamic_cast<BreakpointListViewItem*>(qitem);
//   switch(col)
//   {
//     case StatusIconCol:
//     case StatusTextCol:
//     case FileNameCol:
//     case FilePathCol:
//     case LineCol:
//     case HitCountCol:
//       break;
// 
//     case ConditionCol:
//       item->breakpoint()->setCondition(text);
//       emit sigBreakpointChanged(item->breakpoint());
//       break;
//     case SkipHitsCol:
//       item->breakpoint()->setSkipHits(text.toInt());
//       emit sigBreakpointChanged(item->breakpoint());
//       break;
//   }
// }

void BreakpointListView::resetBreakpointItems()
{
  BreakpointListViewItem* item =
    dynamic_cast<BreakpointListViewItem*>(firstChild());

  while(item)
  {
    item->reset();
    item =
      dynamic_cast<BreakpointListViewItem*>(item-> nextSibling());
  }
}

void BreakpointListView::updateBreakpoint(DebuggerBreakpoint* bp)
{
  BreakpointListViewItem*  item = findBreakpoint(bp);
  if(item)
  {
    item->setBreakpoint(bp);
  }
}

void BreakpointListView::toggleBreakpoint(const KURL& url, int line, bool enabled)
{
  BreakpointListViewItem*  item = findBreakpoint(url, line);
  if(item)
  {
    removeBreakpoint(item->breakpoint());
  }
  else
  {
    addBreakpoint(url, line, enabled);
  }
}

void BreakpointListView::addBreakpoint(const KURL& url, int line, bool enabled)
{
  //note: TextEditor lines are 0 based
  DebuggerBreakpoint* bp = new DebuggerBreakpoint(0, url, line);
  //addBreakpoint(bp);
  if(!enabled) {
    bp->setStatus(DebuggerBreakpoint::DISABLED);
  }  
  BreakpointListViewItem* item = new BreakpointListViewItem(this, bp);

  //insert new item to the bottom of the list
  item->moveItem(lastItem());

  emit sigBreakpointCreated(bp);
}

void BreakpointListView::removeBreakpoint(DebuggerBreakpoint* bp)
{
  BreakpointListViewItem*  item = findBreakpoint(bp);

  if(item)
  {
    takeItem(item);
  }
  emit sigBreakpointRemoved(bp);
  delete item;
}

QValueList<DebuggerBreakpoint*> BreakpointListView::breakpoints()
{

  QValueList<DebuggerBreakpoint*> list;

  BreakpointListViewItem* item =
    dynamic_cast<BreakpointListViewItem*>(firstChild());

  while(item)
  {
    list.append(item->breakpoint());
    item =
      dynamic_cast<BreakpointListViewItem*>(item-> nextSibling());
  }

  return list;
}

QValueList<DebuggerBreakpoint*> BreakpointListView::breakpointsFrom(const KURL& url)
{
  QValueList<DebuggerBreakpoint*> list;

  BreakpointListViewItem* item =
    dynamic_cast<BreakpointListViewItem*>(firstChild());

  while(item)
  {
    if(item->breakpoint()->url() == url)
    {
      list.append(item->breakpoint());
    }
    item =
      dynamic_cast<BreakpointListViewItem*>(item-> nextSibling());
  }

  return list;
}

void BreakpointListView::slotBreakpointMarked(const KURL& url, int line, bool enabled)
{
  toggleBreakpoint(url, line, enabled);
}

void BreakpointListView::slotBreakpointUnmarked(const KURL& url, int line)
{
  toggleBreakpoint(url, line);
}

/*
void BreakpointListView::addBreakpoint(DebuggerBreakpoint* bp) {
  BreakpointListViewItem* item = new BreakpointListViewItem(this, bp);
  emit sigBreakpointCreated(bp);
}
*/

void BreakpointListView::keyPressEvent(QKeyEvent* e)
{
  if(e->key() == Qt::Key_Delete)
  {

    BreakpointListViewItem* item =
      dynamic_cast<BreakpointListViewItem*>(currentItem());

    if(item)
    {
      takeItem(item);
      emit sigBreakpointRemoved(item->breakpoint());
      delete item;
    }
  }
  else
  {
    KListView::keyPressEvent(e);
  }
}

BreakpointListViewItem*  BreakpointListView::findBreakpoint(DebuggerBreakpoint* bp)
{
  BreakpointListViewItem* item =
    dynamic_cast<BreakpointListViewItem*>(firstChild());

  while(item)
  {
    if(item->breakpoint()->compare(bp))
    {
      return item;
    }
    item =
      dynamic_cast<BreakpointListViewItem*>(item-> nextSibling());
  }

  return NULL;
}

BreakpointListViewItem*  BreakpointListView::findBreakpoint(const KURL& url, int line)
{
  //Note: we don't use the DebuggerBreakpoint::id() to verify equality!

  BreakpointListViewItem* item =
    dynamic_cast<BreakpointListViewItem*>(firstChild());

  while(item)
  {
    if(item->breakpoint()->compare(url, line))
    {
      return item;
    }
    item =
      dynamic_cast<BreakpointListViewItem*>(item-> nextSibling());
  }

  return NULL;
}

#include "breakpointlistview.moc"
