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

#include "variableslistview.h"
#include "variableslistviewitem.h"

#include <klocale.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <kapplication.h>

#include <qheader.h>
#include <qclipboard.h>


VariablesListView::VariablesListView(QWidget *parent, const char *name)
    : KListView(parent, name), m_variables(0), m_isReadOnly(false)
{
  setAllColumnsShowFocus(true);
  setRootIsDecorated(true);
  setSorting(-1);

  addColumn(i18n("Name"));
  addColumn(i18n("Value"));
  addColumn(i18n("Type"));

  setColumnWidthMode(NameCol,  Manual);
  setColumnWidthMode(ValueCol, Manual);
  setColumnWidthMode(TypeCol,  Manual);

  setColumnWidth(NameCol,  150);
  setColumnWidth(ValueCol, 150);
  setColumnWidth(TypeCol,  150);

  connect(this, SIGNAL(expanded(QListViewItem*)),
          this, SLOT(slotItemExpanded(QListViewItem*)));
  connect(this, SIGNAL(collapsed(QListViewItem*)),
          this, SLOT(slotItemCollapsed(QListViewItem*)));

  connect(this, SIGNAL(itemRenamed(QListViewItem*, int, const QString&)),
          this, SLOT(slotItemRenamed(QListViewItem*, int, const QString&)));

  connect(this, SIGNAL(doubleClicked(QListViewItem *, const QPoint &, int )),
          this, SLOT(slotDoubleClick( QListViewItem *, const QPoint &, int )));

  //connect(m_menu, SIGNAL(activated(int)), this, SLOT(slotCopyVarToClipboard(int)));

  connect(this, SIGNAL(contextMenuRequested(QListViewItem *, const QPoint& , int)),
          this, SLOT(slotContextMenuRequested(QListViewItem *, const QPoint &, int)));
}

// void VariablesListView::setReadOnly(bool readOnly)
// {
//   m_isReadOnly = readOnly;
// 
//   QListViewItem* item = firstChild();
//   while(item)
//   {
//     item->setRenameEnabled(ValueCol, !m_isReadOnly);
//     item = item->nextSibling();
//   }
// }
// 
// bool VariablesListView::isReadOnly()
// {
//   return m_isReadOnly;
// }


/*
 * The ListView doesn't start renaming if the item is not previously selected
 * And that sux...so, here we go.
 */
void VariablesListView::slotDoubleClick( QListViewItem *item, const QPoint &, int col)
{
  if(col == ValueCol)
  {
    item->startRename(ValueCol);
  }
}

VariablesListView::~VariablesListView()
{
  //clear();
}

void VariablesListView::slotItemRenamed(QListViewItem * item, int, const QString & text)
{

  VariablesListViewItem* vitem =
    dynamic_cast<VariablesListViewItem*>(item);

  //VariableScalarValue* value = new VariableScalarValue();

  dynamic_cast<VariableScalarValue*>(
    vitem->variable()->value())->set(text);

  emit sigVarModified(vitem->variable());
}

void VariablesListView::slotItemCollapsed(QListViewItem* item)
{
  markColapsed(dynamic_cast<VariablesListViewItem*>(item));
}

void VariablesListView::slotItemExpanded(QListViewItem* item)
{
  VariablesListViewItem* converted = dynamic_cast<VariablesListViewItem*>(item);
  populateChildren(converted);
  markExpanded(converted);
}

void VariablesListView::slotContextMenuRequested(QListViewItem* item, const QPoint& p, int)
{
  //   if(col != NameCol) return;
  enum { CopyVarItem, CopyValueItem };
  
  KPopupMenu* menu = new KPopupMenu(this);
  menu->insertItem(i18n("Copy variable"), CopyVarItem);
  menu->insertItem(i18n("Copy value"), CopyValueItem);
 
  int selection = menu->exec(p);
  if(selection == -1)
  {
    delete menu;
    return;
  }
  
  QClipboard* clip = kapp->clipboard();
  VariablesListViewItem* converted =
      dynamic_cast<VariablesListViewItem*>(item);
  
  switch(selection)
  {
    case CopyVarItem:
      clip->setText(converted->variable()->toString(), QClipboard::Clipboard);
      break;
    case CopyValueItem:
      clip->setText(converted->variable()->value()->toString(), QClipboard::Clipboard);
      break;
  }

  delete menu;
      
}

void VariablesListView::markColapsed(VariablesListViewItem* item)
{
  m_expanded.remove(item->stringPath());
}

void VariablesListView::markExpanded(VariablesListViewItem* item)
{
  m_expanded.push_back(item->stringPath());
}

void VariablesListView::populateChildren(VariablesListViewItem* item)
{
  /*
    Populate array/object children at request time
    so it can support recursive references easily
  */

  if(item->childCount() > 0)
  {
    return;
  }

  VariablesList_t* list =
    dynamic_cast<VariableListValue*>((item)->variable()->value())->list();

  addVariables(list, item);
}

void VariablesListView::setVariables(VariablesList_t* vars)
{
  /* saves the position of the viewport
   * ('cause when the view is repopulated, the scroll goes up)
   */
  int contentX = contentsX();
  int contentY = contentsY();

  /* backup the current item selected
   */
  QString currentSelected;
  VariablesListViewItem* item;

  item = dynamic_cast<VariablesListViewItem*>(selectedItem());
  if(item)
  {
    currentSelected = item->stringPath();
  }

  clear();
  deleteVars();

  /* finally, add the new ones
   */
  addVariables(vars);

  /* expand the new items according to the previous state
   */
  reexpandItems();

  /* resets the viewport position
   */
  setContentsPos(contentX, contentY);

  m_variables = vars;
}

void VariablesListView::addVariables(VariablesList_t* vars, VariablesListViewItem* parent)
{
  for(VariablesList_t::iterator it = vars->begin(); it != vars->end(); ++it)
  {
    addVariable(*it, parent);
  }
}

void VariablesListView::reexpandItems()
{
  //if we don't do this, m_expanded will have duplicates
  QValueList<QString> paths = m_expanded;
  m_expanded.clear();

  VariablesListViewItem* item;
  QValueList<QString>::iterator it;
  for(it = paths.begin(); it != paths.end(); it++)
  {
    item = getItemFromPath(*it);
    if(item) item->setOpen(true);
  }
}

VariablesListViewItem* VariablesListView::getItemFromPath(QString path)
{
  if(path.isEmpty()) return NULL;

  int sections = path.contains('/') + 1;
  int index = 0;
  QString s, comp;
  VariablesListViewItem* item = dynamic_cast<VariablesListViewItem*>(firstChild());

  do
  {
    if((!item) || (index >= sections)) break;
    s = item->text(0);
    comp = path.section('/', index, index);
    if(item->text(VariablesListView::NameCol) == path.section('/', index, index))
    {
      index++;
      if(index == sections)
      {
        return dynamic_cast<VariablesListViewItem*>(item);
      }
      else
      {
        populateChildren(item);
        item = dynamic_cast<VariablesListViewItem*>(item->firstChild());
      }
    }
    else
    {
      item = dynamic_cast<VariablesListViewItem*>(item->nextSibling());
      continue;
    }
  }
  while(true);

  return NULL;
}

void VariablesListView::addVariable(Variable* variable, VariablesListViewItem* parent)
{
  VariablesListViewItem* item;

  if(!parent)
  {
    item = new VariablesListViewItem(this, variable);
    //add new item to the bottom of the list
    item->moveItem(lastItem());
  }
  else
  {
    item = new VariablesListViewItem(parent, variable);
    item->moveItem(item->lastItem());
  }
}


void VariablesListView::deleteVars()
{
  if(!m_variables) return;

  for(VariablesList_t::iterator it = m_variables->begin(); it != m_variables->end(); ++it)
  {
    delete *it;
  }

  delete m_variables;
}

#include "variableslistview.moc"
