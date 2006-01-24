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


#include "connection.h"

#include <qsocket.h>
#include <qsocketdevice.h>
#include <qsocketnotifier.h>

#include <kdebug.h>
#include <klocale.h>

Connection::Connection(QObject * parent, const char * name)
    : QObject(parent, name),
    m_currentClient(0), m_device(0), m_notifier(0), m_listening(false)

{}

Connection::~Connection()
{
  close();
}

void Connection::clearDevice()
{
  if(m_listening)
  {
    kdDebug() << "Stop listening on port " << m_device->port() << endl;
    m_device->close();
    m_listening = false;
  }

  delete m_device;
  m_device = NULL;
  delete m_notifier;
  m_notifier = NULL;
}

void Connection::clearSocket()
{
  if(m_currentClient)
  {
    m_currentClient->close();
    delete m_currentClient;
    m_currentClient = NULL;
    emit sigClientClosed();
  }
}

void Connection::slotIncomingConnection(int)
{
  int fd = m_device->accept();
  if(fd < 0) return;

  clearSocket();

  m_currentClient = new QSocket();
  m_currentClient->setSocket(fd);

  connect(m_currentClient, SIGNAL(connectionClosed()), this, SLOT(slotClosed()));
  connect(m_currentClient, SIGNAL(error(int)), this, SLOT(slotError(int)));

  emit sigAccepted(m_currentClient);
}

void Connection::slotClosed()
{
  clearSocket();
  //   emit sigClientClosed();
}

void Connection::slotError(int error)
{
  switch(error)
  {
    case QSocket::ErrConnectionRefused:
      emit sigError(QString("Connection refused"));
      break;
    case QSocket::ErrHostNotFound:
      emit sigError(QString("Host not found"));
      break;
    case QSocket::ErrSocketRead:
      emit sigError(QString("Error reading network data"));
      break;
  }
}

bool Connection::isListening()
{
  return m_listening;
}

void Connection::closeClient()
{
  clearSocket();
  //   emit sigClientClosed();
}

void Connection::close()
{
  closeClient();
  clearDevice();
}

bool Connection::listenOn(int port)
{
  close();

  m_device = new QSocketDevice(QSocketDevice::Stream, QHostAddress().isIPv4Address()
                               ? QSocketDevice::IPv4 : QSocketDevice::IPv6, 0);
  m_device->setAddressReusable( TRUE );

  if(m_device->bind(QHostAddress(), port)
      && m_device->listen(1))
  {
    m_listening = true;
    m_notifier = new QSocketNotifier(m_device->socket(), QSocketNotifier::Read,
                                     this, "accepting new connections" );

    connect( m_notifier, SIGNAL(activated(int)),
             this, SLOT(slotIncomingConnection(int)) );
  }
  else
  {
    m_listening = false;

    switch(m_device->error())
    {
      case QSocketDevice::NoError:
        kdDebug() << "ListenOn Error: NoError" << endl;
        break;
      case QSocketDevice::AlreadyBound:
        kdDebug() << "ListenOn Error: AlreadyBound" << endl;
        break;
      case QSocketDevice::Inaccessible:
        kdDebug() << "ListenOn Error: Inaccessible" << endl;
        break;
      case QSocketDevice::NoResources:
        kdDebug() << "ListenOn Error: NoResources" << endl;
        break;
      case QSocketDevice::InternalError:
        kdDebug() << "ListenOn Error: InternalError" << endl;
        break;
      case QSocketDevice::Impossible:
        kdDebug() << "ListenOn Error: Impossible" << endl;
        break;
      case QSocketDevice::NoFiles:
        kdDebug() << "ListenOn Error: NoFiles" << endl;
        break;
      case QSocketDevice::ConnectionRefused:
        kdDebug() << "ListenOn Error: ConnectionRefused" << endl;
        break;
      case QSocketDevice::NetworkFailure:
        kdDebug() << "ListenOn Error: NetworkFailure" << endl;
        break;
      case QSocketDevice::UnknownError:
        kdDebug() << "ListenOn Error: UnknownError" << endl;
        break;
    }

    delete m_device;
    m_device = 0;
  }

  return isListening();
}

#include "connection.moc"
