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

#include "watchlistview.h"
#include "variableslistviewitem.h"

WatchListView::WatchListView(QWidget *parent, const char *name)
 : VariablesListView(parent, name)
{

}


WatchListView::~WatchListView()
{
}

void WatchListView::addWatch(Variable* var) {
  QListViewItem* item =
    findItem(var->name(), VariablesListView::NameCol);

  if(item) {
    takeItem(item);
    delete item;
  }
  addVariable(var);
}

void WatchListView::keyPressEvent(QKeyEvent* e) {
  if(e->key() == Qt::Key_Delete) {

    VariablesListViewItem* item =
      dynamic_cast<VariablesListViewItem*>(currentItem());

      if(item) {
        takeItem(item);
        emit sigWatchRemoved(item->variable());
        delete item;
      }
  } else {
    VariablesListView::keyPressEvent(e);
  }
}


#include "watchlistview.moc"
