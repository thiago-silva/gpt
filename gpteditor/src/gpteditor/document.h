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
 
#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <qobject.h>
#include <qvaluelist.h>
#include <qstring.h>
#include <kurl.h>

#include <ktexteditor/markinterface.h>

class QWidget;
class BackgroundParser;

namespace KTextEditor
{
  class View;
}

class Document : public QObject
{
  Q_OBJECT
public:
  Document(QWidget *parent, const KURL& = KURL(), const char *name = 0);

  ~Document();

  KTextEditor::View* view();
  KURL url();
  QWidget* tab();
  
  bool save();
  bool saveAs(const KURL&);
  bool open(const KURL& url);
  bool close();

  bool existsOnDisk();
  bool isModified();
  
  void gotoLine(int line);
  int currentLine();
  void markActiveBreakpoint(int line);
  void unmarkActiveBreakpoint(int line);
  void markDisabledBreakpoint(int line);
  void unmarkDisabledBreakpoint(int line);
  void markExecutionPoint(int line);
  void unmarkExecutionPoint();
  void markPreExecutionPoint(int line);
  void unmarkPreExecutionPoint();
  bool hasBreakpointAt(int line);

  void markError(int);
  void unmarkError(int);

  void clearErrorMarks();

  QString wordUnderCursor();

  void setDefaultHighlight();

  void emitParseError(int line, const QString& msg, const KURL& url);

  void parse();
signals:
  void sigBreakpointUnmarked(Document*, int);
  void sigBreakpointMarked(Document*, int, bool enabled);
  void sigStatusMsg(const QString&);
  void sigTextChanged();
  
  void sigDocumentSaved();

  void sigParseError(int, const QString&, const KURL&);
  void sigParseOK(const KURL&);
private slots:
  void slotMarkChanged();  

  void slotSettingsChanged();

//   void testeParseError(int, const QString&, const KURL&);
//   void slotNeedTextHint(int, int, QString&);
private:
  void init();
  void setupMarks();
  void addMark(int line, /*KTextEditor::MarkInterface::MarkTypes type*/ uint type);
  void removeMark(int line, /*KTextEditor::MarkInterface::MarkTypes type*/ uint type);

  bool m_terminating;
  
  //so external marks (from mark*/unmark* funcs) don't emit signals
  bool m_userMarkedGuard;

//   bool m_processingMark;
  
  QWidget* m_tab;
  KTextEditor::View* m_view;
  //we need this if the user edit the file when debugging
  //Since debuggermanager would try to unmark the execmark that
  //is no longer there (kate takes care of changing the line
  //of the mark when edited) causing to have more than 2 execMarks and, after the debug ends
  //leaving a exec mark showing
  int m_execLine;
  int m_preExecLine;  

  BackgroundParser* m_bgParser;
  KURL m_url;

  QValueList<KTextEditor::Mark> m_breakpointMarks;
};

#endif
