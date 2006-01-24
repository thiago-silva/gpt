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

#include "configdlg.h"

#include <qframe.h>
#include <qvbox.h>

#include <klocale.h>
#include <kiconloader.h>
#include <kwin.h>
#include <kapplication.h>
#include <qlayout.h>

#include "gpteditorsettings.h"

#include "compilersettingswidget.h"
#include "debuggersettingswidget.h"
#include "parsersettingswidget.h"

ConfigDlg* ConfigDlg::m_self = 0;

ConfigDlg::ConfigDlg(QWidget *parent, const char *name)
 : KDialogBase(KDialogBase::TreeList, WStyle_DialogBorder, parent, name, true, "Settings", KDialogBase::Ok|KDialogBase::Cancel)
{
  setShowIconsInTreeList(true);
  setMinimumSize(600,400);


  QStringList path;
  path << i18n("GPT");
//   setFolderIcon(path, BarIcon("gpt", KIcon::SizeSmall));

  QVBox *frame;

  frame = addVBoxPage(path, i18n("GPT"), BarIcon("gpteditor", KIcon::SizeSmall));  
  frame->layout()->setMargin(10);
  m_compilerSettingsWidget = new CompilerSettingsWidget(frame, "GPT");//->reparent(((QWidget*)frame), 0, QPoint());

  path.clear();
  path << i18n("Debugger");
  frame= addVBoxPage(path, i18n("Debugger"), BarIcon("bug", KIcon::SizeSmall));
  frame->layout()->setMargin(10);
  m_debuggerSettingsWidget = new DebuggerSettingsWidget(frame);//->reparent(((QWidget*)frame), 0, QPoint());

  path.clear();
  path << i18n("Parser");
  frame= addVBoxPage(path, i18n("Parser"), BarIcon("viewmag", KIcon::SizeSmall));
  frame->layout()->setMargin(10);
  m_parserSettingsWidget = new ParserSettingsWidget(frame);//->reparent(((QWidget*)frame), 0, QPoint());

  enableButtonSeparator(true);
  unfoldTreeList();
}


ConfigDlg::~ConfigDlg()
{
}

void ConfigDlg::showDialog()
{
  if(!m_self) {
    m_self = new ConfigDlg();
  }

  m_self->populateWidgets();
  m_self->exec();
}

void ConfigDlg::populateWidgets()
{
  m_compilerSettingsWidget->populate();
  m_parserSettingsWidget->populate();
  m_debuggerSettingsWidget->populate();
}

void ConfigDlg::slotOk()
{
  KDialogBase::slotOk();

  m_compilerSettingsWidget->updateSettings();
  m_parserSettingsWidget->updateSettings();
  m_debuggerSettingsWidget->updateSettings();

  GPTEditorSettings::self()->writeConfig();

  emit settingsChanged();
}


#include "configdlg.moc"
