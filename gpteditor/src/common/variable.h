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

#ifndef VARIABLE_H
#define VARIABLE_H

#include <qstring.h>
#include <qptrlist.h>

class VariableValue;
class Variable;

typedef QPtrList<Variable> VariablesList_t;

class Variable {
public:
  virtual ~Variable();

  void setName(QString);
  QString name();                       /* ie. type */

  virtual QString compositeName() = 0;  /* ie. house[type], where:
                                         type is this->name()
                                         house is this->parent->name()
                                        */

  virtual void setValue(VariableValue*);
  virtual VariableValue* value();

  //we need to know if another Variable object is using
  //m_value (ie. if this is a refernce) so we know if m_value
  //should be deleted on ~Variable()
  void setValueShared(bool);
  bool isValueShared();

  QString toString();

  virtual void setReference(bool);
  virtual bool isReference();

  Variable* parent();
protected:
  Variable(Variable* parent);
  Variable(QString name);
private:
  Variable*      m_parent;
  QString        m_name;
  VariableValue* m_value;
  bool           m_isShared;
  bool           m_isReference;
};

class VariableValue {
public:
  virtual ~VariableValue();

  void setScalar(bool);
  bool isScalar();
  Variable* owner();

  virtual QString typeName() = 0;
  virtual QString toString(int indent = 0) = 0;
protected:
  VariableValue(Variable* owner);
private:
  Variable* m_varOwner;
  bool      m_isScalar;
};


class VariableScalarValue : public VariableValue {
public:
  virtual ~VariableScalarValue();

  virtual void set(QString);

  virtual QString toString(int indent = 0);

  virtual QString typeName() = 0;
protected:
  VariableScalarValue(Variable* owner);
  QString m_value;
};

class VariableListValue : public VariableValue {
public:
  virtual ~VariableListValue();

  void setList(VariablesList_t*);
  VariablesList_t* list();

  virtual QString toString(int indent = 0) = 0;
  virtual QString typeName() = 0;
protected:
  VariableListValue(Variable* owner);
  VariablesList_t* m_list;
};

#endif
