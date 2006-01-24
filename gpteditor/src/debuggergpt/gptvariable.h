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

#ifndef GPTVARIABLE_H
#define GPTVARIABLE_H

#include "variable.h"


class GPTVariable : public Variable {
public:
  GPTVariable(Variable* parent);
  GPTVariable(QString name);
  virtual ~GPTVariable();

  virtual QString compositeName();  /* ie. house[type], where:
                                         type is this->name()
                                         house is this->parent->name()
                                     */
};

class GPTScalarValue : public VariableScalarValue {
public:
  enum { Inteiro, Real, Logico, Literal, Caractere, Undefined };

  GPTScalarValue(Variable* owner);
  virtual ~GPTScalarValue();

  virtual void setType(int);
  virtual int type();

  virtual QString typeName();
private:
  int m_type;
};

//---------------------------------------------------------------


class GPTArrayValue : public VariableListValue {
public:
  GPTArrayValue(Variable* owner);
  virtual ~GPTArrayValue();

  virtual QString toString(int indent = 0);
  virtual QString typeName();
};

#endif
