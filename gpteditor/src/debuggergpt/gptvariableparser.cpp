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

#include "gptvariableparser.h"
#include "gptvariable.h"
#include <qdom.h>
#include <kdebug.h>
#include <klocale.h>

/*
<vars for="$local" scope="@global">
  <var name="x" type="inteiro" primitive="0">
    <dimensions>
      <dimension level="0" size="2"/>
    </dimensions>
  </var>
</vars>

<vars for="$global" scope="@global">
  <var name="x" type="inteiro" primitive="0">
    <dimensions>
      <dimension level="0" size="2"/>
    </dimensions>
    <index num="0" value="2"/>
  </var>
</vars>
<<<

*/
GPTVariableParser::GPTVariableParser()
{}

GPTVariableParser::~GPTVariableParser()
{}

VariablesList_t* GPTVariableParser::parse(QDomNodeList& list)
{
  return parseList(list, 0);
}

GPTVariable* GPTVariableParser::parse(QDomNode& node)
{
  return parseVar(node, 0);
}

VariablesList_t* GPTVariableParser::parseList(const QDomNodeList& list, GPTVariable* parent, const QString& vtype)
{
  QDomElement e;
  VariablesList_t* vlist = new VariablesList_t;
  GPTVariable* var;
  for(uint i = 0; i < list.count(); i++)
  {
    e = list.item(i).toElement();
    
    var = parseVar(e, parent, vtype);
    vlist->append(var);
  }
  return vlist;
}

GPTVariable* GPTVariableParser::parseVar(QDomNode& node, GPTVariable* parent, const QString& vtype)
{
  QDomElement e = node.toElement();
  
  GPTVariable* var = new GPTVariable(parent);

  bool isPrimitive = e.attributeNode("primitive").value()=="true"?true:false;

  var->setName(e.attributeNode("name").value());

  QString type;
  if(vtype.isEmpty()) {
    type = e.attributeNode("type").value();
  } else {
    type = vtype;
  }

  if(!isPrimitive)
  {
    GPTArrayValue* arrayValue = new GPTArrayValue(var);
    var->setValue(arrayValue);
    arrayValue->setScalar(false);

    arrayValue->setList(parseList(e.namedItem("values").childNodes(), var, type));
  }
  else
  {
    GPTScalarValue* value = new GPTScalarValue(var);
    var->setValue(value);

    if(type == "inteiro")
    {
      value->setType(GPTScalarValue::Inteiro);      
      value->set(e.attributeNode("value").value());
    }
    else if(type == "literal")
    {
      value->setType(GPTScalarValue::Literal);
      QString str = "\"" + e.attributeNode("value").value() + "\"";
      value->set(str);
    }
    else if(type == "real")
    {
      value->setType(GPTScalarValue::Real);
      value->set(e.attributeNode("value").value());
    } 
    else if(type == "lógico") 
    {
      value->setType(GPTScalarValue::Logico);
      value->set(e.attributeNode("value").value().toInt()?i18n("true"):i18n("false"));
    } 
    else if(type == "caractere")
    {
      QString val = e.attributeNode("value").value();
      QString carac = "'";
      if(val == "10")
      {
        carac += "\\n";
      }
      else if(val == "13")
      {
        carac += "\\r";
      }
      else if(val == "9")
      {
        carac += "\\t";
      }
      else
      {
        QChar c = val.toInt();
        if(c.isPrint())
        {
          carac += c;
        }
        else
        {
          carac += "{" + val + "}";
        }
      }
      carac += "'";

      value->setType(GPTScalarValue::Caractere);
      value->set(carac);
    }
  }
  return var;
}

