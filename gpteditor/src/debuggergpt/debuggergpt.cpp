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

#include "debuggergpt.h"
#include "gpteditorsettings.h"
#include "debuggermanager.h"
#include "debuggerstack.h"
#include "debuggerbreakpoint.h"
#include "gptnet.h"

#include <kurl.h>
#include <kfileitem.h>
#include <kprocess.h>
#include <kdebug.h>
#include <qapplication.h>
#include <qregexp.h>
#include <klocale.h>


DebuggerGPT::DebuggerGPT(DebuggerManager* manager)
    : AbstractDebugger(manager), m_name("GPT"), m_isRunning(false), m_exec(0), m_execType(-1)
{
  connect(GPTEditorSettings::self(), SIGNAL(sigSettingsChanged()),
          this, SLOT(slotSettingsChanged()));

  m_net = new GPTNet(this);

  connect(m_net, SIGNAL(sigGPTStarted()), this, SLOT(slotGPTStarted()));
  connect(m_net, SIGNAL(sigGPTClosed()), this, SLOT(slotGPTStopped()));
//   connect(m_net, SIGNAL(sigError(const QString&)), this, SLOT(slotInternalError(const QString&)));
//   connect(m_net, SIGNAL(sigStepDone()), this, SLOT(slotStepDone()));
}

DebuggerGPT::~DebuggerGPT()
{
  cleanExec();
}


QString DebuggerGPT::name() const
{
  return m_name;
}

bool DebuggerGPT::isRunning() const
{
  return m_isRunning;
}

void DebuggerGPT::init()
{
  startJIT();
}

void DebuggerGPT::compile(const KURL& srcURL)
{
  cleanExec();

  //TODO: testar compilacao de arquivos sem extensao
  KURL binURL = srcURL.directory() + "/" + srcURL.fileName().section('.',0,0);
  

  KFileItem file(KFileItem::Unknown, KFileItem::Unknown, srcURL); 

  if(!file.isReadable()) {
    manager()->error(
      i18n("Unable to compile file \"%1\".").arg(srcURL.prettyURL()));
    return;
  }

  m_exec = new KProcess;

  kdDebug() << "compiling: " << " gpt " 
            << GPTEditorSettings::self()->GPTArgs() << " "
            << srcURL.path() << " -o " << binURL.path() << endl;

  *m_exec << "gpt" << srcURL.path() << "-o" << binURL.path();

  connect(m_exec, SIGNAL(processExited(KProcess *)),
    this, SLOT(slotExecFinished(KProcess*)));

  connect(m_exec, SIGNAL(receivedStderr(KProcess*,char*,int)),
    this, SLOT(slotCompileOutput(KProcess*,char*,int)));

  m_currentURL = srcURL;
  m_execType = EnumCompile;

  m_exec->start(KProcess::NotifyOnExit, KProcess::Stderr);

  emit sigCompileStarted(this);
}

void DebuggerGPT::run(const KURL& url)
{
  cleanExec();

  if(GPTEditorSettings::self()->executionMode() == GPTEditorSettings::EnumCompiled)
  {
    runBinary(url);  
  } 
  else
  {
    runInterpreted(url);
  }
}

void DebuggerGPT::runInterpreted(const KURL& srcURL, bool debugging) 
{
  m_exec = new KProcess;

  QString shell = GPTEditorSettings::self()->shell();

  QString cmd = "gpt -i ";

  if(debugging) 
  {
    cmd += "-H " + GPTEditorSettings::self()->host()
        +  " -P " + QString::number(GPTEditorSettings::self()->listenPort()) + " ";
  }
  cmd += i18n("%1 %2;echo \"Press any key to continue...\";read").arg(
    GPTEditorSettings::self()->GPTArgs(), srcURL.path());

//   cmd += GPTEditorSettings::self()->GPTArgs()
//       + " "
//       + srcURL.path()
//       + ";echo \"Pressione Enter para continuar...\";read";

  *m_exec << "konsole" << "-e" << shell 
    << "-c" << cmd;

  connect(m_exec, SIGNAL(processExited(KProcess *)),
                 this, SLOT(slotExecFinished(KProcess*)));

  m_execType = EnumRunInterpreted;

  m_exec->start();

  emit sigRunStarted(this);
}

void DebuggerGPT::runBinary(const KURL& srcURL) 
{
  //get the binary file name from source path
  QString bin = srcURL.directory() + "/" + srcURL.fileName().section('.',0,0);

  KFileItem file(KFileItem::Unknown, KFileItem::Unknown, bin); 

  if(!file.isReadable()) {    
    manager()->error(i18n
      ("Unable to execute the algorithm. Check if it was compiled.").arg(srcURL.prettyURL()));
    return;
  }

  m_exec = new KProcess;
  //*proc << "konsole" << "-e" << "/bin/sh" << "-c" << "'c d/teste';read";

  QString shell = GPTEditorSettings::self()->shell();

  *m_exec << "konsole" << "-e" << shell << "-c" 
    << KProcess::quote(bin) + i18n(";echo \"Press any key to continue...\";read");

  connect(m_exec, SIGNAL(processExited(KProcess *)),
                this, SLOT(slotExecFinished(KProcess*)));

  m_execType = EnumRunBinary;

  m_exec->start();

  emit sigRunStarted(this);
}

bool DebuggerGPT::isDebugging() const {
  return m_isRunning && (m_execType == EnumRunInterpreted);
}

void DebuggerGPT::start(const KURL& url)
{
  cleanExec();
  runInterpreted(url, true);
  m_currentURL = url;
}

KURL DebuggerGPT::currentURL()
{
  return m_currentURL;
}

void DebuggerGPT::continueExecution()
{
  if(isRunning()) m_net->requestContinue();
}

void DebuggerGPT::stop()
{
  if(isRunning()) doStop();
}

void DebuggerGPT::stepInto()
{
  if(isRunning()) m_net->requestStepInto();
}

void DebuggerGPT::stepOver()
{
  if(isRunning()) m_net->requestStepOver();
}

void DebuggerGPT::stepOut()
{
  if(isRunning()) m_net->requestStepOut();
}

void DebuggerGPT::addBreakpoints(const QValueList<DebuggerBreakpoint*>& bps)
{
  if(isRunning())
  {
    QValueList<DebuggerBreakpoint*>::const_iterator it;
    for(it = bps.begin(); it != bps.end(); ++it)
    {
      if((*it)->status() == DebuggerBreakpoint::ENABLED) {
        m_net->requestBreakpoint((*it)->url().path(), (*it)->line());
      }
    }
  }
}

void DebuggerGPT::addBreakpoint(DebuggerBreakpoint* bp)
{
  if(isRunning())
  {
    m_net->requestBreakpoint(bp->url().path(), bp->line());
  }
}

void DebuggerGPT::changeBreakpoint(DebuggerBreakpoint* bp)
{
  //if(isRunning() && (bp->url() == currentURL()))
  if(isRunning())
  {
    if(bp->status() == DebuggerBreakpoint::DISABLED) {
      m_net->requestBreakpointRemoval(bp->url().path(), bp->line());
    } else {
      m_net->requestBreakpoint(bp->url().path(), bp->line());
    }
  }
}

void DebuggerGPT::removeBreakpoint(DebuggerBreakpoint* bp)
{
  if(isRunning())
  {
    m_net->requestBreakpointRemoval(bp->url().path(), bp->line());
  }
}

void DebuggerGPT::changeCurrentExecutionPoint(DebuggerExecutionPoint*)
{}

void DebuggerGPT::modifyVariable(Variable*, DebuggerExecutionPoint*)
{}

void DebuggerGPT::addWatch(const QString& )
{}

void DebuggerGPT::removeWatch(const QString& )
{}

void DebuggerGPT::slotSettingsChanged()
{
  startJIT();
}

void DebuggerGPT::startJIT()
{
  if(!m_net->startListener())
  {
    emit sigInternalError(i18n("Unable to listen on port: %1").arg(GPTEditorSettings::self()->listenPort()));
  }
}

void DebuggerGPT::cleanExec()
{
  if(m_exec)
  {
    m_exec->kill();
  }
  delete m_exec;
  m_exec = 0;
}

void DebuggerGPT::doStop()
{
  m_isRunning = false;

  cleanExec();

  switch(m_execType)
  {    
    case EnumRunInterpreted:
    case EnumRunBinary:
      emit sigRunEnded();
      break;
    case EnumCompile:
      emit sigCompileEnded();
      processCompileOutput();
      break;
    default:
      kdDebug() << "BUG: DebuggerGPT::slotExecFinished()" << endl;
  }  
}

void DebuggerGPT::processCompileOutput()
{
  kdDebug() << "received: >>>" << m_compileOutput << "<<<" << endl ;

  if(m_compileOutput.isEmpty()) {
    manager()->debugMessage(DebuggerManager::InfoMsg, i18n("Compilation successful"), m_currentURL, 1);
    return;
  }

  int line;
  QString errorMsg;
  QString file;

  // parser error!

  QRegExp rx;
  int idx = 0;
  //rx.setPattern("([^:]*):(\\d+) - (([^.]|\\.\\.\\.)+)\\.");
  rx.setPattern("([^:]*):(\\d+) - ([^\\n]+)\\n");
  bool hasErrors = false;
  while(rx.search(m_compileOutput, idx) != -1) {

    idx += rx.matchedLength();

    file = rx.cap(1);
    line =  rx.cap(2).toInt();
    errorMsg = rx.cap(3);
    
    hasErrors = true;
    emit sigCompileError(errorMsg, KURL::fromPathOrURL(file), line);
  }

  //gpt error

  idx = 0;
  rx.setPattern("gpt: (.*)");
  while(rx.search(m_compileOutput, idx) != -1) {

    idx += rx.matchedLength();
    errorMsg = rx.cap(1);
    
    hasErrors = true;
    emit sigCompileError(errorMsg, KURL(), 0);
  }

  if(!hasErrors) 
  {
    manager()->debugMessage(DebuggerManager::InfoMsg, i18n("Compilation successful"), m_currentURL, 1);
  }

  m_compileOutput = "";
}

void DebuggerGPT::slotExecFinished(KProcess*) 
{
  doStop();
}

void DebuggerGPT::slotCompileOutput(KProcess* ,char* str,int len)
{
  QString rec;
  rec.setLatin1(str, len);
  m_compileOutput += rec;
}

//net started
void DebuggerGPT::slotGPTStarted()
{
  m_isRunning = true;  
  emit sigDebugStarted(this);
}

//net stopped
void DebuggerGPT::slotGPTStopped()
{
  m_isRunning = false;
  emit sigDebugEnded();
}

void DebuggerGPT::updateStack(DebuggerStack* stack)
{
//   m_currentExecutionPoint = stack->topExecutionPoint();
//   m_globalExecutionPoint  = stack->bottomExecutionPoint();
  manager()->updateStack(stack);
  emit sigDebugBreak();
}

void DebuggerGPT::updateVariables(VariablesList_t* array, bool isGlobal)
{
  if(isGlobal)
  {
    manager()->updateGlobalVars(array);
  }
  else
  {
    manager()->updateLocalVars(array);
  }
}

#include "debuggergpt.moc"
