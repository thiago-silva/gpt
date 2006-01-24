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

#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H

#include <ktabwidget.h>
#include <qvaluelist.h>
#include <ktexteditor/markinterface.h>

class MainWindow;
class KURL;
class Document;

namespace KTextEditor
{
  class View;
}

namespace KParts
{
  class Part;
}

class EditorTabWidget : public KTabWidget
{
  Q_OBJECT
public:
  EditorTabWidget(QWidget* parent, MainWindow *window, const char *name = 0);
  ~EditorTabWidget();

  //void setMainWindow(MainWindow*);

  void openNew();
  bool openDocument(const KURL& url);
  bool closeCurrentDocument();
  bool closeAllDocuments();
  void setCurrentDocumentTab(const KURL&, bool forceOpen = false);

  bool saveCurrentFile();
  bool saveCurrentFileAs(const KURL & url);

  void gotoLineAtFile(const KURL& url, int line);

  KURL documentURL(int index);
  KURL currentDocumentURL();
  bool currentDocumentExistsOnDisk();
  int  currentDocumentLine();

  void markActiveBreakpoint(const KURL&, int);
  void unmarkActiveBreakpoint(const KURL&, int);
  void markDisabledBreakpoint(const KURL&, int);
  void unmarkDisabledBreakpoint(const KURL&, int);
  void markExecutionPoint(const KURL&, int);
  void unmarkExecutionPoint(const KURL&);
  void markPreExecutionPoint(const KURL&, int);
  void unmarkPreExecutionPoint(const KURL&);

  void markError(const KURL&, int);
  void unmarkError(const KURL&, int);

  bool hasBreakpointAt(const KURL& , int);

  KTextEditor::View* currentView();

  void terminate();

signals:
  void sigBreakpointMarked(const KURL&, int, bool);
  void sigBreakpointUnmarked(const KURL&, int);
  void sigNewDocument();
  void sigNoDocument();
  void sigAddWatch(const QString&);

  void sigParseError(int, const QString&, const KURL&);
  void sigParseOK(const KURL&);
public slots:
  void slotAddWatch();

protected slots:
  virtual void   closeRequest(int);
  virtual void   contextMenu(int, const QPoint &);

private slots:
  void slotCurrentChanged(QWidget*);
  void slotBreakpointMarked(Document* doc, int line, bool enabled);
  void slotBreakpointUnmarked(Document* doc, int line);

  void slotTextChanged();
  void slotStatusMsg(const QString&);

  void slotMenuAboutToShow();
  
  void slotDropEvent(QDropEvent*);

  void slotDocumentSaved();

  void slotParseError(int, const QString&, const KURL&);
  void slotParseOK(const KURL&);
  
protected:
  void dragEnterEvent(QDragEnterEvent*);
  void dragMoveEvent( QDragMoveEvent *);
  void dropEvent(QDropEvent*);

private:
  void                        initDoc(Document* doc);
  bool                        closeDocument(int);
  Document*                   document(uint);
  Document*                   document(const KURL&);
  Document*                   currentDocument();

  void                        createDocument();
  bool                        createDocument(const KURL& url);
  void                        setupMarks(KTextEditor::View* view);

  int                         documentIndex(const KURL& url);
  //   void                        dispatchMark(KTextEditor::Mark& mark, bool adding);
  //   void                        loadMarks(Document_t&, KTextEditor::Document*);


  bool m_terminating;
  QValueList<Document*> m_docList;


  //bool m_markGuard;
  //we don't want sigBreakpointUnmarked() to be emited when the document close.
  //bool m_closeGuard;

  MainWindow* m_window;

  KTextEditor::View* m_currentView;
  //   KParts::PartManager* m_partManager;
};

#endif
