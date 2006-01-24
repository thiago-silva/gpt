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


#include "gpteditorsettings.h"

GPTEditorSettings* GPTEditorSettings::m_self = 0;

GPTEditorSettings::GPTEditorSettings(  )
  : QObject(NULL, NULL), KConfigSkeleton( QString::fromLatin1( "gpteditorrc" ) )
{

  setCurrentGroup( QString::fromLatin1( "GPT" ) );

  //exec mode
  QValueList<KConfigSkeleton::ItemEnum::Choice> valuesMode;
  {
    KConfigSkeleton::ItemEnum::Choice choice;
    choice.name = QString::fromLatin1("Compiled");
    valuesMode.append( choice );
  }
  {
    KConfigSkeleton::ItemEnum::Choice choice;
    choice.name = QString::fromLatin1("Interpreted");
    valuesMode.append( choice );
  }
  KConfigSkeleton::ItemEnum  *itemExecMode;
  itemExecMode = new KConfigSkeleton::ItemEnum( currentGroup(), QString::fromLatin1("ExecutionMode"), 
    mExecutionMode, valuesMode, EnumCompiled );

  addItem(itemExecMode, QString::fromLatin1("ExecutionMode"));

  //shell
  KConfigSkeleton::ItemString *itemShell;
  itemShell = new KConfigSkeleton::ItemString(currentGroup(), 
    QString::fromLatin1("Shell"), mShell, "/bin/sh" );
  addItem(itemShell, QString::fromLatin1("Shell"));


  //includeFile
  KConfigSkeleton::ItemPath  *itemIncludeFile;
  itemIncludeFile = new KConfigSkeleton::ItemPath(currentGroup(), 
    QString::fromLatin1("IncludeFile"), mIncludeFile);
  addItem(itemIncludeFile, QString::fromLatin1("IncludeFile"));

  //host
  KConfigSkeleton::ItemString *ItemHost;
  ItemHost = new KConfigSkeleton::ItemString(currentGroup(), 
    QString::fromLatin1("Host"), mHost, "localhost" );
  addItem(ItemHost, QString::fromLatin1("Host"));

  //port
  KConfigSkeleton::ItemInt  *itemListenPort;
  itemListenPort = new KConfigSkeleton::ItemInt(currentGroup(), QString::fromLatin1("ListenPort"), mListenPort, 7680);
  addItem(itemListenPort, QString::fromLatin1("ListenPort"));

  //parseActive
  KConfigSkeleton::ItemBool  *itemParserActive;
  itemParserActive = new KConfigSkeleton::ItemBool(currentGroup(), QString::fromLatin1("ParserActive"), mParserActive, true);
  addItem(itemParserActive, QString::fromLatin1("ParserActive"));

  //args
  KConfigSkeleton::ItemString *ItemArgs;
  ItemArgs = new KConfigSkeleton::ItemString(currentGroup(), 
    QString::fromLatin1("GPTArgs"), mGPTArgs, "-d" );
  addItem(ItemArgs, QString::fromLatin1("GPTArgs"));

  readConfig();
}

GPTEditorSettings::~GPTEditorSettings()
{
}

void GPTEditorSettings::setExecutionMode(int mode) 
{
  mExecutionMode = mode;
}

int GPTEditorSettings::executionMode() const
{
  return mExecutionMode;
}

void GPTEditorSettings::setShell(const QString& shell)  
{
  mShell = shell;
}

QString GPTEditorSettings::shell() 
{
  return mShell;
}

void GPTEditorSettings::setIncludeFile(const QString& filepath)  
{
  mIncludeFile = filepath;
}

QString GPTEditorSettings::includeFile() const
{
  return mIncludeFile;
}

void GPTEditorSettings::setHost(QString host) 
{
  mHost = host;
}

QString GPTEditorSettings::host() const 
{
  return mHost;
}

void GPTEditorSettings::setListenPort(int port) 
{
  mListenPort = port;
}

int GPTEditorSettings::listenPort() const 
{
  return mListenPort;
}

void GPTEditorSettings::setParserActive(bool val) 
{
  mParserActive = val;
}

bool GPTEditorSettings::parserActive() 
{
  return mParserActive;
}

void GPTEditorSettings::setGPTArgs(const QString& args)
{
  mGPTArgs = args;  
}

QString GPTEditorSettings::GPTArgs()
{
  return mGPTArgs;
}

GPTEditorSettings* GPTEditorSettings::self()
{
  if(!m_self) {
    m_self = new GPTEditorSettings();
  }

  return m_self;
}

void GPTEditorSettings::writeConfig() 
{
  KConfigSkeleton::writeConfig();

  emit sigSettingsChanged();
}

#include "gpteditorsettings.moc"
