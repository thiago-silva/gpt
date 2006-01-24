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

#ifndef ABSTRACTDEBUGGER_H
#define ABSTRACTDEBUGGER_H

#include <qobject.h>
#include <qstring.h>

class KDialog;
class DebuggerManager;
class DebuggerBreakpoint;
class DebuggerConfigurations;
class DebuggerExecutionPoint;
class DebuggerStack;
class Variable;
class KURL;

class AbstractDebugger : public QObject
{
  Q_OBJECT
public:
  AbstractDebugger(QObject *parent, const char* name = 0);
  virtual ~AbstractDebugger();

  /* Most (if not all) of those functions are meant to be called from the DebuggerManager
  */

  /* the name of the debugger */
  virtual QString name()   const = 0;

  //virtual int     id() const = 0; // might have some use in the future to have this

  /* if a debug session is active */
  virtual bool isRunning()       const = 0;

  /* inits the client (listen to port, inits members, order a pizza....) */
  virtual void init() = 0;

  /* commands commonly supported by a debugger. */
  virtual void compile(const KURL&)= 0;
  virtual void run(const KURL&)    = 0;
  virtual void start(const KURL&)  = 0;
  virtual void continueExecution() = 0;
  virtual void stop()              = 0;
  virtual void stepInto()          = 0;
  virtual void stepOver()          = 0;
  virtual void stepOut()           = 0;
  //virtual void pause()           = 0; //TODO

  /* adds a list of breakpoints.
  Its normaly (I guess, _only_!) called when the DebuggerManager is notified with the sigDebugStarted() signal*/
  virtual void addBreakpoints(const QValueList<DebuggerBreakpoint*>&) = 0;

  /* DebuggerManager says: add a single breakpoint! */
  virtual void addBreakpoint(DebuggerBreakpoint*)               = 0;

  virtual void changeBreakpoint(DebuggerBreakpoint*) = 0;
  virtual void removeBreakpoint(DebuggerBreakpoint*) = 0;

  /* Called when the user changes the execution point on the stack ComboBox.
     What we want when this happen is to retrieve local variables and related data
     when this happens.
  */
  virtual void changeCurrentExecutionPoint(DebuggerExecutionPoint*) = 0;

  /* The user modified a variable (probably through the VariablesListView) */
  virtual void modifyVariable(Variable* v, DebuggerExecutionPoint*) = 0;

  virtual void addWatch(const QString& expression) = 0;
  virtual void removeWatch(const QString& expression) = 0;


protected:
  virtual DebuggerManager* manager();

signals:
  /* Ifnroms that we are running without debugging. */
  void sigRunStarted(AbstractDebugger*);
  void sigRunEnded();

  void sigCompileStarted(AbstractDebugger*);
  void sigCompileEnded();

  /* Informs that we are active on a session. */
  void sigDebugStarted(AbstractDebugger*);

  void sigDebugEnded();

  /* When a step(in/over/out) is done, or a breakpoint is reached, and so on */
  void sigDebugBreak();

  //Debugger client error (conection, listen port, etc)
  void sigInternalError(const QString&);

  void sigCompileError(int, const QString&);
};

#endif
