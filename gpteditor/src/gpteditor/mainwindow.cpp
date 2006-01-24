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

#include "mainwindow.h"
#include "editortabwidget.h"
#include "variableslistview.h"
#include "watchlistview.h"
#include "messagelistview.h"
#include "debuggercombostack.h"
#include "breakpointlistview.h"
#include "debuggermanager.h"
#include "configdlg.h"
#include "gpteditorsettings.h"

#include <kapplication.h>
#include <kstatusbar.h>
#include <kaction.h>
#include <klineedit.h>
#include <kpushbutton.h>
#include <ktextedit.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kkeydialog.h>
#include <kedittoolbar.h>
#include <ktexteditor/view.h>
#include <kfiledialog.h>
#include <kfileitem.h>
#include <qvaluelist.h>


#include <kdialogbase.h>
#include <kiconloader.h>
#include <ktextedit.h>

/*
#include <ktexteditor/document.h>
#include <ktexteditor/editorchooser.h>
#include <ktexteditor/view.h>
#include <ktexteditor/editinterface.h>
*/


#include <qlayout.h>
#include <qsplitter.h>


MainWindow::MainWindow(QWidget* parent, const char* name, WFlags fl)
    : KParts::MainWindow(parent, name, fl)
{
  setXMLFile( "gpteditorui.rc" );
  if(!name) { setName("MainWindow"); }

  setupStatusBar();

  m_debugger_manager = new DebuggerManager(this);

  createWidgets();

  setupActions();

  createGUI(0);

  resize( QSize(633, 533).expandedTo(minimumSizeHint()) );
  clearWState(WState_Polished);

  m_debugger_manager->init();

  connect(kapp, SIGNAL(aboutToQuit()), this, SLOT(slotClose()));

  stateChanged("init");
}

void MainWindow::setupStatusBar()
{
  m_statusBar = new KStatusBar(this);

  //first item: debug led
  m_lbLed = new QLabel(this);
  m_lbLed->setPixmap(QPixmap(UserIcon("indicator_off")));
  m_lbLed->setAlignment( Qt::AlignCenter );
  m_lbLed->setSizePolicy(QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ));
  m_statusBar->addWidget(m_lbLed);

  //second item: debugger name
  m_lbDebugName = new QLabel(this);
  m_lbDebugName->setMinimumSize( 40, 0 );
  //m_lbDebugMsg->setSizePolicy(QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ));
  m_statusBar->addWidget(m_lbDebugName, 0 /*stretch*/, false );
  
  //third item: debug msgs
  m_lbDebugMsg = new QLabel(this);
  m_lbDebugMsg->setMinimumSize( 0, 0 );
  m_lbDebugMsg->setSizePolicy(QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed ));
  m_statusBar->addWidget(m_lbDebugMsg, 1 /*stretch*/, false );

  //fourth item: editor msgs
  m_lbEditorMsg = new QLabel(this);
  m_lbEditorMsg->setMinimumSize( 230, 0 );
  m_lbEditorMsg->setSizePolicy(QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ));
  m_lbEditorMsg->setAlignment( Qt::AlignCenter );
  m_statusBar->addWidget(m_lbEditorMsg, 1 /*stretch*/, false );

}

void MainWindow::setupActions()
{
  //file menu
  KStdAction::openNew(this, SLOT(slotNewFile()), actionCollection());
  KStdAction::open(this, SLOT(slotOpenFile()), actionCollection());

  m_actionRecent = KStdAction::openRecent(this, SLOT(slotFileRecent(const KURL&)), actionCollection());
  m_actionRecent->loadEntries(kapp->config());//,"Recent Files");

  KStdAction::save(this, SLOT(slotSaveCurrentFile()), actionCollection());
  KStdAction::saveAs(this, SLOT(slotSaveCurrentFileAs()), actionCollection());

  KStdAction::close(this, SLOT(slotCloseFile()), actionCollection());

  (void)new KAction(i18n("Close All"), 0, this, SLOT(slotCloseAllFiles()), actionCollection(), "file_close_all");

  KStdAction::quit(this, SLOT(slotQuit()), actionCollection());

  KStdAction::keyBindings(this, SLOT(slotEditKeys()), actionCollection());
  KStdAction::configureToolbars(this, SLOT(slotEditToolbars()), actionCollection());
  KStdAction::preferences(this, SLOT(slotShowSettings()), actionCollection(), "settings_gpteditor");


  (void)new KAction(i18n("Compile"), "make_kdevelop", "F10", m_debugger_manager,
                    SLOT(slotCompileApplication()), actionCollection(), "compile_application");

  (void)new KAction(i18n("Run"), "gear", "F9", m_debugger_manager,
                    SLOT(slotRunApplication()), actionCollection(), "run_application");

  (void)new KAction(i18n("Start Debug"), "dbgstart", "F5", m_debugger_manager,
                    SLOT(slotDebugStart()), actionCollection(), "debug_start");

  (void)new KAction(i18n("Stop Debug"), "stop", "Escape", m_debugger_manager,
                    SLOT(slotDebugStop()), actionCollection(), "debug_stop");

  (void)new KAction(i18n("Step Over"), "dbgnext", "F6", m_debugger_manager,
                    SLOT(slotDebugStepOver()), actionCollection(), "debug_step_over");

  (void)new KAction(i18n("Step Into"), "dbgstep", "F7", m_debugger_manager,
                    SLOT(slotDebugStepInto()), actionCollection(), "debug_step_into");

  (void)new KAction(i18n("Step Out"), "dbgstepout", "F8", m_debugger_manager,
                    SLOT(slotDebugStepOut()), actionCollection(), "debug_step_out");

//   (void)new KAction(i18n("Profile (DBG only)"), "math_sum", "Alt+P", m_debugger_manager,
//                     SLOT(slotProfile()), actionCollection(), "script_profile");

  (void)new KAction(i18n("Toggle Breakpoint"), "activebreakpoint", "Alt+B", m_debugger_manager,
                    SLOT(slotDebugToggleBp()), actionCollection(), "debug_toggle_bp");

//   (void)new KAction("", "math_brace", 0, m_tabEditor,
//                     SLOT(slotAddWatch()), actionCollection(), "editor_add_watch");


  // Editor default action

  setStandardToolBarMenuEnabled(true);
}

void MainWindow::createWidgets()
{
  setCentralWidget(new QWidget(this));
  QVBoxLayout* mainFormBaseLayout = new QVBoxLayout(centralWidget(), 1, 1);
  
/*  
  QHBoxLayout* cbLayout = new QHBoxLayout(centralWidget(), 1, 1);
  
  QLabel *lb = new QLabel("GET:", centralWidget());
  lb->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lb->sizePolicy().hasHeightForWidth()));
  cbLayout->addWidget(lb);
  QComboBox* cb = new QComboBox(centralWidget());
  cb->setEditable(true);
  cb->insertItem("oi");
  cb->insertItem("123");
  cbLayout->addWidget(cb);
  
  lb = new QLabel("POST:", centralWidget());
  lb->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lb->sizePolicy().hasHeightForWidth()));
  cbLayout->addWidget(lb);
  cb = new QComboBox(centralWidget());
  cb->setEditable(true);
  cb->insertItem("12445");
  cb->insertItem("123");
  cbLayout->addWidget(cb);
      
  mainFormBaseLayout->addLayout(cbLayout);
*/

  QSplitter* splitter = new QSplitter(centralWidget());
  splitter->setOrientation( Qt::Vertical  );
  mainFormBaseLayout->addWidget(splitter);  
  
  m_tabEditor = new EditorTabWidget(splitter, this);
  splitter->setCollapsible(m_tabEditor, false);
  splitter->setOpaqueResize(true);

  connect(m_tabEditor, SIGNAL(sigParseError(int, const QString&, const KURL&)),
    this, SLOT(slotParseError(int, const QString&, const KURL&)));

  m_tabDebug = new QTabWidget(splitter);
  splitter->setCollapsible(m_tabDebug, false);
  m_tabDebug->setGeometry(0,0,0,height()/15);

  QWidget* globalVarTab = new QWidget(m_tabDebug);
  QVBoxLayout* globalVarTabLayout = new QVBoxLayout(globalVarTab, 1, 1);

  m_globaVarList = new VariablesListView(globalVarTab);
  globalVarTabLayout->addWidget(m_globaVarList);
  m_tabDebug->insertTab(globalVarTab, i18n("Global"));

  QWidget* tabStack = new QWidget(m_tabDebug);
  QVBoxLayout* varTabLayout = new QVBoxLayout(tabStack, 1, 1);
  QHBoxLayout* stackComboLayout = new QHBoxLayout(0, 6, 6);

  QLabel* stackLabel = new QLabel(tabStack);
  stackLabel->setText(i18n("Stack:"));
  stackLabel->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, stackLabel->sizePolicy().hasHeightForWidth()));
  stackComboLayout->addWidget(stackLabel);

  m_stackCombo = new DebuggerComboStack(tabStack);
  stackComboLayout->addWidget(m_stackCombo);
  varTabLayout->addLayout(stackComboLayout);

  m_localVarList= new VariablesListView(tabStack);
  varTabLayout->addWidget(m_localVarList);
  m_tabDebug->insertTab(tabStack, i18n("Local"));

//   QWidget* tabWatch = new QWidget(m_tabDebug);
//   QVBoxLayout* watchTabLayout = new QVBoxLayout(tabWatch, 1, 1);
// 
//   QHBoxLayout* addWatchLayout = new QHBoxLayout(0, 6, 6);
// 
//   QLabel* addLabel = new QLabel(tabWatch);
//   addLabel->setText(("Watch:"));
//   addWatchLayout->addWidget(addLabel);
// 
//   m_edAddWatch = new KLineEdit(tabWatch);
//   m_edAddWatch->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, m_edAddWatch->sizePolicy().hasHeightForWidth()));
//   addWatchLayout->addWidget(m_edAddWatch);
// 
//   m_btAddWatch = new KPushButton(tabWatch);
//   m_btAddWatch->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, m_btAddWatch->sizePolicy().hasHeightForWidth()));
//   m_btAddWatch->setText(("Add"));
//   addWatchLayout->addWidget(m_btAddWatch);
// 
//   QSpacerItem* spacer = new QSpacerItem(430, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
//   addWatchLayout->addItem(spacer);
//   watchTabLayout->addLayout(addWatchLayout);
// 
//   m_watchList = new WatchListView(tabWatch);
//   watchTabLayout->addWidget(m_watchList);
//   m_tabDebug->insertTab(tabWatch, QString("Watch"));

  QWidget* breakpointTab = new QWidget(m_tabDebug);
  QVBoxLayout* breakpointTabLayout = new QVBoxLayout(breakpointTab, 1, 1);

  m_breakpointList = new BreakpointListView(breakpointTab);
  breakpointTabLayout->addWidget(m_breakpointList);
  m_tabDebug->insertTab(breakpointTab, i18n("Breakpoints"));

  QWidget* logTab = new QWidget(m_tabDebug);
  QVBoxLayout* logTabLayout = new QVBoxLayout(logTab, 1, 1);

  m_messageListView = new MessageListView(logTab);
  logTabLayout->addWidget(m_messageListView);
  m_tabDebug->insertTab(logTab, i18n("Messages"));

//   m_edOutput = new KTextEdit(outputTab);
//   outputTabLayout->addWidget(m_edOutput);
//   m_edOutput->setReadOnly(true);
//   m_edOutput->setTextFormat(Qt::PlainText);
//   m_edOutput->setPaper( QBrush(QColor("white")));

  /*
  KTextEditor::Document* doc = KTextEditor::EditorChooser::createDocument(
      0L, "KTextEditor::Document");
  //doc->setReadWrite(false);
  m_edOutput = dynamic_cast<KTextEditor::EditInterface*>(doc);
  m_edOutput->setText("oioi");
  outputTabLayout->addWidget(doc->createView(outputTab));
  */
}

MainWindow::~MainWindow()
{
  delete m_debugger_manager;
}

void MainWindow::openFile()
{
  slotOpenFile();
}

void MainWindow::slotNewFile() {
  m_tabEditor->openNew();
}

void MainWindow::slotOpenFile()
{
  //Show KFileDialog on the current Site's "local base dir" or
  //use ::protoeditor to retrieve the last folder used

  QString location = ":gpteditor";

  //note: the filter must be the same as SiteSettingsDialog::SiteSettingsDialog default file
  QStringList strList =
    KFileDialog::getOpenFileNames(
      location, i18n("*.gpt|G-Portugol Source\n*|All"), this);

  if(strList.count())
  {
    for(QStringList::Iterator it = strList.begin(); it != strList.end(); ++it )
    {
      openFile(*it);
    }
  }
}

void MainWindow::openFile(const KURL& url)
{
  KFileItem file(KFileItem::Unknown, KFileItem::Unknown, url);
  if(file.isReadable())
  {
    if(m_tabEditor->openDocument(url))
    {
      m_actionRecent->addURL(url);
      return;
    }
  }

  m_actionRecent->removeURL(url);
  showSorry(i18n("\"%1\" is unreadable.").arg(url.prettyURL()));
}


void MainWindow::slotFileRecent(const KURL& url)
{
  openFile(url);
}

void MainWindow::slotCloseFile()
{
  m_tabEditor->closeCurrentDocument();
}

void MainWindow::slotSaveCurrentFile() 
{
  saveCurrentFile();
}

void MainWindow::slotSaveCurrentFileAs()
{
  saveCurrentFileAs();
}

bool MainWindow::saveCurrentFile()
{
  if(!m_tabEditor->currentDocumentExistsOnDisk())
  {
    saveCurrentFileAs();
  }
  else if(!m_tabEditor->saveCurrentFile())
  {
    //katepart already show an error message
    //showSorry("Unable to save file");
    return false;
  }
  return true;
}

bool MainWindow::saveCurrentFileAs()
{
  QString location = ":gpteditor";
  KURL url = KFileDialog::getSaveURL(location, QString::null, this);

  if(!url.isEmpty())
  {
    if(m_tabEditor->saveCurrentFileAs(url))
    {
      return true;
    }
  }
  return false;
}

// bool MainWindow::close()
// {
//   m_tabEditor->closeAllDocuments();
//   m_actionRecent->saveEntries(kapp->config());
//   kapp->config()->sync();
// }

void MainWindow::slotClose()
{
  m_actionRecent->saveEntries(kapp->config());
  kapp->config()->sync();
}

void MainWindow::slotCloseAllFiles()
{
  m_tabEditor->closeAllDocuments();
}


void MainWindow::slotQuit()
{
  if(m_tabEditor->closeAllDocuments())
  {
    close();
  }
  kapp->config()->sync();
}

void MainWindow::closeEvent(QCloseEvent * e)
{
  if(m_tabEditor->closeAllDocuments())
  {
    KMainWindow::closeEvent(e);
  }
}

void MainWindow::showError(const QString& msg) const
{
  KMessageBox::error(0, msg);
}

void MainWindow::slotEditKeys()
{
  KKeyDialog dlg;
  dlg.insert(actionCollection());

  if(m_tabEditor->count() != 0)
  {
    KTextEditor::View* view  = m_tabEditor->currentView();
    if(view)
    {
      dlg.insert(view->actionCollection());
    }
  }

  dlg.configure();
}

#include <kdebug.h>
void MainWindow::actionStateChanged(const QString& str)
{
  kdDebug() << "state: " << str << endl;
  stateChanged(str);
}

void MainWindow::slotEditToolbars()
{
  KEditToolbar dlg(guiFactory());
  if (dlg.exec())
  {
    //setupGUI();
    applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
  }
}

void MainWindow::slotParseOK(const KURL&)
{
//   messageListView()->clear();
}

void MainWindow::slotParseError(int /*line*/, const QString& /*msg*/, const KURL& /*url*/)
{
//   messageListView()->set(DebuggerManager::ErrorMsg, msg, line, url);
}

void MainWindow::setEditorStatusMsg(const QString& msg)
{
  m_lbEditorMsg->setText(msg);
}
void MainWindow::setDebugStatusMsg(const QString& msg)
{
  m_lbDebugMsg->setText(msg);
}

void MainWindow::setDebugStatusName(const QString& name)
{
  m_lbDebugName->setText(name);
}

void MainWindow::setLedState(int state)
{
  switch(state)
  {
    case LedOn:
      m_lbLed->setPixmap(QPixmap(UserIcon("indicator_on")));
      break;
    case LedWait:
      m_lbLed->setPixmap(QPixmap(UserIcon("indicator_wait")));
      break;
    case LedOff:
    default:
      m_lbLed->setPixmap(QPixmap(UserIcon("indicator_off")));
  }
}

void MainWindow::slotShowSettings()
{
  ConfigDlg::showDialog();
}


void MainWindow::showSorry(const QString& msg) const
{
  KMessageBox::sorry(0, msg);
}

EditorTabWidget* MainWindow::tabEditor()
{
  return m_tabEditor;
}

DebuggerComboStack* MainWindow::stackCombo()
{
  return m_stackCombo;
}

VariablesListView*  MainWindow::globalVarList()
{
  return m_globaVarList;
}

VariablesListView*  MainWindow::localVarList()
{
  return m_localVarList;
}

KLineEdit* MainWindow::edAddWatch()
{
  return m_edAddWatch;
}

KPushButton* MainWindow::btAddWatch()
{
  return m_btAddWatch;
}

WatchListView*  MainWindow::watchList()
{
  return m_watchList;
}

BreakpointListView* MainWindow::breakpointListView()
{
  return m_breakpointList;
}

MessageListView* MainWindow::messageListView()
{
  return m_messageListView;
}

void MainWindow::activateMessageTab()
{
#warning remover magic number
  m_tabDebug->setCurrentPage(3);
}

#include "mainwindow.moc"
