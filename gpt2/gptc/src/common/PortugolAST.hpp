/***************************************************************************
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
 *   tsilva@sourcecraft.info                                               *
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
#include <string>

using namespace std;
using namespace antlr;

class Type;

class PortugolAST : public CommonAST {
public:
  PortugolAST();
  PortugolAST( RefToken t );
  PortugolAST( const CommonAST& other );
  PortugolAST( const PortugolAST& other );

  ~PortugolAST();

  void setLine(int line);
  int getLine() const;

  void setColumn(int);
  int getColumn() const;

  void setEvalType(Type*);
  Type* getEvalType();

  virtual RefAST clone( void ) const;

  virtual void initialize( RefToken t );

  virtual const char* typeName( void ) const;

  virtual std::string toString() const;

  static RefAST factory();
  static const char* const TYPE_NAME;
protected:
  int   line;
  int   column;
  Type* type;
  int endLine;
  int eval_type; //evaluated type of expression
  string filename;
};

typedef ASTRefCount<PortugolAST> RefPortugolAST;

#endif
