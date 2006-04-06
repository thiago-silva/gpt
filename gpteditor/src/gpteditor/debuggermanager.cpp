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

#include "debuggermanager.h"
#include "mainwindow.h"
#include "debuggerstack.h"
#include "editortabwidget.h"
#include "abstractdebugger.h"
#include "debuggercombostack.h"
#include "variableslistview.h"
#include "watchlistview.h"
#include "messagelistview.h"
#include "debuggerbreakpoint.h"
#include "breakpointlistview.h"

#include "gpteditorsettings.h"
#include "debuggergpt.h"

#include <kapplication.h>
#include <kcombobox.h>
#include <kstatusbar.h>
#include <kaction.h>
#include <klineedit.h>
#include <ktextedit.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kdialogbase.h>
#include <kdebug.h>

DebuggerManager::DebuggerManager(MainWindow* window, QObject *parent, const char* name)
    : QObject(parent, name), m_activeDebugger(0), m_window(window)/*, m_showProfileDialog(false)*/
{}

/******************************* internal functions ******************************************/

void DebuggerManager::init()
{
  //connects the WATCH UI to us
//   connect(m_window->btAddWatch(), SIGNAL(clicked()),
//           this, SLOT(slotAddWatch()));

//   connect(m_window->edAddWatch(), SIGNAL(returnPressed()),
//           this, SLOT(slotAddWatch()));

//   connect(m_window->tabEditor(), SIGNAL(sigAddWatch(const QString&)),
//           this, SLOT(slotAddWatch(const QString&)));

//   connect(m_window->watchList(), SIGNAL(sigWatchRemoved(Variable*)),
//           this, SLOT(slotWatchRemoved(Variable*)));

//   connect(m_window->watchList(), SIGNAL(sigVarModified(Variable*)),
//           this, SLOT(slotLocalVarModified(Variable*)));


  //connects the STACK UI to us
  connect(m_window->stackCombo(),
          SIGNAL(changed(DebuggerExecutionPoint*, DebuggerExecutionPoint*)), this,
          SLOT(slotComboStackChanged(DebuggerExecutionPoint*, DebuggerExecutionPoint*)));

  //connects the Variables UI (VariablesList local/global) to us
  connect(m_window->globalVarList(), SIGNAL(sigVarModified(Variable*)),
          this, SLOT(slotGlobalVarModified(Variable*)));

  connect(m_window->localVarList(), SIGNAL(sigVarModified(Variable*)),
          this, SLOT(slotLocalVarModified(Variable*)));

  //connects the editor with the BREAKPOINT UI
  connect(m_window->tabEditor(),
          SIGNAL(sigBreakpointMarked(const KURL&, int, bool)),
          m_window->breakpointListView(),
          SLOT(slotBreakpointMarked(const KURL&, int, bool)));

  connect(m_window->tabEditor(),
          SIGNAL(sigBreakpointUnmarked(const KURL&, int )),
          m_window->breakpointListView(),
          SLOT(slotBreakpointUnmarked(const KURL&, int)));

  //connects the editor to us
  connect(m_window->tabEditor(), SIGNAL(sigNewDocument()),
          this, SLOT(slotNewDocument()));

  connect(m_window->tabEditor(), SIGNAL(sigNoDocument()),
          this, SLOT(slotNoDocument()));


  //connects the BREAKPOINT UI to us
  connect(m_window->breakpointListView(), SIGNAL(sigBreakpointCreated(DebuggerBreakpoint*)),
          this, SLOT(slotBreakpointCreated(DebuggerBreakpoint*)));

  connect(m_window->breakpointListView(), SIGNAL(sigBreakpointChanged(DebuggerBreakpoint*)),
          this, SLOT(slotBreakpointChanged(DebuggerBreakpoint*)));

  connect(m_window->breakpointListView(), SIGNAL(sigBreakpointRemoved(DebuggerBreakpoint*)),
          this, SLOT(slotBreakpointRemoved(DebuggerBreakpoint*)));

  connect(m_window->breakpointListView(), SIGNAL(sigDoubleClick(const KURL&, int)),
          this, SLOT(slotGotoLineAtFile(const KURL&, int)));

  //connects the LOG UI to us
  connect(m_window->messageListView(), SIGNAL(sigDoubleClick(const KURL&, int)),
          this, SLOT(slotGotoLineAtFile(const KURL&, int)));

  m_activeDebugger = new DebuggerGPT(this);

  connectDebugger(m_activeDebugger);

  m_activeDebugger->init();
}

DebuggerManager::~DebuggerManager()
{
  delete m_activeDebugger;
}

void DebuggerManager::connectDebugger(AbstractDebugger* debugger)
{
  //Connect the debugger to us
  
  connect(debugger, SIGNAL(sigRunStarted(AbstractDebugger*)),
          this, SLOT(slotRunStarted(AbstractDebugger*)));

  connect(debugger, SIGNAL(sigRunEnded()),
          this, SLOT(slotRunEnded()));

  connect(debugger, SIGNAL(sigCompileStarted(AbstractDebugger*)),
          this, SLOT(slotCompileStarted(AbstractDebugger*)));

  connect(debugger, SIGNAL(sigCompileEnded()),
          this, SLOT(slotCompileEnded()));
  
  connect(debugger, SIGNAL(sigDebugStarted(AbstractDebugger*)),
          this, SLOT(slotDebugStarted(AbstractDebugger*)));

  connect(debugger, SIGNAL(sigDebugEnded()),
          this, SLOT(slotDebugEnded()));

  connect(debugger, SIGNAL(sigInternalError(const QString&)),
          this, SLOT(slotInternalError(const QString&)));

  connect(debugger, SIGNAL(sigDebugBreak()),
          this, SLOT(slotDebugBreak()));


  connect(debugger, SIGNAL(sigCompileError(const QString&, const KURL&, int)),
          this, SLOT(slotCompileError(const QString&, const KURL&, int)));
}


/******************************* Application interface ******************************************/

bool DebuggerManager::sanityCurrentFile() 
{
  if(m_window->tabEditor()->count() == 0) 
  {
    m_window->showSorry(i18n("No file opened."));
    return false;
  } 
  else if(!m_window->tabEditor()->currentDocumentExistsOnDisk())
  {
    if(!m_window->saveCurrentFileAs()) 
    {
      return false;
    }
  }
  else
  {
    m_window->saveCurrentFile();
  }
  return true;
}

void DebuggerManager::slotCompileApplication()
{
  if(m_activeDebugger->isRunning())
  {
    m_activeDebugger->stop();
  }


  if(!sanityCurrentFile()) 
  {
    return;
  }

  m_window->messageListView()->clear();
  m_window->activateMessageTab();

  m_activeDebugger->compile(m_window->tabEditor()->currentDocumentURL());
}

void DebuggerManager::slotRunApplication() 
{
  if(m_activeDebugger->isRunning())
  {
    m_activeDebugger->stop();
  }


  if(!sanityCurrentFile()) 
  {
    return;
  }

  m_activeDebugger->run(m_window->tabEditor()->currentDocumentURL());
}

void DebuggerManager::slotDebugStart()
{
  if(!sanityCurrentFile()) 
  {
    return;
  }

  if(m_activeDebugger->isDebugging()) {
    clearStackMarks();
    m_window->setDebugStatusMsg(i18n("Continuing..."));
    m_activeDebugger->continueExecution();
  } else {
    m_activeDebugger->start(m_window->tabEditor()->currentDocumentURL());
  }
}

void DebuggerManager::slotDebugStop()
{
  if(!m_activeDebugger) return;

  m_window->setDebugStatusMsg(i18n("Stoping..."));
  m_activeDebugger->stop();
}

void DebuggerManager::slotDebugStepInto()
{
  if(!m_activeDebugger) return;

  m_window->setDebugStatusMsg(i18n("Steping..."));
  m_activeDebugger->stepInto();
}

void DebuggerManager::slotDebugStepOver()
{
  if(!m_activeDebugger) return;

  m_window->setDebugStatusMsg(i18n("Steping..."));
  m_activeDebugger->stepOver();
}

void DebuggerManager::slotDebugStepOut()
{
  if(!m_activeDebugger) return;

  m_window->setDebugStatusMsg(i18n("Steping..."));
  m_activeDebugger->stepOut();
}

//from the menu action
void DebuggerManager::slotDebugToggleBp()
{
  KURL url = m_window->tabEditor()->currentDocumentURL();
  int line = m_window->tabEditor()->currentDocumentLine();

  if(!m_window->tabEditor()->hasBreakpointAt(url, line))
  {
    m_window->tabEditor()->markActiveBreakpoint(url, line);
  }
  else
  {
    m_window->tabEditor()->unmarkActiveBreakpoint(url, line);
  }

  m_window->breakpointListView()->toggleBreakpoint(url, line);
}

void DebuggerManager::slotAddWatch()
{
  QString expression = m_window->edAddWatch()->text();
  m_window->edAddWatch()->clear();

  m_activeDebugger->addWatch(expression);
}

void DebuggerManager::slotAddWatch(const QString& expression)
{
  m_window->edAddWatch()->clear();
  m_activeDebugger->addWatch(expression);
}

void DebuggerManager::slotComboStackChanged(DebuggerExecutionPoint* old, DebuggerExecutionPoint* nw)
{
  //Glossary:
  //  -PreExecutionPoint: a DebuggerExecutionPoint representing a point in the stack of the backtrace.
  //                      This is used when the user selects a point on the stack ComboBox that is not the
  //                      topmost (the active line the debugger is running)
  
  
  //-set the current document/line according to the new stack context
  //-unmark the (possibly) previously PreExecutionPoint according to the old stack context
  //-mark the PreExecutionPoint of the new stack context
  //-request the variables for this context

  EditorTabWidget* ed = m_window->tabEditor();

  ed->gotoLineAtFile(nw->url(), nw->line()-1);

  ed->unmarkPreExecutionPoint(old->url()/*, old->line()*/);

  if(nw != m_window->stackCombo()->stack()->topExecutionPoint())
  {
    ed->markPreExecutionPoint(nw->url(), nw->line());
  }

  if(m_activeDebugger)
  {
    m_activeDebugger->changeCurrentExecutionPoint(nw);
  }
}

void DebuggerManager::slotGlobalVarModified(Variable* var)
{
  if(!m_activeDebugger) return;

  m_activeDebugger->modifyVariable(var,
                                   m_window->stackCombo()->stack()->bottomExecutionPoint());
}

void DebuggerManager::slotLocalVarModified(Variable* var)
{
  if(!m_activeDebugger) return;

  m_activeDebugger->modifyVariable(var,
                                   m_window->stackCombo()->selectedDebuggerExecutionPoint());
}

void DebuggerManager::slotBreakpointCreated(DebuggerBreakpoint* bp)
{
  if(!m_activeDebugger) return;
  m_activeDebugger->addBreakpoint(bp);
}

void DebuggerManager::slotBreakpointChanged(DebuggerBreakpoint* bp)
{
  if(m_activeDebugger) {
    m_activeDebugger->changeBreakpoint(bp);
  }

  switch(bp->status())
  {
    case DebuggerBreakpoint::ENABLED:
      m_window->tabEditor()->unmarkDisabledBreakpoint(bp->url(), bp->line());
      m_window->tabEditor()->markActiveBreakpoint(bp->url(), bp->line());
      break;
    case DebuggerBreakpoint::DISABLED:
      m_window->tabEditor()->unmarkActiveBreakpoint(bp->url(), bp->line());
      m_window->tabEditor()->markDisabledBreakpoint(bp->url(), bp->line());
    case DebuggerBreakpoint::UNRESOLVED:
      break;
  }
}

void DebuggerManager::slotBreakpointRemoved(DebuggerBreakpoint* bp)
{
  if(bp->status() == DebuggerBreakpoint::ENABLED) {
    m_window->tabEditor()->unmarkActiveBreakpoint(
      bp->url(), bp->line());
  } else {
    m_window->tabEditor()->unmarkDisabledBreakpoint(
      bp->url(), bp->line());
  }

  if(m_activeDebugger)
  {
    m_activeDebugger->removeBreakpoint(bp);
  }
}

void DebuggerManager::slotGotoLineAtFile(const KURL& url, int line)
{
  m_window->tabEditor()->gotoLineAtFile(url, line-1);
}

void DebuggerManager::slotWatchRemoved(Variable* var)
{
  m_activeDebugger->removeWatch(var->name());
}

void DebuggerManager::slotNoDocument()
{
  if(!m_activeDebugger) return;

  m_activeDebugger->stop();

}

void DebuggerManager::slotNewDocument()
{
  //if the new document has breakpoints, mark them.

  QValueList<DebuggerBreakpoint*> bplist =
    m_window->breakpointListView()->breakpointsFrom(
      m_window->tabEditor()->currentDocumentURL().prettyURL());

  QValueList<DebuggerBreakpoint*>::iterator it;
  for(it = bplist.begin(); it != bplist.end(); ++it)
  {
    m_window->tabEditor()->markActiveBreakpoint((*it)->url(), (*it)->line());
  }
}

/******************************* Debugger interface ******************************************/

void DebuggerManager::clearStackMarks() 
{
  //**** dealing with the current stackCombo (MARKS stuff)
  //-if stackCombo isn't empty, unmark the previously marked ExecutionMark
  // (wich is the top context of the comboStack)
  //-if comboStack is not pointed to the top context, unmark the previously
  // marked PreExecutionPoint and request the vars for this context

  //Conveniece vars:
  DebuggerExecutionPoint* execPoint;
  EditorTabWidget* ed = m_window->tabEditor();

  if(m_window->stackCombo()->count() > 0)
  {
    execPoint =
      m_window->stackCombo()->stack()->topExecutionPoint();

    ed->unmarkExecutionPoint(execPoint->url()/*, execPoint->line()*/);
  }

  if(m_window->stackCombo()->currentItem() != 0)
  {
    execPoint =
      m_window->stackCombo()->selectedDebuggerExecutionPoint();

    ed->unmarkPreExecutionPoint(execPoint->url()/*, execPoint->line()*/);
  }
}

void DebuggerManager::updateStack(DebuggerStack* stack)
{
  clearStackMarks();

  DebuggerExecutionPoint* execPoint;
  EditorTabWidget* ed = m_window->tabEditor();


  //**** dealing with the new Stack (the argument)
  //-set the current document/line according to the top context of the stack
  //-mark the new ExecutionPoint
  //-sets the stack to the comboStack

  //lets update first and force the user to be on the top of the stack
  m_window->stackCombo()->setStack(stack);

  execPoint = stack->topExecutionPoint();
  ed->setCurrentDocumentTab(execPoint->url(), true);
  ed->gotoLineAtFile(execPoint->url(), execPoint->line()-1);

  ed->markExecutionPoint(execPoint->url(), execPoint->line());
}

void DebuggerManager::updateGlobalVars(VariablesList_t* vars)
{
  m_window->globalVarList()->setVariables(vars);
}

void DebuggerManager::updateLocalVars(VariablesList_t* vars)
{
  m_window->localVarList()->setVariables(vars);
}

void DebuggerManager::updateWatch(Variable* var)
{
  m_window->watchList()->addWatch(var);
}

void DebuggerManager::updateBreakpoint(DebuggerBreakpoint* bp)
{
  m_window->breakpointListView()->updateBreakpoint(bp);
}

void DebuggerManager::debugMessage(int type, const QString& msg, const KURL& url, int line)
{
  if(type == DebuggerManager::ErrorMsg) 
  {
    m_window->tabEditor()->gotoLineAtFile(url, line);
  }
  m_window->messageListView()->add(type, msg, line, url);
}

void DebuggerManager::slotCompileError(const QString& msg, const KURL& file, int line)
{
  m_window->messageListView()->add(DebuggerManager::ErrorMsg, msg, line, file);
}

void DebuggerManager::error(const QString& msg)
{
  m_window->showError(msg);  
}

void DebuggerManager::slotCompileStarted(AbstractDebugger* debugger)
{
  m_activeDebugger = debugger;
  
  //clear everyone
  m_window->messageListView()->clear();

  m_window->setDebugStatusMsg(i18n("Compiling..."));
  m_window->setDebugStatusName(m_activeDebugger->name()); //show the name of the debugger on the StatusBar
  m_window->setLedState(MainWindow::LedOn);               //green led

  //setup the actions stuff
  m_window->actionStateChanged("compile_started");
}

void DebuggerManager::slotCompileEnded()
{
  //setup the actions stuff
  m_window->actionStateChanged("debug_stopped");
 
  m_window->setDebugStatusMsg(i18n("Finished"));
  m_window->setLedState(MainWindow::LedOff); //red led

  m_window->setDebugStatusName("");
}

void DebuggerManager::slotRunStarted(AbstractDebugger* debugger) 
{
  m_activeDebugger = debugger;
  
  //clear everyone
  m_window->messageListView()->clear();

  m_window->setDebugStatusMsg(i18n("Executable started"));
  m_window->setDebugStatusName(m_activeDebugger->name()); //show the name of the debugger on the StatusBar
  m_window->setLedState(MainWindow::LedOn);               //green led

  //setup the actions stuff
  m_window->actionStateChanged("executable_started");
}

void DebuggerManager::slotRunEnded() 
{
  //setup the actions stuff
  m_window->actionStateChanged("debug_stopped");
 
  m_window->setDebugStatusMsg(i18n("Finished"));
  m_window->setLedState(MainWindow::LedOff); //red led

  m_window->setDebugStatusName("");  
}

void DebuggerManager::slotDebugStarted(AbstractDebugger* debugger)
{
  //let the party begins...
  
  m_activeDebugger = debugger;
  
  //send all breakpoints to the debugger 
  m_activeDebugger->addBreakpoints(
    m_window->breakpointListView()->breakpoints());

  //let the user change the variables 
//   m_window->globalVarList()->setReadOnly(false);
//   m_window->localVarList()->setReadOnly(false);
//   m_window->watchList()->setReadOnly(false);

  //clear everyone
  m_window->globalVarList()->clear();
  m_window->localVarList()->clear();
  m_window->messageListView()->clear();
  m_window->stackCombo()->clear();

  m_window->setDebugStatusMsg(i18n("Debug started"));
  m_window->setDebugStatusName(m_activeDebugger->name()); //show the name of the debugger on the StatusBar
  m_window->setLedState(MainWindow::LedOn);               //green led

  //setup the actions stuff
  m_window->actionStateChanged("debug_started");
  m_window->actionCollection()->action("debug_start")->setText(i18n("Continue"));
  
  /*
  m_window->actionCollection()->action("debug_stop")->setEnabled(true);
  m_window->actionCollection()->action("debug_step_into")->setEnabled(true);
  m_window->actionCollection()->action("debug_step_over")->setEnabled(true);
  m_window->actionCollection()->action("debug_step_out")->setEnabled(true);
  */
}

void DebuggerManager::slotDebugEnded()
{
  //reset all breakpoints to their original state
  m_window->breakpointListView()->resetBreakpointItems();

  //do not let the user change variables anymore
//   m_window->globalVarList()->setReadOnly(true);
//   m_window->localVarList()->setReadOnly(true);
//   m_window->watchList()->setReadOnly(true);

  //setup the actions stuff
  m_window->actionStateChanged("debug_stopped");
  m_window->actionCollection()->action("debug_start")->setText(i18n("Start Debug"));
  
  m_window->setDebugStatusMsg(i18n("Stopped"));
  m_window->setLedState(MainWindow::LedOff); //red led

  EditorTabWidget* ed = m_window->tabEditor();

  DebuggerStack* stack = m_window->stackCombo()->stack();

  if(stack)
  {
    //remove the execution line mark
    DebuggerExecutionPoint* execPoint;
    execPoint = stack->topExecutionPoint();

    ed->unmarkExecutionPoint(execPoint->url()/*, execPoint->line()*/);

    //remove the pre execution line mark if any
    execPoint =
      m_window->stackCombo()->selectedDebuggerExecutionPoint();

    ed->unmarkPreExecutionPoint(execPoint->url()/*, execPoint->line()*/);
  }
  
  /*
  m_window->actionCollection()->action("debug_stop")->setEnabled(false);
  m_window->actionCollection()->action("debug_step_into")->setEnabled(false);
  m_window->actionCollection()->action("debug_step_over")->setEnabled(false);
  m_window->actionCollection()->action("debug_step_out")->setEnabled(false);
  */

  m_window->setDebugStatusName("");
}

void DebuggerManager::slotDebugBreak()
{
  //step(into/over/out) or a breakpoint or something like happened. We don't care too mutch about it :)
  m_window->setDebugStatusMsg(i18n("Done"));
}

void DebuggerManager::slotInternalError(const QString& message)
{
  m_window->showError(message);
}


#include "debuggermanager.moc"
