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

#ifndef DEBUGGERMANAGER_H
#define DEBUGGERMANAGER_H
#include <qobject.h>
#include <qmap.h>
#include <qstring.h>
#include "variable.h"

class DebuggerStack;
class AbstractDebugger;
class DebuggerBreakpoint;
class MainWindow;
class DebuggerExecutionPoint;
class KURL;

/*
 The core of the application.
 Most of the data flow is represented in this class.
 The other part is the implementation-dependet handled by individual debugger clients.
*/

class DebuggerManager : public QObject
{
  Q_OBJECT
public:

  enum { InfoMsg, WarningMsg, ErrorMsg, }; //Debugger session message types (non-internal)

  DebuggerManager(MainWindow* window, QObject *parent = 0, const char* name = 0);
  ~DebuggerManager();

  void init();

  /* for debugger to notify us */
  void updateStack(DebuggerStack*);
  void updateGlobalVars(VariablesList_t*);
  void updateLocalVars(VariablesList_t*);
  void updateWatch(Variable*);
  void updateBreakpoint(DebuggerBreakpoint* bp);
  void debugMessage(int, const QString&, const KURL&, int);
  void error(const QString&);

public slots:
  void slotCompileApplication();  
  void slotRunApplication();
  void slotDebugStart();
  void slotDebugStop();
  void slotDebugStepInto();
  void slotDebugStepOver();
  void slotDebugStepOut();

  void slotGotoLineAtFile(const KURL&, int);
  
private slots:
  
  /* Application (Window) to DebuggerManager
     Those are called when the user manipulates the UI.
  */

  void slotDebugToggleBp();

  void slotAddWatch(); //from WatchList
  void slotAddWatch(const QString& expression); //from editor ctx menu
  void slotComboStackChanged(DebuggerExecutionPoint*, DebuggerExecutionPoint*);
  void slotGlobalVarModified(Variable* var);
  void slotLocalVarModified(Variable* var);
  void slotBreakpointCreated(DebuggerBreakpoint*);
  void slotBreakpointChanged(DebuggerBreakpoint*);
  void slotBreakpointRemoved(DebuggerBreakpoint*);
  void slotWatchRemoved(Variable*);
  void slotNoDocument();
  
  void slotNewDocument();
  
  /* AbstractDebugger to DebuggerManager */

  void slotRunStarted(AbstractDebugger*);
  void slotRunEnded();
  void slotDebugStarted(AbstractDebugger*);
  void slotDebugEnded();
  void slotCompileStarted(AbstractDebugger*);
  void slotCompileEnded();
  void slotDebugBreak();
  
  //Debugger internal error (conection, listen port, etc)
  void slotInternalError(const QString&);

  void slotCompileError(int, const QString&);
private:
  bool sanityCurrentFile();
  void connectDebugger(AbstractDebugger*);

  AbstractDebugger* m_activeDebugger;
  MainWindow       *m_window;

};

#endif
