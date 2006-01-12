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


#ifndef BASEPORTUGOLPARSER_H
#define BASEPORTUGOLPARSER_H

#include <antlr/config.hpp>
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include <antlr/NoViableAltException.hpp>
#include <antlr/LLkParser.hpp>

#include "PortugolParserTokenTypes.hpp"

// #include "SymbolTable.hpp"

  using namespace std;
  using namespace antlr;

class BasePortugolParser : public LLkParser
{

public:
  BasePortugolParser(const ParserSharedInputState& lexer, int k_);
  BasePortugolParser(TokenBuffer& tokenBuf, int k_);
  BasePortugolParser(TokenStream& lexer, int k_);
  
  virtual void match(int t);
  virtual void matchNot(int t);
  virtual void match(const BitSet& b);

  string getAlgName();

protected:
  static string expecting_algorithm_name;
  static string expecting_variable;
  static string expecting_datatype;
  static string expecting_identifier;
  static string expecting_expression;
  static string expecting_valid_sentence;  
  static string expecting_attr_op;
  static string expecting_fimse;
  static string expecting_datatype_pl;
  static string expecting_fimvar_or_var;
  static string expecting_stm_or_fim;
  static string expecting_stm_or_fimse;
  static string expecting_stm_or_fimenq;
  static string expecting_stm_or_fimpara;
  static string expecting_eof_or_function;
  static string expecting_function_name;
  static string expecting_param_or_fparen;
  static string expecting_inicio_or_vardecl;

  RefToken lastToken;

 bool isDatatype(int tk) {
    return (tk == PortugolParserTokenTypes::T_KW_INTEIRO)
           || (tk == PortugolParserTokenTypes::T_KW_INTEIROS)
           || (tk == PortugolParserTokenTypes::T_KW_REAL)
           || (tk == PortugolParserTokenTypes::T_KW_REAIS)
           || (tk == PortugolParserTokenTypes::T_KW_CARACTERE)
           || (tk == PortugolParserTokenTypes::T_KW_CARACTERES)
           || (tk == PortugolParserTokenTypes::T_KW_LITERAL)
           || (tk == PortugolParserTokenTypes::T_KW_LITERAIS)
           || (tk == PortugolParserTokenTypes::T_KW_LOGICO)
           || (tk == PortugolParserTokenTypes::T_KW_LOGICOS);
  }

  bool isKeyword(int tk) {
    return (tk == PortugolParserTokenTypes::T_KW_FIM_VARIAVEIS) 
           || (tk == PortugolParserTokenTypes::T_KW_ALGORITMO)
           || (tk == PortugolParserTokenTypes::T_KW_VARIAVEIS)
           || (tk == PortugolParserTokenTypes::T_KW_INTEIRO)
           || (tk == PortugolParserTokenTypes::T_KW_REAL)
           || (tk == PortugolParserTokenTypes::T_KW_CARACTERE)
           || (tk == PortugolParserTokenTypes::T_KW_LITERAL)
           || (tk == PortugolParserTokenTypes::T_KW_LOGICO)
           || (tk == PortugolParserTokenTypes::T_KW_INICIO)
           || (tk == PortugolParserTokenTypes::T_KW_VERDADEIRO)
           || (tk == PortugolParserTokenTypes::T_KW_FALSO)
           || (tk == PortugolParserTokenTypes::T_KW_FIM)
           || (tk == PortugolParserTokenTypes::T_KW_OU)
           || (tk == PortugolParserTokenTypes::T_KW_E)
           || (tk == PortugolParserTokenTypes::T_KW_NOT)
           || (tk == PortugolParserTokenTypes::T_KW_SE)
           || (tk == PortugolParserTokenTypes::T_KW_SENAO)
           || (tk == PortugolParserTokenTypes::T_KW_ENTAO)
           || (tk == PortugolParserTokenTypes::T_KW_FIM_SE)
           || (tk == PortugolParserTokenTypes::T_KW_ENQUANTO)
           || (tk == PortugolParserTokenTypes::T_KW_FACA)
           || (tk == PortugolParserTokenTypes::T_KW_FIM_ENQUANTO)
           || (tk == PortugolParserTokenTypes::T_KW_PARA)
           || (tk == PortugolParserTokenTypes::T_KW_DE)
           || (tk == PortugolParserTokenTypes::T_KW_ATE)
           || (tk == PortugolParserTokenTypes::T_KW_PASSO)
           || (tk == PortugolParserTokenTypes::T_KW_FIM_PARA)
           || (tk == PortugolParserTokenTypes::T_KW_FUNCAO)
           || (tk == PortugolParserTokenTypes::T_KW_RETORNE)
//            || (tk == PortugolParserTokenTypes::T_KW_CONJUNTO)
           || (tk == PortugolParserTokenTypes::T_KW_MATRIZ)           
           || (tk == PortugolParserTokenTypes::T_KW_INTEIROS)
           || (tk == PortugolParserTokenTypes::T_KW_REAIS)
           || (tk == PortugolParserTokenTypes::T_KW_CARACTERES)
           || (tk == PortugolParserTokenTypes::T_KW_LITERAIS)           
           || (tk == PortugolParserTokenTypes::T_KW_LOGICOS); 
  }

  string getTokenDescription(const RefToken& t);

  int reportMismatchedError(int line, string expecting, string found = "", string after = "");
  int reportCustomError(int line, string msg);  


  void handleVarDeclColonMissing(const MismatchedTokenException& e, const RefToken& id);

//   void handleVarDecl(const RefToken& id);
//   void checkVarDecl(const RefToken& id);

  void printTip(const string& msg, int line, int cd);

//   SymbolTable symbtable;
  string currentScope; //global,f1(),...

  string _name;
};

#endif
