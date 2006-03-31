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

#include "editortabwidget.h"
#include "mainwindow.h"
#include "document.h"

#include <kparts/partmanager.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/cursorinterface.h>
#include <ktexteditor/view.h>
#include <ktexteditor/viewcursorinterface.h>

#include <kactioncollection.h>
#include <kiconloader.h>
#include <kmimetype.h>
#include <kpopupmenu.h>
#include <qdir.h>
#include <kfileitem.h>
#include <kurldrag.h>
#include <qdragobject.h> 
#include <klocale.h>
#include <kiconloader.h>
#include <kdebug.h>

EditorTabWidget::EditorTabWidget(QWidget* parent, MainWindow *window, const char *name)
    : KTabWidget(parent, name), m_terminating(false),
    m_window(window), m_currentView(0)
{
  setAcceptDrops(TRUE);
  
  connect(this, SIGNAL(currentChanged(QWidget*)), this, SLOT(slotCurrentChanged(QWidget*)));

  //reordering with middle button: need extra coding to work (it alters the index of the tabs
  //wich should be in sync with the list)
  //   setTabReorderingEnabled(true);
  setHoverCloseButton (true);
  setHoverCloseButtonDelayed (true);
}

EditorTabWidget::~EditorTabWidget()
{
  terminate();
}

void EditorTabWidget::terminate()
{
  m_terminating = true;
  closeAllDocuments();
}

void EditorTabWidget::openNew() {
  createDocument();
}

bool EditorTabWidget::openDocument(const KURL& url)
{
  int index;
  if((index = documentIndex(url.path())) != -1)
  {
    //file already opened, show it
    this->setCurrentPage(index);
    return true;
  }
  else
  {
    if(createDocument(url))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool EditorTabWidget::closeDocument(int index)
{
  if((index < 0) || (count() == 0))
  {
    return false;
  }

  Document* doc = document(index);

  if(!doc->close()) return false; //file is modifiyed and the user hit "cancel" on prompt to save

  if(!m_terminating)
  {
    m_window->guiFactory()->removeClient(doc->view());
    if(index == currentPageIndex())
    {
      m_currentView = 0L;
    }
  }

  QWidget* w = page(index);
  removePage(w);

  m_docList.remove(doc);
  delete doc;

  if((count() == 0) && !m_terminating)
  {
    emit sigNoDocument();
    m_window->actionStateChanged("has_nofileopened");
    m_window->setCaption(QString::null);
    m_window->setEditorStatusMsg(QString::null);
  }

  return true;
}

bool EditorTabWidget::closeCurrentDocument()
{
  return closeDocument(currentPageIndex());
}

bool EditorTabWidget::closeAllDocuments()
{
  while(count())
  {
    //stop closing documents if the user hit "cancel" on save prompt
    if(!closeCurrentDocument()) return false;
  }

  return true;
}

void EditorTabWidget::setCurrentDocumentTab(const KURL& url, bool forceOpen)
{
  int index = documentIndex(url);

  /* currentPageIndex() == index:
      setCurrentDocumentTab() eh ativado quando se inicia debugging!
      setCurrentPage() emits currentChanged() -> slotCurrentChanged().
      Em slotCurrentChanged(), action state muda para has_fileopen
      e isso ativa botoes como "debug" e "run".
      De qualquer forma, evita resetar o currentPage
  */
  if(currentPageIndex() == index) return;

  if(index != -1 ) 
  {
    setCurrentPage(index);
  }
  else if(forceOpen)
  {
    createDocument(KURL(url));
  }
}

bool EditorTabWidget::saveCurrentFile()
{
  if(count() == 0) return false;

  return currentDocument()->save();
}

bool EditorTabWidget::saveCurrentFileAs(const KURL & url)
{
  if(count() == 0) return false;

  bool ret = currentDocument()->saveAs(url);
  if(ret) {
    slotDocumentSaved();
  }

  return ret;
}

void EditorTabWidget::gotoLineAtFile(const KURL& url, int line)
{
  setCurrentDocumentTab(url, true);

  if(currentPageIndex() == -1) return;

  currentDocument()->gotoLine(line);
}

KURL EditorTabWidget::documentURL(int index)
{
  int size = m_docList.count();
  if((index >= 0) && (index < size))
  {
    return document(index)->url();
  }
  else
  {
    return KURL();
  }
}

KURL EditorTabWidget::currentDocumentURL()
{
  return documentURL(currentPageIndex());
}
bool EditorTabWidget::currentDocumentExistsOnDisk()
{
  return currentDocument()->existsOnDisk();
}

int EditorTabWidget::currentDocumentLine()
{
  if(currentPageIndex() == -1) return 0;

  return currentDocument()->currentLine();
}

void EditorTabWidget::markActiveBreakpoint(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->markActiveBreakpoint(line);
  }
}

void EditorTabWidget::unmarkActiveBreakpoint(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->unmarkActiveBreakpoint(line);
  }
}

void EditorTabWidget::markDisabledBreakpoint(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->markDisabledBreakpoint(line);
  }
}

void EditorTabWidget::unmarkDisabledBreakpoint(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->unmarkDisabledBreakpoint(line);
  }
}

void EditorTabWidget::markExecutionPoint(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->markExecutionPoint(line);
  }
}

void EditorTabWidget::unmarkExecutionPoint(const KURL& url)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->unmarkExecutionPoint();
  }
}

void EditorTabWidget::markPreExecutionPoint(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->markPreExecutionPoint(line);
  }
}

void EditorTabWidget::unmarkPreExecutionPoint(const KURL& url)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->unmarkPreExecutionPoint();
  }
}

void EditorTabWidget::markError(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    document(url)->markError(line);
  }
}

void EditorTabWidget::unmarkError(const KURL& url, int line)
{  
  Document* doc = document(url);
  if(doc)
  {
    document(url)->unmarkError(line);
  }
}


bool EditorTabWidget::hasBreakpointAt(const KURL& url, int line)
{
  Document* doc = document(url);
  if(doc)
  {
    return document(url)->hasBreakpointAt(line);
  }
  else
  {
    return false;
  }
}

void EditorTabWidget::createDocument() {
  QString home = QDir::homeDirPath();
  KURL url(i18n("%1/untitled_%2.gpt").arg(home, QString::number(count()+1)));
  Document* doc = new Document(this, url);

  initDoc(doc);
  doc->setDefaultHighlight();

//   cursorInterface(doc->view()->document())->createCursor()->insertText(
//     "/* Descrição do algoritmo */\n\n"
//     "algoritmo nome_do_algoritmo;\n\n"
//     "início\n\n"
//     "fim\n"
//   );  
//   viewCursorInterface(doc->view())->setCursorPosition(5,0);

  doc->view()->setFocus();

}

bool EditorTabWidget::createDocument(const KURL& url) {
  Document* doc = new Document(this);

  if(!doc->open(url))
  {
    delete doc;
    return false;
  }
  initDoc(doc);
  return true;
}

void EditorTabWidget::initDoc(Document* doc)
{
  connect(doc, SIGNAL(sigBreakpointMarked(Document*, int, bool )), this,
          SLOT(slotBreakpointMarked(Document*, int, bool)));

  connect(doc, SIGNAL(sigBreakpointUnmarked(Document*, int )), this,
          SLOT(slotBreakpointUnmarked(Document*, int)));

  connect(doc, SIGNAL(sigTextChanged()), this,
          SLOT(slotTextChanged()));

  connect(doc, SIGNAL(sigStatusMsg(const QString&)), this,
          SLOT(slotStatusMsg(const QString&)));

  connect(doc, SIGNAL(sigDocumentSaved()), this, 
          SLOT(slotDocumentSaved()));  

  connect(doc->view(), SIGNAL(dropEventPass(QDropEvent*)),
    this, SLOT(slotDropEvent(QDropEvent*)));

  connect(doc, SIGNAL(sigParseError(int, const QString&, const KURL&)),
    this, SLOT(slotParseError(int, const QString&, const KURL&)));

  connect(doc, SIGNAL(sigParseOK(const KURL&)), 
    this, SLOT(slotParseOK( const KURL& )));

  QIconSet mimeIcon(KMimeType::pixmapForURL(doc->url(), 0, KIcon::Small));
  if (mimeIcon.isNull())
  {
    mimeIcon = QIconSet(SmallIcon("document"));
  }

  addTab(doc->tab(), mimeIcon, doc->url().fileName());
  setTabToolTip(doc->tab(), doc->url().prettyURL());

  KTextEditor::PopupMenuInterface* popupIf = dynamic_cast<KTextEditor::PopupMenuInterface*>(doc->view());
  if (popupIf)
  {
    QPopupMenu *popup = (QPopupMenu*) m_window->factory()->container("ktexteditor_popup", m_window);
    popupIf->installPopup(popup);
    connect(popup, SIGNAL(aboutToShow()), this, SLOT(slotMenuAboutToShow()));
  }

  m_docList.append(doc); 

  setCurrentPage(count()-1);
  emit sigNewDocument();

  doc->parse();
}

int EditorTabWidget::documentIndex(const KURL& url)
{
  int i = 0;

  QValueList<Document*>::iterator it;
  for (it = m_docList.begin(); it != m_docList.end(); ++it, i++ )
  {
    if((*it)->url() == url)
    {
      return i;
    }
  }
  return -1;
}

void EditorTabWidget::closeRequest(int index)
{
  closeDocument(index);
}

void EditorTabWidget::slotCurrentChanged(QWidget*)
{
  if(m_terminating) return;

  m_window->setCaption(currentDocumentURL().prettyURL());

  if(m_currentView)
  {
    m_window->guiFactory()->removeClient(m_currentView);
  }

  if(count())
  {
    m_window->actionStateChanged("has_fileopened");

    m_currentView = document(currentPageIndex())->view();
    m_window->guiFactory()->addClient(m_currentView);
  }
}

void EditorTabWidget::slotBreakpointMarked(Document* doc, int line, bool enabled)
{
  emit sigBreakpointMarked(doc->url().prettyURL(), line, enabled);
}

void EditorTabWidget::slotBreakpointUnmarked(Document* doc, int line)
{
  emit sigBreakpointUnmarked(doc->url().prettyURL(), line);
}

void EditorTabWidget::slotTextChanged()
{
  if(currentPageIndex() == -1) return;

  int index = currentPageIndex();
  Document* doc = document(index);

  if(doc->isModified())
  {
    setTabIconSet(page(index), QIconSet(SmallIcon("filesave")));
  }
  else
  {
    QIconSet mimeIcon(KMimeType::pixmapForURL(doc->url().prettyURL(), 0, KIcon::Small));
    if (mimeIcon.isNull())
    {
      mimeIcon = QIconSet(SmallIcon("document"));
    }
    setTabIconSet(page(index), mimeIcon);
  }
}

void EditorTabWidget::slotStatusMsg(const QString& msg)
{
  m_window->setEditorStatusMsg(msg);
}

void EditorTabWidget::slotDocumentSaved()
{
  if(!currentDocument()->isModified())
  {
    QIconSet mimeIcon(KMimeType::pixmapForURL(currentDocumentURL(), 0, KIcon::Small));
    if (mimeIcon.isNull())
    {
      mimeIcon = QIconSet(SmallIcon("document"));
    }
    setTabLabel(currentDocument()->tab(), currentDocumentURL().fileName());
    setTabIconSet(currentDocument()->tab(), mimeIcon);
    setTabToolTip(currentDocument()->tab(), currentDocumentURL().prettyURL());  
  }
}

void EditorTabWidget::contextMenu(int index, const QPoint & p)
{
  enum { Close, CloseOthers, CloseAll };

  KPopupMenu* menu = new KPopupMenu(this);
  menu->insertItem(SmallIcon("fileclose"), i18n("Close"), Close);
  menu->insertItem(i18n("Close Other Tabs"), CloseOthers);
  menu->insertItem(i18n("Close All"), CloseAll);

  Document* doc = document(index);
  int idx = 0;
  switch(menu->exec(p))
  {
    case Close:
      closeDocument(index);
      break;
    case CloseOthers:
      while(count() != 1)
      {
        if(document(idx) != doc)
        {
          //stop closing documents if the user hits "cancel" on save dialog
          if(!closeDocument(idx)) break;
        }
        else
        {
          idx++;
        }
      }
      break;
    case CloseAll:
      closeAllDocuments();
      break;
  }

  delete menu;
}

KTextEditor::View* EditorTabWidget::currentView()
{
  if(count() == 0) return 0;
  return m_currentView;
}

Document* EditorTabWidget::document(uint index)
{
  if(index > (m_docList.count()-1))
  {
    return 0L;
  }
  else
  {
    return *(m_docList.at(index));
  }
}

Document* EditorTabWidget::document(const KURL& url)
{
  QValueList<Document*>::iterator it;
  for(it = m_docList.begin(); it != m_docList.end(); ++it)
  {

    if((*it)->url() == url)
    {
      return (*it);
    }
  }

  return 0L;
}

Document* EditorTabWidget::currentDocument()
{
  return document(currentPageIndex());
}

void EditorTabWidget::slotMenuAboutToShow()
{
//   KAction* ac = m_window->actionCollection()->action("editor_add_watch");
//   if(ac)
//   {
//     QString watch = currentDocument()->wordUnderCursor();
//     if(watch.isEmpty())
//     {
//       ac->setText(QString("Add Watch"));
//       ac->setEnabled(false);
//     }
//     else
//     {
//       ac->setEnabled(true);
//       ac->setText(QString("Add Watch: ") + watch);
//     }
//   }
}

void EditorTabWidget::slotAddWatch()
{
//   emit sigAddWatch(currentDocument()->wordUnderCursor());
}

void EditorTabWidget::dragEnterEvent(QDragEnterEvent* e)
{
  if(KURLDrag::canDecode(e)) {
    e->accept(rect());
  }
}

void EditorTabWidget::dragMoveEvent( QDragMoveEvent *e )
{
   e->accept();
}
    
void EditorTabWidget::dropEvent(QDropEvent* e)
{
  slotDropEvent(e);
}

void EditorTabWidget::slotDropEvent(QDropEvent* e) {
  KURL::List list;
  if(KURLDrag::decode(e, list)) {
    int size  = list.size();
    for(int i = 0; i < size; i++)
    {
      m_window->openFile(list[i]);
    }
  }  
}

void EditorTabWidget::slotParseOK(const KURL& url)
{
  Document* d = document(url);
  if(d)
  {
    d->clearErrorMarks();
  }

  QIconSet mimeIcon(KMimeType::pixmapForURL(currentDocumentURL(), 0, KIcon::Small));
  if (mimeIcon.isNull())
  {
    mimeIcon = QIconSet(SmallIcon("document"));
  }
  setTabIconSet(document(url)->tab(), mimeIcon);


  emit sigParseOK(url);  
}

void EditorTabWidget::slotParseError(int line, const QString& msg, const KURL& url)
{
  KIconLoader *loader = KGlobal::iconLoader();

  QIconSet mimeIcon(loader->loadIcon("cancel", KIcon::Small));
  if(!mimeIcon.isNull())
  {
    setTabIconSet(document(url)->tab(), mimeIcon);
  }

  Document* d = document(url);
  if(d)
  {
    d->clearErrorMarks();
    markError(url, line);
  }
  emit sigParseError(line, msg, url);
}

#include "editortabwidget.moc"
