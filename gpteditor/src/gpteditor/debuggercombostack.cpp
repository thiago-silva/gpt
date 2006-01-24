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

#include "debuggercombostack.h"
#include "debuggerstack.h"

DebuggerComboStack::DebuggerComboStack(QWidget* parent, const char* name)
  : QComboBox(false, parent, name), m_currentExecutionPoint(NULL), m_stack(NULL)
{
  connect(this, SIGNAL(activated(int)), this, SLOT(slotChanged(int)));
}

DebuggerComboStack::~DebuggerComboStack()
{
  delete m_stack;
}

void DebuggerComboStack::setStack(DebuggerStack* stack)
{
  clear();

  DebuggerStack::DebuggerExecutionPointList_t execPointList
    = stack->debuggerExecutionPointList();

  DebuggerExecutionPoint* execPoint;
  for(execPoint = execPointList.first(); execPoint; execPoint = execPointList.next()) {
    insertItem(execPoint->function() + " : " + QString::number(execPoint->line()));
  }

  setCurrentItem(0);
  m_currentExecutionPoint = execPointList.first();

  if(m_stack) delete m_stack;
  m_stack = stack;
}

DebuggerStack* DebuggerComboStack::stack() {
  return m_stack;
}

DebuggerExecutionPoint* DebuggerComboStack::selectedDebuggerExecutionPoint() {
  return m_currentExecutionPoint;
}

void DebuggerComboStack::slotChanged(int index)
{
  DebuggerExecutionPoint* execPoint
    = m_stack->debuggerExecutionPointList().at(index);

  emit changed(m_currentExecutionPoint, execPoint);

  m_currentExecutionPoint = execPoint;
}


#include "debuggercombostack.moc"
