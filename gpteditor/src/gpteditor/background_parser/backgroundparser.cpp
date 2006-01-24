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

#include "backgroundparser.h"

#include <ktexteditor/editinterface.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>

#include <kdebug.h>

#include <iostream>
#include <sstream>
#include <string>

#include <antlr/AST.hpp>
#include <gpt/PortugolLexer.hpp>
#include <gpt/PortugolParser.hpp>
#include <gpt/SemanticWalker.hpp>
// #include <gpt/SymbolTable.hpp>
#include <gpt/PortugolAST.hpp>
#include <gpt/ErrorHandler.hpp>

#include <qtimer.h>

BackgroundParser::BackgroundParser(Document* doc, QObject *parent, const char *name)
  :  QObject(parent, name), m_ownerDoc(doc), /*m_ied(0),*/ m_timer(0)
{  
  connect(m_ownerDoc->view()->document(), SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
//   m_ied = dynamic_cast<KTextEditor::EditInterface*>(m_ownerDoc->view()->document());

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(parse()));
}

BackgroundParser::~BackgroundParser()
{
}

void BackgroundParser::parse()
{
   KTextEditor::EditInterface *ied = 0;
  ied = 
    dynamic_cast<KTextEditor::EditInterface*>(m_ownerDoc->view()->document());
  if(ied) {
    doParse(ied->text());
  }
}

void BackgroundParser::slotTextChanged()
{
  m_timer->stop();

  emit sigParseOK(m_ownerDoc->url()); //so the editor can clear the error marks 
  
  m_timer->start(500, true);
}

void BackgroundParser::doParse(const QString& text) 
{
  try
  {
    istringstream input(text.latin1());

    PortugolLexer lexer(input);
    PortugolParser parser(lexer);

    ASTFactory ast_factory(PortugolAST::TYPE_NAME,&PortugolAST::factory);
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

//     ErrorHandler::self()->stopOnError(true);

    parser.algoritmo();

    RefPortugolAST tree = parser.getPortugolAST();
    if(!ErrorHandler::self()->hasError() && tree)
    {
      SymbolTable stable;
      SemanticWalker semantic(stable);
      semantic.algoritmo(tree);
    }    

    if(ErrorHandler::self()->hasError()) {
      ErrorHandler::ErrorMsg err = 
        ErrorHandler::self()->getFirstError();
      kdDebug() << "parse error: " << err.line << ": " << err.msg.c_str() << endl;
      emit sigParseError(err.line, QString(err.msg.c_str()), m_ownerDoc->url());        
    } else {
      emit sigParseOK(m_ownerDoc->url());
    }
  }
//   catch(UniqueErrorException& e) 
//   {  
//     emit sigParseError(e.line(), QString(e.message().c_str()), m_ownerDoc->url());
//   }
  catch(ANTLRException& e)
  {
    kdDebug() << "bgparser: erro interno: " << e.toString().c_str() << endl;
  }
  catch(exception& e)
  {
    kdDebug() << "bgparser: erro interno: " << e.what() << endl;
  }
  catch(...)
  {
    kdDebug() << "bgparser: erro interno: " << endl;
  }

  ErrorHandler::self()->clear();
}

#include "backgroundparser.moc"
