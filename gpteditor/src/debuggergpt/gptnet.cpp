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

#include "gptnet.h"

#include "connection.h"
#include "gpteditorsettings.h"
#include "debuggergpt.h"
#include "debuggerstack.h"
#include "gptvariable.h"
#include "gptvariableparser.h"
#include "debuggerbreakpoint.h"

#include <qdom.h>
#include <qsocket.h>
#include <kdebug.h>

GPTNet::GPTNet(DebuggerGPT* debugger, QObject* parent, const char* name)
  : QObject(parent, name), m_debugger(debugger), m_con(0), m_socket(0)
{
  m_con = new Connection();
  connect(m_con, SIGNAL(sigAccepted(QSocket*)), this, SLOT(slotIncomingConnection(QSocket*)));
  connect(m_con, SIGNAL(sigClientClosed()), this, SLOT(slotGPTClosed()));
  connect(m_con, SIGNAL(sigError(const QString&)), this, SLOT(slotError(const QString&)));
}
  
GPTNet::~GPTNet()
{
  delete m_con;
}

bool GPTNet::startListener()
{
  return m_con->listenOn(GPTEditorSettings::self()->listenPort());
}

void GPTNet::requestContinue()
{
  QString cmd = "continue";
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
}

void GPTNet::requestStop()
{
  QString cmd = "stop";
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
  m_con->closeClient();
}

void GPTNet::requestStepInto()
{
  QString cmd = "step_into";
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
}

void GPTNet::requestStepOver()
{
  QString cmd = "step_over";
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
}

void GPTNet::requestStepOut()
{
  QString cmd = "step_out";
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
}

void GPTNet::requestBreakpoint(int line)
{
  QString cmd = "breakpoint cmd=add line=" + QString::number(line);
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
}

void GPTNet::requestBreakpointRemoval(int line)
{
  QString cmd = "breakpoint cmd=remove line=" + QString::number(line);
  QString size = QString::number(cmd.length()+1);
  m_socket->writeBlock(size, size.length()+1);
  m_socket->writeBlock(cmd, cmd.length()+1);
}

void GPTNet::slotIncomingConnection(QSocket* socket)
{
  connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadBuffer()));
  m_socket = socket;

  emit sigGPTStarted();
}

void GPTNet::slotReadBuffer()
{
  long xmlSize;
  QString buffer;
  char ch;
  long read;
  QString str;

  do
  {
    buffer = "";

    while((ch = m_socket->getch()) != 0)
    {
      buffer += ch;
    }

    xmlSize = buffer.toLong()+1;//\0
    char data[xmlSize];

    read = m_socket->readBlock(data, xmlSize);

    str.setAscii(data,xmlSize);

//     kdDebug() << "incoming: >>>\n" << str << "\n<<<" << endl;
    processXML(str);

  }
  while(m_socket->bytesAvailable());  
}


void GPTNet::processXML(const QString& xml)
{
  QDomDocument* d = new QDomDocument();
  d->setContent(xml);

  QDomElement root = d->documentElement();
  QString name = root.tagName();

  if(name == "stackinfo")
  {
    processStack(root);
  }
  else if(name == "vars")
  {
     processVariables(root);
  }
  else 
  {
    kdDebug() << "BUG: GPTNet::processXML(): unknown root name :" << name << endl;
  }
  delete d;
}

void GPTNet::processStack(QDomElement& node)
{
  int id;
  QString file;
  QString function;
  int line;

  QDomNodeList list = node.elementsByTagName("entry");
  QDomElement st;

  DebuggerStack* stack = new DebuggerStack();
  for(uint i = 0; i < list.count(); i++)
  {
    st = list.item(i).toElement();
    
    id = st.attribute("id").toInt();
    file = st.attribute("file");
    function = st.attribute("function");
    line = st.attribute("line").toInt();

    stack->insert(id, KURL::fromPathOrURL(file), line, function!="@global"?function:"(escopo global)");    
  }  
  m_debugger->updateStack(stack);
}

void GPTNet::processVariables(QDomElement& node)
{
  QString scope = node.attribute("for");

  QDomNodeList list = node.childNodes();
  GPTVariableParser p;

  VariablesList_t* array = p.parse(list);

  if(scope == "$local")
  {
    m_debugger->updateVariables(array, false);
  }
  else 
  {
    m_debugger->updateVariables(array, true);
  }
}

void GPTNet::slotGPTClosed()
{
   emit sigGPTClosed();
}

void GPTNet::slotError(const QString& msg)
{
  kdDebug() << "GPTNet::slotError: " << msg << endl;
}
