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

#ifndef VARIABLESLISTVIEW_H
#define VARIABLESLISTVIEW_H

#include <klistview.h>
#include <qvaluelist.h>
#include "variable.h"

class VariablesListViewItem;
// class KPopupMenu;

class VariablesListView : public KListView
{
  Q_OBJECT
public:
  enum { NameCol = 0, ValueCol, TypeCol };

  VariablesListView(QWidget *parent = 0, const char *name = 0);
  virtual ~VariablesListView();

  void setVariables(VariablesList_t* vars);

//   void setReadOnly(bool);
//   bool isReadOnly();
signals:
  void sigVarModified(Variable*);

protected slots:
  void slotItemExpanded(QListViewItem * item);
  void slotItemCollapsed(QListViewItem * item);
  void slotItemRenamed(QListViewItem * item, int col, const QString & text);

  void slotDoubleClick(QListViewItem *, const QPoint &, int );

  void slotContextMenuRequested(QListViewItem *, const QPoint &, int);

protected:
  void markExpanded(VariablesListViewItem* item);
  void markColapsed(VariablesListViewItem* item);

  void populateChildren(VariablesListViewItem* item);

  void addVariables(VariablesList_t* vars, VariablesListViewItem* parent = NULL);
  void addVariable(Variable* variable, VariablesListViewItem* parent = NULL);

  void reexpandItems();
  VariablesListViewItem* getItemFromPath(QString path);
  void deleteVars();

//   KPopupMenu* m_menu;
  QValueList<QString> m_expanded;
  VariablesList_t*    m_variables;
  bool m_isReadOnly;
};

#endif
