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

#include "variable.h"

Variable::Variable(Variable* parent)
    : m_parent(parent), m_name(""), m_value(NULL), m_isShared(false), m_isReference(false) {}

Variable::Variable(QString name)
    : m_parent(0), m_name(name), m_value(NULL), m_isReference(false) {}

Variable::~Variable()
{
  if(!isValueShared()) {
    delete m_value;
  }
}

void Variable::setName(QString name)
{
  m_name = name;
}

QString Variable::name()
{
  return m_name;
}

void Variable::setValue(VariableValue* value)
{
  if(!isValueShared()) {
    delete m_value;
  }
  m_value = value;
}

VariableValue* Variable::value()
{
  return m_value;
}


void Variable::setValueShared(bool shared)
{
  m_isShared = shared;
}

bool Variable::isValueShared()
{
  return m_isShared;
}

QString Variable::toString()
{
  QString s;

  if(value()->isScalar()) {
    s = name() + " = " + value()->toString();
  } else {
    s =  name() + " = (" + value()->typeName() + ")";
    s += value()->toString(3);
  }
  return s;
}

void Variable::setReference(bool reference)
{
  m_isReference = reference;
}

bool Variable::isReference()
{
  return m_isReference;
}

Variable* Variable::parent()
{
  return m_parent;
}


//--------------------------------------------------------------


VariableValue::VariableValue(Variable* owner)
    : m_varOwner(owner), m_isScalar(true) {}

VariableValue::~VariableValue() {}

void VariableValue::setScalar(bool scalar)
{
  m_isScalar = scalar;
}

Variable* VariableValue::owner()
{
  return m_varOwner;
}

bool VariableValue::isScalar()
{
  return m_isScalar;
}

//---------------------------------------------------------------------------

VariableScalarValue::VariableScalarValue(Variable* owner)
    : VariableValue(owner) {}

VariableScalarValue::~VariableScalarValue() {}

void VariableScalarValue::set(QString value)
{
  m_value = value;
}

QString VariableScalarValue::toString(int)
{
  return m_value;
}

//---------------------------------------------------------------------------

VariableListValue::VariableListValue(Variable* owner)
    : VariableValue(owner), m_list(NULL) {}

VariableListValue::~VariableListValue()
{
  delete m_list; //autoDelete must be set to true;
}

void VariableListValue::setList(VariablesList_t* list)
{
  delete m_list;

  m_list = list;
  m_list->setAutoDelete(true);
}

VariablesList_t* VariableListValue::list()
{
  return m_list;
}
