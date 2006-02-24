/***************************************************************************
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
 *   thiago.silva@kdemal.net                                               *
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

#ifndef PORTUGOLAST_HPP
#define PORTUGOLAST_HPP

#include <antlr/CommonAST.hpp>

using namespace antlr;

class PortugolAST : public CommonAST {
public:
  PortugolAST();
  PortugolAST( RefToken t );
  PortugolAST( const CommonAST& other );
  PortugolAST( const PortugolAST& other );

  ~PortugolAST();

  void setLine(int line);
  int getLine();

  void setEndLine(int endLine);
  int getEndLine();

  void setEvalType(int type) {
    eval_type = type;
  }
  int getEvalType() {
    return eval_type;
  }

  virtual RefAST clone( void ) const;

  virtual void initialize( RefToken t );
 
  virtual const char* typeName( void ) const;

  static RefAST factory();
  static const char* const TYPE_NAME;
protected:
  int line;
  int endLine;
  int eval_type; //evaluated type of expression
};

typedef ASTRefCount<PortugolAST> RefPortugolAST;

#endif
