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


#ifndef CONNECTION_H
#define CONNECTION_H

#include <qobject.h>

class QSocket;
class QSocketDevice;
class QSocketNotifier;

/* General class for socket connections. */

class Connection  : public QObject
{
  Q_OBJECT
public:
  Connection(QObject * parent = 0, const char * name = 0);
  ~Connection();

  bool isListening();
  bool listenOn(int); 

  //close client only
  void closeClient();

  //close client and server
  void close();
private slots:
  void slotIncomingConnection(int);
  void slotClosed();
  void slotError(int);

signals:
  void sigAccepted(QSocket*);
  void sigClientClosed();
  void sigError(const QString&);

private:
  void clearSocket();
  void clearDevice();

  QSocket         *m_currentClient;
  QSocketDevice   *m_device;
  QSocketNotifier *m_notifier;
  bool m_listening;
};

#endif
