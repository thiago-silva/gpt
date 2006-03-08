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

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <klineedit.h>
#include <kurlrequester.h>
#include <klocale.h>

#include "gpteditorsettings.h"
#include "compilersettingswidget.h"

CompilerSettingsWidget::CompilerSettingsWidget(QWidget *parent, const char *name)
 : QWidget(parent, name)
{

  QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 10);

  QGridLayout* grid = new QGridLayout(0, 3, 2, 0, 6);
//   grid->setSpacing(10);

  QLabel* label = new QLabel(this);
  label->setText(i18n("Execution mode:"));
  grid->addWidget(label, 0, 0);

  m_cpExecutionMode = new QComboBox(this);  
  m_cpExecutionMode->insertItem(i18n("Compilation"));
  m_cpExecutionMode->insertItem(i18n("Interpretation"));  
  grid->addWidget(m_cpExecutionMode, 0, 1);


  label = new QLabel(this);
  label->setText(i18n("Arguments:"));
  grid->addWidget(label, 1, 0);
  m_edArgs = new KLineEdit(this);
  m_edArgs->setEnabled(false);
  grid->addWidget(m_edArgs, 1, 1);


  label = new QLabel(this);
  label->setText(i18n("Shell:"));
  grid->addWidget(label, 2, 0);
  
  m_edShell = new KURLRequester(this);
  m_edShell->setFilter(i18n("|All"));
  grid->addWidget(m_edShell, 2, 1);

//   label = new QLabel(this);
//   label->setText("Incluir arquivo automaticamente:");
//   grid->addWidget(label, 3, 0);
//   
//   m_edIncludeFile = new KURLRequester(this);
//   m_edIncludeFile->setFilter("*.gpt|Algoritmo G-Portugol\n*|Todos");
//   grid->addWidget(m_edIncludeFile, 3, 1);

  QHBoxLayout* hbox = new QHBoxLayout(this);
  hbox->addLayout(grid);
  hbox->addItem(new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum ));
  
  mainLayout->addLayout(hbox);

  mainLayout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum,QSizePolicy::Expanding ));
}


CompilerSettingsWidget::~CompilerSettingsWidget()
{
}
void CompilerSettingsWidget::populate() 
{
  m_cpExecutionMode->setCurrentItem(GPTEditorSettings::self()->executionMode());
  m_edShell->setURL(GPTEditorSettings::self()->shell());
//   m_edIncludeFile->setURL(GPTEditorSettings::self()->includeFile());
  m_edArgs->setText(GPTEditorSettings::self()->GPTArgs());
}

void CompilerSettingsWidget::updateSettings() 
{
  GPTEditorSettings::self()->setExecutionMode(m_cpExecutionMode->currentItem());
  GPTEditorSettings::self()->setShell(m_edShell->url());
//   GPTEditorSettings::self()->setIncludeFile(m_edIncludeFile->url());
  //GPTEditorSettings::self()->setGPTArgs(m_edArgs->text());
  GPTEditorSettings::self()->setGPTArgs("");
}

#include "compilersettingswidget.moc"
