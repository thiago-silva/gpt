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

#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <klistview.h>
#include <kurl.h>


class MessageListViewItem : public KListViewItem {
public:
  enum { TypeCol = 0, MessageCol, LineCol, FileCol };

  MessageListViewItem(KListView* parent);
  MessageListViewItem(KListView* parent, int type, QString message, int line, const KURL& file);
  ~MessageListViewItem();

  KURL url();
  int line();

private:
  int m_line;
  KURL m_url;  
};

class MessageListView : public KListView
{
Q_OBJECT
public:
  MessageListView(QWidget *parent = 0, const char *name = 0);
  ~MessageListView();

  void set(int type, QString message, int line, const KURL& file);
  void add(int type, QString message, int line, const KURL& file);

signals:
  void sigDoubleClick(const KURL&, int);

private slots:
  void slotDoubleClick(QListViewItem *, const QPoint &, int);
};

#endif
