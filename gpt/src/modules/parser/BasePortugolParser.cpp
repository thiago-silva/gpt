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


#include "BasePortugolParser.hpp"
#include "PortugolParserTokenTypes.hpp"
#include "GPTDisplay.hpp"


string BasePortugolParser::expecting_algorithm_name = "nome do algoritmo";
string BasePortugolParser::expecting_variable = "uma variável";
string BasePortugolParser::expecting_datatype = "um tipo (inteiro, literal,...)";
string BasePortugolParser::expecting_datatype_pl = "um tipo de vetor/matriz (inteiros, literais,...)";
string BasePortugolParser::expecting_identifier = "identificador";
string BasePortugolParser::expecting_expression = "expressão";
string BasePortugolParser::expecting_valid_sentence = "sentença válida";
string BasePortugolParser::expecting_attr_op = "operador \":=\"";
string BasePortugolParser::expecting_fimse= "\"fim-se\"";
string BasePortugolParser::expecting_fimvar_or_var = "\"fim-variáveis\" ou declaração de variável";
string BasePortugolParser::expecting_stm_or_fim = "\"fim\" ou comando válido";
string BasePortugolParser::expecting_stm_or_fimse = "\"fim-se\" ou comando válido";
string BasePortugolParser::expecting_stm_or_fimenq = "\"fim-enquanto\" ou comando válido";
string BasePortugolParser::expecting_stm_or_fimpara = "\"fim-para\" ou comando válido";
string BasePortugolParser::expecting_eof_or_function = "fim de arquivo (EOF) ou \"função\"";
string BasePortugolParser::expecting_function_name = "nome da função";
string BasePortugolParser::expecting_param_or_fparen = "variável ou \")\"";


BasePortugolParser::BasePortugolParser(const ParserSharedInputState& lexer, int k_)
  : LLkParser(lexer,k)
{
}

BasePortugolParser::BasePortugolParser(TokenBuffer& tokenBuf, int k_)
  : LLkParser(tokenBuf,k)
{
}

BasePortugolParser::BasePortugolParser(TokenStream& lexer, int k_)
  : LLkParser(lexer,k)
{
}

string BasePortugolParser::getAlgName() {
  return _name;
}

void BasePortugolParser::match(int t) {
  lastToken = LT(1);
  LLkParser::match(t);
}

void BasePortugolParser::matchNot(int t) {
  lastToken = LT(1);
  LLkParser::match(t);}

void BasePortugolParser::match(const BitSet& b) {
  lastToken = LT(1);
  LLkParser::match(b);
}


string BasePortugolParser::getTokenDescription(const RefToken& token) {
  string str;

  if(token->getType() == PortugolParserTokenTypes::T_IDENTIFICADOR) {
    str = "\"";
    str += token->getText();
    str += "\"";
  } else if(isKeyword(token->getType())) {
    str = "a palavra-chave ";
    str += getTokenNames()[token->getType()];
  } else if(token->getType() == PortugolParserTokenTypes::EOF_) {
    str = "fim de arquivo (EOF)";
  } else {
//     str = getTokenNames()[token->getType()];
    str = token->getText();
  }
  return str;
}

int BasePortugolParser::reportParserError(int line, string expecting, string found, string after) {
  string str;
  if(found.length()) {
    str = ", encontrado ";
    str += found;
  }
  if(after.length()) {
    str += " após \"";
    str += after;
    str += "\"";
  }


  stringstream s;
  s << "Esperando " << expecting << str;
  return GPTDisplay::self()->add(s.str(), line);

}

void BasePortugolParser::printTip(const string& msg, int line, int cd) {
  GPTDisplay::self()->addTip(msg, line, cd);
}
