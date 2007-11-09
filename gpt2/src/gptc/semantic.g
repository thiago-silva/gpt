/*
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             */


header {
  #include "BaseSemanticWalker.hpp"
}


options {
  language="Cpp";
}

class SemanticWalker extends TreeParser("BaseSemanticWalker");
options {
  importVocab    = Portugol;
  noConstructors = true;
  genHashLines   = false;
}

{
public:
  SemanticWalker::SemanticWalker()
	 : BaseSemanticWalker() { }
}


programa
  : (importacao)*

    (   declaracao_variaveis
//       | declaracao_constantes
//       | bloco_declaracao_estrutura
    )*
  ;

importacao
  : #(T_USE T_TEXTO_LITERAL)
    //TODO: carregar a symboltable das bibliotecas especificadas
  ;

declaracao_variaveis
  : #(T_VARIAVEIS (declaracao_variavel)+)
  | declaracao_variavel
  ;

declaracao_variavel
  : #(T_VARIAVEL tipo lista_identificadores (lista_inicializacao)?)
  ;

lista_identificadores
  : (T_IDENTIFICADOR)+
  ;

tipo
  : tipo_primitivo
  | tipo_matriz
  | T_IDENTIFICADOR
  ;

lista_inicializacao
  : #(T_VALOR valor_inicializacao)
  ;

valor_inicializacao
  : (matriz_literal | estrutura_literal | expressao)
  ;

matriz_literal
  : #(T_VAL_MATRIZ (valor_inicializacao)+)
  ;

estrutura_literal
  : #(T_VAL_ESTRUTURA (T_IDENTIFICADOR valor_inicializacao)+)
  ;

tipo_primitivo
  : T_INTEIRO
  | T_REAL
  | T_CARACTERE
  | T_LITERAL
  | T_LOGICO
  | T_CORINGA
  ;

tipo_matriz
  : #(T_MATRIZ tipo_da_matriz (T_INTEIRO_LITERAL|T_NULO)+)
  ;

tipo_da_matriz
  : tipo_primitivo | T_IDENTIFICADOR
  ;


expressao
  : expr_elemento
  ;

expr_elemento
  : literal
  ;

literal
  : T_TEXTO_LITERAL
  | T_INTEIRO_LITERAL
  | T_REAL_LITERAL
  | T_CARACTERE_LITERAL
  | T_VERDADEIRO
  | T_FALSO
  | T_NULO
  ;

//   T_OU
//   : expr_bit_ou (T_E^ expr_bit_ou)*
//   : expr_bit_oux (T_BIT_OU^ expr_bit_oux)*
//   : expr_bit_e (T_BIT_OUX^ expr_bit_e)*
//   : expr_igual (T_BIT_E^ expr_igual)*
//   : expr_relacional (T_IGUAL^ expr_relacional | T_DIFERENTE^ expr_relacional)*
//   : expr_ad ((T_MAIOR^ | T_MAIOR_EQ^ | T_MENOR^ | T_MENOR_EQ^) expr_ad)*
//   : expr_multip (T_MAIS^ expr_multip | T_MENOS^ expr_multip)*
//   : expr_unario ((T_DIV^ | T_MULTIP^ | T_MOD^) expr_unario)*
//   : ( T_MENOS      {#op_unario = #[T_UN_NEGATIVO,"&negat"];}
//     | T_MAIS       {#op_unario = #[T_UN_POSITIVO,"&pos"];}
//     | n:T_NAO      {#op_unario = #[T_NAO        ,"negaç"];}
//     | bn:T_BIT_NAO {#op_unario = #[T_BIT_NAO    ,"&negb"];}

// expr_elemento
//   :  (T_IDENTIFICADOR T_ABRE_PAREN)=> chamada_subrotina
//   |  lvalue
//   |  literal
//   | T_ABRE_PAREN! expressao T_FECHA_PAREN!
//   ;
