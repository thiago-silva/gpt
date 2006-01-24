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

//#include "protoeditor.h"
#include "mainwindow.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#include <config.h>

static const char description[] =
  I18N_NOOP("G-Portugol editor");

static const char url[] = "http://TODO.com";
  
static const char version[] = VERSION;

static KCmdLineOptions options[] = {};
//   {
//     { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
//     KCmdLineLastOption
//   };


int main(int argc, char **argv)
{  
  KAboutData about("gpteditor", I18N_NOOP("GPTEditor"), version, description,
                   KAboutData::License_GPL, "(C) 2005 Thiago Silva", 0, url, "thiago.silva@kdemail.com");
  about.addAuthor( "Thiago Silva", 0, "thiago.silva@kdemail.com" );
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions( options );


  KApplication app;
  //ProtoEditor *protoeditor = 0;
  //ProtoEditor protoeditor;
  MainWindow window;

  

  if (app.isRestored())
  {
    RESTORE(MainWindow);
  }
  else
  {
    // no session.. just start up normally

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    //protoeditor = new ProtoEditor();
    //QWidget* mainWin = protoeditor.mainWindow();
    app.setMainWidget(&window);
    window.show();

    args->clear();
  }

  // mainWin has WDestructiveClose flag by default, so it will delete itself.
  return app.exec();

  //delete protoeditor;
}

