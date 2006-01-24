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

#include "debuggersettingswidget.h"
#include "gpteditorsettings.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <klineedit.h>
#include <klocale.h>

DebuggerSettingsWidget::DebuggerSettingsWidget(QWidget *parent, const char *name)
 : QWidget(parent, name)
{
  QVBoxLayout* mainLayout = new QVBoxLayout(this, 0, 10);

  QHBoxLayout* hbox = new QHBoxLayout( 0, 0, 6);

  QGridLayout* gridLayout = new QGridLayout(this, 1, 1, 0, 6); 
  
  m_edHost = new KLineEdit( this);
  gridLayout->addWidget( m_edHost, 0, 1 );

  QLabel* label = new QLabel( this);
  label->setText(i18n("Host:"));
  gridLayout->addWidget( label, 0, 0 );

  label = new QLabel( this);
  label->setText(i18n("Port:"));
  gridLayout->addWidget( label, 1, 0 );

  m_spListenPort = new QSpinBox( this);
  m_spListenPort->setMaxValue(99999);
  gridLayout->addWidget( m_spListenPort, 1, 1 );

  hbox->addLayout( gridLayout );
  hbox->addItem( new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));

  mainLayout->addLayout( hbox );

  mainLayout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ));
}


DebuggerSettingsWidget::~DebuggerSettingsWidget()
{
}

void DebuggerSettingsWidget::populate()
{
  m_edHost->setText(GPTEditorSettings::self()->host());
  m_spListenPort->setValue(GPTEditorSettings::self()->listenPort());
}

void DebuggerSettingsWidget::updateSettings()
{
  GPTEditorSettings::self()->setHost(m_edHost->text());
  GPTEditorSettings::self()->setListenPort(m_spListenPort->value());
}

#include "debuggersettingswidget.moc"


