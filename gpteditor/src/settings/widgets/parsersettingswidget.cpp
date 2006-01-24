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

#include "parsersettingswidget.h"

#include "gpteditorsettings.h"

#include <qlayout.h>
#include <qcheckbox.h>
#include <klocale.h>

#include "config.h"

ParserSettingsWidget::ParserSettingsWidget(QWidget *parent, const char *name)
 : QWidget(parent, name)
{
  QVBoxLayout* vbox = new QVBoxLayout(this, 0, 10);

  m_ckUseParser = new QCheckBox(this);
  m_ckUseParser->setText(i18n("Enable background parsing"));

  vbox->addWidget(m_ckUseParser);
  vbox->addItem(new QSpacerItem(0,0));

  #ifndef BG_PARSER_ENABLED
    m_ckUseParser->setChecked(false);
    m_ckUseParser->setEnabled(false);
  #endif
//   connect(m_ckUseParser, SIGNAL(stateChanged(int)), this, SLOT(slotUseParser(int)));
}


ParserSettingsWidget::~ParserSettingsWidget()
{
}

void ParserSettingsWidget::populate()
{
  m_ckUseParser->setChecked(GPTEditorSettings::self()->parserActive());
}

void ParserSettingsWidget::updateSettings()
{
  GPTEditorSettings::self()->setParserActive(m_ckUseParser->isChecked());
}

#include "parsersettingswidget.moc"
