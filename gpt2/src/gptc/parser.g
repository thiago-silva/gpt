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


options {
  language="Cpp";
}

class PortugolParser extends Parser;
options {
  importVocab  = Portugol;
  genHashLines = false;
  buildAST     = true;
}

{
private:
  void createRootNode(antlr::ASTPair& ast, antlr::RefToken token) {
    astFactory->makeASTRoot(ast, astFactory->create(token));
  }

  void createRootNode(antlr::ASTPair& ast, int token) {
    astFactory->makeASTRoot(ast, astFactory->create(token));
  }

  void addChildNode(antlr::ASTPair& ast, antlr::RefToken token) {
    astFactory->addASTChild(ast, astFactory->create(token));
  }

  void addChildNode(antlr::ASTPair& ast, antlr::RefAST child) {
    astFactory->addASTChild(ast, child);
  }

  antlr::RefAST createNode(int type, const std::string& txt) {
    return astFactory->create(type, txt);
  }
}



//####################################
//##             Início             ##
//####################################

programa
  : (declaracao_algoritmo)?

    (importacao)*

    (   declaracao_variaveis
      | declaracao_constantes
      | bloco_declaracao_estrutura
    )*

    corpo

    EOF
  ;

declaracao_algoritmo!
  : T_KW_ALGORITMO T_IDENTIFICADOR T_PONTO_VIRGULA
  ;

importacao
  : T_KW_USE^ T_TEXTO_LITERAL T_PONTO_VIRGULA!
  ;

corpo
  :  (declaracao_subrotina)*
      bloco_codigo
     (declaracao_subrotina)*
  ;



//####################################
//##    Declaração de Variáveis     ##
//####################################

declaracao_variaveis
  : bloco_declaracao_variaveis
  | T_KW_VARIAVEL! declaracao_variavel_uniq
  ;

bloco_declaracao_variaveis
  : T_KW_VARIAVEIS^ (declaracao_variavel)+ T_KW_FIM_VARIAVEIS!
  ;

declaracao_variavel!
  : ids:lista_identificadores   T_2_PONTOS
    t:tipo (l:lista_inicializacao)? T_PONTO_VIRGULA

            {
              if (l_AST == antlr::nullAST) {
                #declaracao_variavel =
                    #([T_KW_VARIAVEL,"&var"], t,ids);
              } else {
                #declaracao_variavel =
                    #([T_KW_VARIAVEL,"&var"], t,ids,([TI_VALOR,"&valor"],l));
              }
            }
  ;

lista_identificadores
  : T_IDENTIFICADOR (T_VIRGULA! T_IDENTIFICADOR)*
  ;

declaracao_variavel_uniq!
  : id:T_IDENTIFICADOR T_2_PONTOS t:tipo
    (l:lista_inicializacao)? T_PONTO_VIRGULA

            {
              if (l_AST == antlr::nullAST) {
                #declaracao_variavel_uniq=
                    #([T_KW_VARIAVEL,"&var"], t,id);
              } else {
                #declaracao_variavel_uniq =
                    #([T_KW_VARIAVEL,"&var"], t,id,([TI_VALOR,"&valor"],l));
              }
            }
  ;

tipo
  : tipo_primitivo
  | tipo_matriz
  | T_IDENTIFICADOR
  ;

lista_inicializacao
  : T_ATRIBUICAO! (expressao | inicializacao_composta)
  ;

inicializacao_composta!
  : T_ABRE_CHAVE!
      idx:inicializacao_indices
    T_FECHA_CHAVE!

                 {#inicializacao_composta = #([TI_VMATRIZ, "&vmatriz"],idx);}

  | T_ABRE_COLCHETE!
      mem:inicializacao_membros
    T_FECHA_COLCHETE!

                 {#inicializacao_composta = #([TI_VESTRUTURA, "&vestr"],mem);}
  ;

inicializacao_indices
  : (expressao | inicializacao_composta) (T_VIRGULA! inicializacao_indices)?
  ;

inicializacao_membros
  : T_IDENTIFICADOR T_ATRIBUICAO! (expressao | inicializacao_composta)
    (T_VIRGULA! inicializacao_membros)?
  ;

tipo_primitivo
  : T_KW_INTEIRO
  | T_KW_REAL
  | T_KW_CARACTERE
  | T_KW_LITERAL
  | T_KW_LOGICO
  | T_KW_CORINGA
  ;

tipo_matriz!
  : T_KW_MATRIZ dim:matriz_dimensoes
    T_KW_DO T_KW_TIPO t:matriz_tipo

                    {#tipo_matriz = #([TI_MATRIZ, "&matriz"], t,dim);}
  ;

matriz_dimensoes
  : ( T_ABRE_CHAVE!
      ( T_INTEIRO_LITERAL
        | /*vazio*/  {addChildNode(currentAST, createNode(TI_NULL,"&null"));}
      )
      T_FECHA_CHAVE!
    )+
  ;

matriz_tipo
  : tipo_primitivo | T_IDENTIFICADOR
  ;



//####################################
//##    Declaração de Constantes    ##
//####################################


declaracao_constantes
  : bloco_declaracao_constantes
  | T_KW_CONSTANTE! declaracao_constante
  ;

bloco_declaracao_constantes
  : T_KW_CONSTANTES^ (declaracao_constante)+ T_KW_FIM_CONSTANTES!
  ;

declaracao_constante!
  : id:T_IDENTIFICADOR T_2_PONTOS t:tipo
    l:lista_inicializacao T_PONTO_VIRGULA

               {#declaracao_constante = #([T_KW_CONSTANTE,"&const"], t,id,l);}
  ;




//####################################
//##    Declaração de Estruturas    ##
//####################################


bloco_declaracao_estrutura
  : T_KW_ESTRUTURA^ T_IDENTIFICADOR (declaracao_variavel)+ T_KW_FIM_ESTRUTURA!
  ;




//####################################
//##    Declaração de Subrotinas    ##
//####################################

declaracao_subrotina
  : declaracao_funcao
  | declaracao_procedimento
  ;

declaracao_funcao
  : T_KW_FUNCAO^ T_IDENTIFICADOR
      T_ABRE_PAREN! (lista_parametros)? T_FECHA_PAREN! T_2_PONTOS! tipo
    (declaracao_constantes | declaracao_variaveis)*
    bloco_codigo
  ;

declaracao_procedimento
  : T_KW_PROCEDIMENTO^ T_IDENTIFICADOR
      T_ABRE_PAREN! (lista_parametros)? T_FECHA_PAREN!
    (declaracao_constantes | declaracao_variaveis)*
    bloco_codigo
  ;

lista_parametros
  : parametro (T_VIRGULA! parametro)*
  ;

parametro!
  : ((c:T_KW_CONSTANTE)? r:T_KW_REF)? id:T_IDENTIFICADOR T_2_PONTOS t:tipo

                        {#parametro = #([T_KW_VARIAVEL,"&param"],t,id,c,r);}
  ;



//####################################
//##         Bloco de código        ##
//####################################

bloco_codigo
  : T_KW_INICIO^ lista_enunciados T_KW_FIM!
  ;

lista_enunciados
  : (enunciado)*
  ;

enunciado
  : (lvalue T_ATRIBUICAO)=> en_atribuicao
  | en_retorne
  | en_se
  | en_enquanto
  | en_repita
  | en_para
  | en_caso

  | en_asm

  | T_KW_SAIR^ T_PONTO_VIRGULA!
  | T_PONTO_VIRGULA!

//  TODO: retirar da gramatica ou resolver a ambiguidade
//        se esta producao vingar, retirar enunciado::chamada_subrotina
//   | expressao  T_PONTO_VIRGULA!

  | chamada_subrotina T_PONTO_VIRGULA!
  ;

en_atribuicao
  : lvalue T_ATRIBUICAO^ expressao T_PONTO_VIRGULA!
  ;


en_retorne
  : T_KW_RETORNE^ (expressao)? T_PONTO_VIRGULA!
  ;

lvalue
  : T_IDENTIFICADOR^ matriz_indices
  ;

matriz_indices
  : ( T_ABRE_CHAVE!
      expressao
      T_FECHA_CHAVE!
    )*
  ;

en_se
  : T_KW_SE^ expressao T_KW_ENTAO!
      lista_enunciados (T_KW_SENAO! lista_enunciados)? T_KW_FIM_SE!
  ;

en_enquanto
  : T_KW_ENQUANTO^ expressao T_KW_FACA! lista_enunciados T_KW_FIM_ENQUANTO!
  ;

en_para
  : T_KW_PARA^ lvalue T_KW_DE! expressao
    T_KW_ATE! expressao (passo)? T_KW_FACA!
      lista_enunciados T_KW_FIM_PARA!
  ;

passo
  : T_KW_PASSO^ (T_MAIS|T_MENOS)? T_INTEIRO_LITERAL
  ;

en_repita
  : T_KW_REPITA^ lista_enunciados T_KW_ATE expressao
  ;

en_caso
  : T_KW_CASO^ expressao
      T_KW_SEJA! (teste_caso)+
      (caso_senao)? T_KW_FIM_CASO!
  ;

teste_caso!
  : l:literal T_KW_FACA! en:lista_enunciados T_KW_FIM_FACA!

                                {#teste_caso = #(l, en);}
  ;

caso_senao
  : T_KW_SENAO^ lista_enunciados T_KW_FIM_SENAO!
  ;


//TODO: codigo ilustrativo.
en_asm!
  : T_KW_ASM (~(T_KW_FIMASM))* T_KW_FIMASM
  ;


//####################################
//##            Expressões          ##
//####################################

expressao
  : expr_e (T_KW_OU^ expr_e)*
  ;

expr_e
options {
  defaultErrorHandler=false;
}
  : expr_bit_ou (T_KW_E^ expr_bit_ou)*
  ;

expr_bit_ou
options {
  defaultErrorHandler=false;
}
  : expr_bit_oux (T_BIT_OU^ expr_bit_oux)*
  ;

expr_bit_oux
options {
  defaultErrorHandler=false;
}
  : expr_bit_e (T_BIT_OUX^ expr_bit_e)*
  ;

expr_bit_e
options {
  defaultErrorHandler=false;
}
  : expr_igual (T_BIT_E^ expr_igual)*
  ;

expr_igual
options {
  defaultErrorHandler=false;
}
  : expr_relacional (T_IGUAL^ expr_relacional | T_DIFERENTE^ expr_relacional)*
  ;

expr_relacional
options {
  defaultErrorHandler=false;
}
  : expr_ad ((T_MAIOR^ | T_MAIOR_EQ^ | T_MENOR^ | T_MENOR_EQ^) expr_ad)*
  ;

expr_ad
options {
  defaultErrorHandler=false;
}
  : expr_multip (T_MAIS^ expr_multip | T_MENOS^ expr_multip)*
  ;

expr_multip
options {
  defaultErrorHandler=false;
}
  : expr_unario ((T_DIV^ | T_MULTIP^ | T_MOD^) expr_unario)*
  ;

expr_unario
options {
  defaultErrorHandler=false;
}
  : op_unario expr_elemento
  ;

op_unario!
  : ( T_MENOS   {#op_unario = #[TI_UN_NEGATIVO,"&negat"];}
    | T_MAIS    {#op_unario = #[TI_UN_POSITIVO,"&pos"];}
    | T_KW_NAO  {#op_unario = #[TI_UN_NEGACAO ,"negaç"];}
    | T_BIT_NAO {#op_unario = #[TI_UN_NEGACAOB,"&negb"];}
    )?
  ;

expr_elemento
  :  (T_IDENTIFICADOR T_ABRE_PAREN)=> chamada_subrotina
  |  lvalue
  |  literal
  |! T_ABRE_PAREN e:expressao T_FECHA_PAREN {#expr_elemento =
                                              #([TI_PARENTESIS,"&paren"], e);}
  ;


chamada_subrotina!
  : id:T_IDENTIFICADOR T_ABRE_PAREN (args:lista_argumentos)? T_FECHA_PAREN

                        {#chamada_subrotina = #([TI_CALL,"&call"], id, args);}
  ;

lista_argumentos
  : expressao (T_VIRGULA! expressao)*
  ;

literal
  : T_TEXTO_LITERAL
  | T_INTEIRO_LITERAL
  | T_REAL_LITERAL
  | T_CARACTERE_LITERAL
  | T_KW_VERDADEIRO
  | T_KW_FALSO
  ;
