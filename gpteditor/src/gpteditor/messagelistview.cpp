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

#include "messagelistview.h"
#include <klocale.h>
#include <kiconloader.h>
#include <kurl.h>
#include <kdebug.h>

#include "debuggermanager.h"

MessageListViewItem::MessageListViewItem(KListView* parent)
  : KListViewItem(parent)
{
}
MessageListViewItem::MessageListViewItem(KListView* parent, int type, QString message, int line, const KURL& url)
  : KListViewItem(parent), m_line(line), m_url(url)
{
  setSelectable(true);
  
  switch(type) {
    case DebuggerManager::InfoMsg:
      setPixmap(MessageListViewItem::TypeCol, SmallIcon("info"));
      break;
    case DebuggerManager::WarningMsg:
      setPixmap(MessageListViewItem::TypeCol, SmallIcon("messagebox_warning"));
      break;
    case DebuggerManager::ErrorMsg:
      setPixmap(MessageListViewItem::TypeCol, SmallIcon("cancel"));
      break;
  }

  setText(MessageListViewItem::MessageCol, message);
  setText(MessageListViewItem::LineCol, QString::number(line));
  setText(MessageListViewItem::FileCol, url.fileName());  
}

MessageListViewItem::~MessageListViewItem()
{
}

KURL MessageListViewItem::url()
{
  return m_url;
}
int MessageListViewItem::line()
{
  return m_line;
}
//------------------------------------

MessageListView::MessageListView(QWidget *parent, const char *name)
 : KListView(parent, name)
{
  setSorting(-1);
  setAllColumnsShowFocus(true);

  addColumn("*");
  addColumn(i18n("Message"));
  addColumn(i18n("Line"));
  addColumn(i18n("File"));

  setColumnWidth(MessageListViewItem::MessageCol, 350);
  setColumnWidth(MessageListViewItem::FileCol, 200);

  setColumnWidthMode (0, QListView::Manual);
  setColumnWidthMode (1, QListView::Manual);
  setColumnWidthMode (2, QListView::Manual);
  setColumnWidthMode (3, QListView::Manual);

  connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint &, int )),
    this, SLOT(slotDoubleClick(QListViewItem *, const QPoint &, int )));
}

MessageListView::~MessageListView()
{
}

void MessageListView::add(int type, QString message, int line, const KURL& url) {
  MessageListViewItem* item = new MessageListViewItem(this,
    type, message, line, url);

  //add new item to the bottom of the list  
  item->moveItem(lastItem());
}

void MessageListView::set(int type, QString message, int line, const KURL& file)
{
  clear();
  add(type, message, line, file);
}

void MessageListView::slotDoubleClick(QListViewItem * item, const QPoint &, int)
{
  MessageListViewItem *msgItem = 
    dynamic_cast<MessageListViewItem*>(item);

  emit sigDoubleClick(msgItem ->url(), msgItem ->line());
}


#include "messagelistview.moc"
