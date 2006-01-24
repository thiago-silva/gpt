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

#ifndef GPTEDITORSETTINGS_H
#define GPTEDITORSETTINGS_H

#include <kconfigskeleton.h>
#include <qobject.h>
#include <kurl.h>
#include <qstring.h>

/*
Opções:

compilador:
    modo de execucao: compilado
                      interpretado

    Incluir arquivo automaticamente:
      [/usr/share/comum.pt         ] [find]

    ---Depuração interativa----
      Host : localhost
      Porta: 7680


Parser:
    [x] ativar analise de código.
*/
class GPTEditorSettings :   public QObject
                          , public KConfigSkeleton {
  Q_OBJECT
public:

  enum 
  {
    EnumCompiled,
    EnumInterpreted
  };

  GPTEditorSettings();
  ~GPTEditorSettings();


  void        setExecutionMode(int mode);
  int         executionMode() const;
  void        setShell(const QString&);
  QString     shell();
  void        setIncludeFile(const QString& filepath);
  QString     includeFile() const ;
  void        setHost(QString host);
  QString     host() const;
  void        setListenPort(int port);
  int         listenPort() const;
  void        setParserActive(bool val);
  bool        parserActive();

  void        setGPTArgs(const QString&);
  QString     GPTArgs();

  static GPTEditorSettings* self();

  virtual void writeConfig();

signals:
  void sigSettingsChanged();


protected:  
  int     mExecutionMode;
  QString mShell;
  QString mIncludeFile;
  QString mHost;
  int     mListenPort;

  bool    mParserActive;

  QString mGPTArgs;

  static GPTEditorSettings* m_self;
};

#endif
