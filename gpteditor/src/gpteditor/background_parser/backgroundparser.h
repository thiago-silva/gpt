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

#ifndef BACKGROUNDPARSER_H
#define BACKGROUNDPARSER_H

#include <qobject.h>
#include <qstring.h>
#include "document.h"

class KURL;
class QTimer;
namespace KTextEditor {
  class EditInterface;
}

class BackgroundParser : public QObject {
  Q_OBJECT
public:
  BackgroundParser(Document*, QObject *parent = 0L, const char *name = 0L);
  ~BackgroundParser();

public slots:
  void parse();

signals:
  void sigParseError(int, const QString&, const KURL&);
  void sigParseOK(const KURL&);

private slots:
  void slotTextChanged();
  
private:
  void doParse(const QString& text);
  
  Document* m_ownerDoc;
//   KTextEditor::EditInterface *m_ied;
  QTimer* m_timer;
};

#endif
