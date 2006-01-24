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

#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

// #include <kmainwindow.h>
#include <kparts/mainwindow.h>
#include <qstring.h>

class EditorTabWidget;
class DebuggerComboStack;
class VariablesListView;
class KLineEdit;
class KPushButton;
class WatchListView;
class BreakpointListView;
class MessageListView;
class KTextEdit;
class KPushButton;
class KStatusBar;
class DebuggerManager;
class KRecentFilesAction;
class KURL;
class KSelectAction;
class KDialogBase;
class QLabel;
class QTabWidget;

/*
 
namespace KTextEditor {
  class EditInterface;
}
*/

class MainWindow : public KParts::MainWindow
{
  Q_OBJECT

public:
  enum { LedOn, LedOff, LedWait };
  
  MainWindow(QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel);
  ~MainWindow();

  EditorTabWidget*    tabEditor();
  DebuggerComboStack* stackCombo();
  VariablesListView*  globalVarList();
  VariablesListView*  localVarList();

  KLineEdit*          edAddWatch();
  KPushButton*        btAddWatch();
  WatchListView*      watchList();
  BreakpointListView* breakpointListView();
  MessageListView*    messageListView();
//   KTextEdit*          edOutput();
  //KTextEditor::EditInterface*  edOutput();

  void showError(const QString&) const;
  void showSorry(const QString&) const;

  void openFile();
  void openFile(const KURL& url);
  void actionStateChanged(const QString&);

  void setEditorStatusMsg(const QString&);
  void setDebugStatusMsg(const QString&);
  void setDebugStatusName(const QString&);
  void setLedState(int);


  bool saveCurrentFileAs();
  bool saveCurrentFile();  
  
  void activateMessageTab();
private slots:
  void slotSaveCurrentFile();
  void slotSaveCurrentFileAs();
  void slotNewFile();
  void slotOpenFile();
  void slotCloseFile();
  void slotCloseAllFiles();  
  void slotQuit();
  //void slotShowSettings();
  void slotEditKeys();
  void slotEditToolbars();
  
  void slotParseError(int, const QString&, const KURL&);
  void slotParseOK(const KURL&);

  /*
  void slotHasNoFiles();
  void slotHasFiles();

  void slotHasNoUndo();
  void slotHasUndo();
  void slotHasNoRedo();
  void slotHasRedo();
  */

  void slotShowSettings();
  void slotFileRecent(const KURL&);

  void slotClose();

protected:
  virtual void closeEvent(QCloseEvent * e);

private:
  void setupStatusBar();
  void setupActions();
  void createWidgets();

  QLabel *m_lbLed;
  QLabel *m_lbDebugMsg;
  QLabel *m_lbDebugName;
  QLabel *m_lbEditorMsg;
  EditorTabWidget* m_tabEditor;
  DebuggerComboStack* m_stackCombo;
  VariablesListView* m_globaVarList;
  VariablesListView* m_localVarList;
  KLineEdit* m_edAddWatch;
  KPushButton* m_btAddWatch;
  WatchListView* m_watchList;
  BreakpointListView* m_breakpointList;
  MessageListView* m_messageListView;
  QTabWidget* m_tabDebug;
//   KTextEdit* m_edOutput;
  //KTextEditor::EditInterface *m_edOutput;

  KSelectAction      *m_siteAction;
//   KSelectAction      *m_defaultScriptAction;
  KStatusBar         *m_statusBar;
  KRecentFilesAction *m_actionRecent;

  DebuggerManager* m_debugger_manager;
};

#endif
