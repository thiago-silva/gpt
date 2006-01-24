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

#ifndef DEBUGGERGPT_H
#define DEBUGGERGPT_H

#include "abstractdebugger.h"
#include "variable.h"

#include <qmap.h>
#include <kurl.h>

class DebuggerStack;
class DebuggerManager;
class KProcess;
class GPTNet;


class DebuggerGPT : public AbstractDebugger
{
  Q_OBJECT
public:
  DebuggerGPT(DebuggerManager*);
  virtual ~DebuggerGPT();

  virtual QString name() const;
  virtual bool isRunning() const;
  virtual void init();

  virtual void compile(const KURL&);
  virtual void run(const KURL&);
  virtual void start(const KURL&);
  virtual void continueExecution();
  virtual void stop();
  virtual void stepInto();
  virtual void stepOver();
  virtual void stepOut();

  virtual void addBreakpoints(const QValueList<DebuggerBreakpoint*>&);
  virtual void addBreakpoint(DebuggerBreakpoint*);
  virtual void changeBreakpoint(DebuggerBreakpoint*);
  virtual void removeBreakpoint(DebuggerBreakpoint*);

  virtual void changeCurrentExecutionPoint(DebuggerExecutionPoint*);
  virtual void modifyVariable(Variable* v, DebuggerExecutionPoint*);

  virtual void addWatch(const QString& expression);
  virtual void removeWatch(const QString& expression);

  void updateStack(DebuggerStack* stack);
  void updateVariables(VariablesList_t* array, bool isGlobal);

  KURL currentURL();
public slots:
  void slotSettingsChanged();



private slots:
  void slotExecFinished(KProcess*);

/*    void slotGBStarted();
    void slotGBClosed();*/
//     void slotStepDone();
//     void slotInternalError(const QString&);
  
  void slotGPTStarted();
  void slotGPTStopped();
  void slotCompileOutput(KProcess*,char*,int);
  
private:
//   void requestWatches();

  enum { EnumCompile, EnumRunInterpreted, EnumRunBinary };

  void startJIT();

  void runBinary(const KURL& srcURL);
  void runInterpreted(const KURL& srcURL, bool debugging = false);

  void cleanExec();
  void doStop();

  void processCompileOutput();

  QString m_name;
  bool m_isRunning;
  KProcess* m_exec;
  int m_execType;

  GPTNet* m_net;
  QValueList<QString>  m_wathcesList;  

  QString m_compileOutput;
  KURL m_currentURL;
};

#endif
