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


#include "PortugolAST.hpp"
#include <sstream>

const char* const PortugolAST::TYPE_NAME = "PortugolAST";

PortugolAST::PortugolAST()
    : CommonAST(), line(-1) {
}

PortugolAST::PortugolAST(RefToken t)
    : CommonAST(t), line(t->getLine()) {
}

PortugolAST::PortugolAST( const CommonAST& other )
    : CommonAST(other), line(-1) {
}

PortugolAST::PortugolAST( const PortugolAST& other )
    : CommonAST(other), line(other.line) {
}

PortugolAST::~PortugolAST() {
}

RefAST PortugolAST::clone( void ) const
{
  PortugolAST *ast = new PortugolAST( *this );
  return RefAST(ast);
}

const char* PortugolAST::typeName( void ) const
{
  return PortugolAST::TYPE_NAME;
}

void PortugolAST::initialize(RefToken t)
{
  CommonAST::initialize(t);
  setLine(t->getLine());
}

void PortugolAST::setLine(int line_) {
  line = line_;
}

int PortugolAST::getLine() const {
  return line;
}

// std::string PortugolAST::toString() const {
//   std::stringstream s;
//   s << "(" << getLine() << ")" << getText();
//   return s.str();
// }

RefAST PortugolAST::factory() {
  return RefAST(new PortugolAST);
}
