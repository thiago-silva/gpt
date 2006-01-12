/***************************************************************************
 *   Copyright (C) 2005 by Thiago Silva                                    *
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
#include "ErrorHandler.hpp"


string BasePortugolParser::expecting_algorithm_name = "nome do algoritmo";
string BasePortugolParser::expecting_variable = "uma variável";
string BasePortugolParser::expecting_datatype = "um tipo (inteiro, literal,...)";
string BasePortugolParser::expecting_identifier = "identificador";
string BasePortugolParser::expecting_expression = "expressão";
string BasePortugolParser::expecting_valid_sentence = "sentença válida";
string BasePortugolParser::expecting_attr_op = "operador \":=\"";
string BasePortugolParser::expecting_fimse= "\"fim-se\"";
string BasePortugolParser::expecting_datatype_pl = "um tipo de vetor/matriz (inteiros, literais,...)";
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

int BasePortugolParser::reportMismatchedError(int line, string expecting, string found, string after) {
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
  return ErrorHandler::self()->add(s, line);
       
}

int BasePortugolParser::reportCustomError(int line, string msg) {
  return ErrorHandler::self()->add(msg, line);
}


// void BasePortugolParser::handleVarDeclColonMissing(const MismatchedTokenException& e, const RefToken& id) {
//   reportMismatchedError(e.getLine(), getTokenNames()[e.expecting], getTokenDescription(e.token));
// 
//   //if "a inteiro"
//   //  printTip "colocar ':' entre 'a' e 'inteiro'
//   //if "a b : ...(unknow)"
//   //   printTip "colocar "," entre "a" e "b"
//   //if "a b ...(unknow)" --> ":" nao está a vista!
//   //  synchronizar até T_SEMICOL|T_FIM_VARIAVEIS e
//   //  cair fora! (para que continue para a proxima decl ou T_KW_FIM_VARIAVEIS)
// 
//   if(e.token->getType() == PortugolParserTokenTypes::T_IDENTIFICADOR) {
//     printTip(string("Coloque uma vírgula entre as variáveis \"") 
//              + id->getText() + "\" e \"" + e.token->getText() + "\".", id->getLine());
//   }
// 
//   //ex. "a inteiro"
//   if(isDatatype(LA(1))) {
//     printTip(string("Faltando colocar ':' entre \"") 
//            + id->getText() + "\" e " + getTokenNames()[LA(1)], id->getLine());      
// 
//     //nao consumir. LA(1) eh datatype, continua a producao.
// 
//   //ex: a b : ...(unknow)
//   } else if(LA(2) == PortugolParserTokenTypes::T_COLON) {
// 
//     consume(); //o que quer que o LA(1) seja //(b) no exemplo
//     consume(); //o LA(2) (":")  
// 
//     //continuar a producao (LA(1) agora deve ser um datatype)
// 
//   //ex: a b (unknow)...(LA(1) == b)
//   } else {
//     BitSet b;
//     b.add(PortugolParserTokenTypes::T_COLON);
//     b.add(PortugolParserTokenTypes::T_KW_FIM_VARIAVEIS);
//     consumeUntil(b);
// 
//     //se lookahead == ":", consumir e continuar (LA(1) deve ser datatype)
//     if(LA(1) == PortugolParserTokenTypes::T_COLON) {
//       consume();
//     }
//   }
// }

void BasePortugolParser::printTip(const string& msg, int line, int cd) {
  ErrorHandler::self()->addTip(msg, line, cd);
}
