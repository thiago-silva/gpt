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

#include "gptvariable.h"
#include <klocale.h>

GPTVariable::GPTVariable(Variable* parent)
  : Variable(parent)
{
}
GPTVariable::GPTVariable(QString name)
  : Variable(name)
{
}
GPTVariable::~GPTVariable() {
}

QString GPTVariable::compositeName()
{
  QString cname;
  if(parent()) {
    cname = parent()->compositeName();
    if(parent()->value()->typeName() == i18n("Vector")) {
      cname += "['" + name() + "']";
    } else {
      cname += "->" + name();
    }
  } else {
    cname = name();
  }

  return cname;
}


//------------------------------------------------------

GPTScalarValue::GPTScalarValue(Variable* owner)
  : VariableScalarValue(owner), m_type(Undefined)
{
}

GPTScalarValue::~GPTScalarValue()
{
}

QString GPTScalarValue::typeName()
{
  switch(m_type) {
    case Inteiro:
      return i18n("Integer");
    case Real:
      return i18n("Float");
    case Logico:
      return i18n("Boolean");
    case Literal:
      return i18n("String");
    case Caractere:
      return i18n("Character");
    default:
      return i18n("Undefined");
  }
}

void GPTScalarValue::setType(int type)
{
  m_type = type;
}

int GPTScalarValue::type()
{
  return m_type;
}


//----------------------------------------------------------------

GPTArrayValue::GPTArrayValue(Variable* owner)
  : VariableListValue(owner)
{
}

GPTArrayValue::~GPTArrayValue()
{
}

QString GPTArrayValue::toString(int indent)
{
  QString ind;
  ind.fill(' ', indent);

  QString s;

  Variable* v;
  for(v =  m_list->first(); v; v = m_list->next()) {
    s += "\n";
    s += ind;

    if(v->value()->isScalar()) {
      s += v->name() + " => " + v->value()->toString();
    } else {
      s += v->name() + " = (" + v->value()->typeName() + ")";
      s += v->value()->toString(indent+3);
    }
  }

  return s;
}

QString GPTArrayValue::typeName()
{
  QString tname;

  tname += "[" + QString::number(m_list->count()) + "]";
  if(!m_list->first()->value()->isScalar()) {
    tname += m_list->first()->value()->typeName();
  }
  

  if(!owner()->parent()) {
    VariablesList_t* v = m_list;
    while(!v->first()->value()->isScalar()) {
      v = dynamic_cast<VariableListValue*>(v->first()->value())->list();
    }        

    tname = i18n("Vector") + tname + 
            " (" + v->first()->value()->typeName() + ")";
  }
  return tname;
}

