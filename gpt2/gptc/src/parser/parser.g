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
  #include <antlr/NoViableAltException.hpp>
  #include <antlr/SemanticException.hpp>

  #include "PortugolAST.hpp"
  using namespace antlr;
}

options {
  language="Cpp";
}

class PortugolParser extends Parser;
options {
  importVocab    = Portugol;
  genHashLines   = false;
  buildAST       = true;
  ASTLabelType   = "RefPortugolAST";
  noConstructors = true;
}

{
public:
  PortugolParser(antlr::TokenBuffer& tokenBuf, bool report = true)
  : antlr::LLkParser(tokenBuf,1), _reportErrors(report), _hasErrors(false)
  {
  }

  PortugolParser(antlr::TokenStream& lexer, bool report = true)
  : antlr::LLkParser(lexer,1), _reportErrors(report), _hasErrors(false)
  {
  }

  void reportError(const RecognitionException& ex) {
    _hasErrors = true;
    if (!_reportErrors) return;
    std::cerr << ex.toString().c_str() << std::endl;
  }

  void reportError(const std::string& s) {
    _hasErrors = true;
    if (!_reportErrors) return;
    if (getFilename() == "") {
      std::cerr << "error: " << s.c_str() << std::endl;
    } else {
      std::cerr << getFilename().c_str() << ": error: " << s.c_str() << std::endl;
    }
  }

  void reportWarning(const std::string& s) {
    if (!_reportErrors) return;
    if (getFilename() == "") {
      std::cerr << "warning: " << s.c_str() << std::endl;
    } else {
      std::cerr << getFilename().c_str() << ": warning: " << s.c_str() << std::endl;
    }
  }

  bool hasErorrs() {
    return _hasErrors;
  }

private:
  void createRootNode(antlr::ASTPair& ast, antlr::RefToken token) {
    astFactory->makeASTRoot(ast, astFactory->create(token));
  }

  void createRootNode(antlr::ASTPair& ast, int token, std::string txt = "") {
    astFactory->makeASTRoot(ast, astFactory->create(token, txt));
  }

  void addChildNode(antlr::ASTPair& ast, antlr::RefToken token) {
    astFactory->addASTChild(ast, astFactory->create(token));
  }

  void addChildNode(antlr::ASTPair& ast, RefAST child) {
    astFactory->addASTChild(ast, child);
  }

  antlr::RefAST createNode(int type, const std::string& txt) {
    return astFactory->create(type, txt);
  }

  bool _reportErrors;
  bool _hasErrors;
}



//####################################
//##             Início             ##
//####################################

programa
{createRootNode(currentAST, T_ALGORITMO, "&algoritmo");}

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
  : T_ALGORITMO T_IDENTIFICADOR T_PONTO_VIRGULA
  ;

importacao
  : T_USE^ T_TEXTO_LITERAL T_PONTO_VIRGULA!
  ;

corpo
  :  declaracao_subprograma corpo
  |  bloco_codigo           (declaracao_subprograma)*
  |  /*vazio*/
  ;



//####################################
//##    Declaração de Variáveis     ##
//####################################

declaracao_variaveis
  : bloco_declaracao_variaveis
  | T_VARIAVEL! declaracao_variavel_uniq
  ;

bloco_declaracao_variaveis
  : T_VARIAVEIS! (declaracao_variavel)+ T_FIM_VARIAVEIS!
  ;

declaracao_variavel!
  : ids:lista_identificadores   T_2_PONTOS
    t:tipo (l:lista_inicializacao)? T_PONTO_VIRGULA

            {
              if (l_AST == antlr::nullAST) {
                #declaracao_variavel =
                    #([T_VARIAVEL,"&var"], t,ids);
              } else {
                #declaracao_variavel =
                    #([T_VARIAVEL,"&var"], t,ids,([T_VALOR,"&valor"],l));
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
                    #([T_VARIAVEL,"&var"], t,id);
              } else {
                #declaracao_variavel_uniq =
                    #([T_VARIAVEL,"&var"], t,id,([T_VALOR,"&valor"],l));
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
  : ch:T_ABRE_CHAVE!
      idx:inicializacao_indices
    T_FECHA_CHAVE!

                 {
                   #inicializacao_composta = #([T_VAL_MATRIZ, "&vmatriz"],idx);
                   #inicializacao_composta->setLine(ch->getLine());
                 }

  | co:T_ABRE_COLCHETE!
      mem:inicializacao_membros
    T_FECHA_COLCHETE!

                 {
                   #inicializacao_composta =
                       #([T_VAL_ESTRUTURA, "&vestr"],mem);
                   #inicializacao_composta->setLine(co->getLine());
                 }
  ;

inicializacao_indices
  : (expressao | inicializacao_composta) (T_VIRGULA! inicializacao_indices)?
  ;

inicializacao_membros
  : T_IDENTIFICADOR T_ATRIBUICAO! (expressao | inicializacao_composta)
    (T_VIRGULA! inicializacao_membros)?
  ;

tipo_primitivo
  : T_INTEIRO
  | T_REAL
  | T_CARACTERE
  | T_LITERAL
  | T_LOGICO
  | T_CORINGA
  ;

tipo_matriz!
  : m:T_MATRIZ dim:matriz_dimensoes
    T_DO T_TIPO t:tipo_da_matriz

                    {#tipo_matriz = #(m, t,dim);}
  ;

matriz_dimensoes
  : ( T_ABRE_CHAVE!
      ( T_INTEIRO_LITERAL
        | /*vazio*/  {addChildNode(currentAST, createNode(T_NULO,"&null"));}
      )
      T_FECHA_CHAVE!
    )+
  ;

tipo_da_matriz
  : tipo_primitivo | T_IDENTIFICADOR
  ;



//####################################
//##    Declaração de Constantes    ##
//####################################


declaracao_constantes
  : bloco_declaracao_constantes
  | T_CONSTANTE! declaracao_constante
  ;

bloco_declaracao_constantes
  : T_CONSTANTES! (declaracao_constante)+ T_FIM_CONSTANTES!
  ;

declaracao_constante!
  : id:T_IDENTIFICADOR T_2_PONTOS t:tipo
    l:lista_inicializacao T_PONTO_VIRGULA
               {#declaracao_constante =
                    #([T_CONSTANTE,"&const"],t,id,([T_VALOR,"&valor"],l));}
  ;




//####################################
//##    Declaração de Estruturas    ##
//####################################


bloco_declaracao_estrutura
  : T_ESTRUTURA^ T_IDENTIFICADOR (declaracao_variavel)+ T_FIM_ESTRUTURA!
  ;




//####################################
//##    Declaração de Subrotinas    ##
//####################################


declaracao_subprograma
  : declaracao_funcao
  | declaracao_procedimento
  ;

declaracao_funcao
  : T_FUNCAO^ T_IDENTIFICADOR
      T_ABRE_PAREN! (lista_parametros)? T_FECHA_PAREN! T_2_PONTOS! tipo_retorno
    (declaracao_constantes | declaracao_variaveis)*
    bloco_codigo
  ;

declaracao_procedimento
  : T_PROCEDIMENTO^ T_IDENTIFICADOR
      T_ABRE_PAREN! (lista_parametros)? T_FECHA_PAREN!
    (declaracao_constantes | declaracao_variaveis)*
    bloco_codigo
  ;

tipo_retorno!
  : t:tipo {#tipo_retorno = #([T_TIPO_RETORNO,"&ret"],t);}
  ;

lista_parametros
  : (param_reticencias | parametro (T_VIRGULA! lista_parametros)?)
  ;

param_reticencias!
  : ret:T_RETICENCIAS id:T_IDENTIFICADOR
                       {#param_reticencias = #([T_PARAM,"&param"],ret,id);}
  ;

parametro!
  : (r:T_REF (c:T_CONSTANTE)?)? id:T_IDENTIFICADOR T_2_PONTOS t:tipo

                          {#parametro = #([T_PARAM,"&param"],t,id,r,c);}
  ;


//####################################
//##         Bloco de código        ##
//####################################

bloco_codigo
  : T_INICIO^ lista_enunciados T_FIM!
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

  | T_SAIR^ T_PONTO_VIRGULA!
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
  : T_RETORNE^ (expressao)? T_PONTO_VIRGULA!
  ;

lvalue!
  : id:T_IDENTIFICADOR (m:lvalue_indices)? (T_PONTO es:lvalue_membro)?
    {#lvalue = #([T_LVALUE,"&lvalue"], id, m, es);}
  ;

lvalue_membro!
  : l:lvalue             {#lvalue_membro = #([T_MEMBRO,"&membro"], l);}
  ;

lvalue_indices
  : i:indices_matriz     {#lvalue_indices = #([T_SUBSCRITO,"&subscrito"], i);}
  ;

indices_matriz
  : ( T_ABRE_CHAVE!
      expressao
      T_FECHA_CHAVE!
    )+
  ;

en_se
  : T_SE^ expressao T_ENTAO!
      lista_enunciados (T_SENAO lista_enunciados)? T_FIM_SE!
  ;

en_enquanto
  : T_ENQUANTO^ expressao T_FACA! lista_enunciados T_FIM_ENQUANTO!
  ;

en_para
  : T_PARA^ lvalue T_DE! expressao
    T_ATE! expressao (passo)? T_FACA!
      lista_enunciados T_FIM_PARA!
  ;

passo
  : T_PASSO^ (T_MAIS|T_MENOS)? T_INTEIRO_LITERAL
  ;

en_repita
  : T_REPITA^ lista_enunciados T_ATE expressao
  ;

en_caso
  : T_CASO^ expressao
      T_SEJA! (teste_caso)+
      (caso_senao)? T_FIM_CASO!
  ;

teste_caso
  : literal T_FACA^ lista_enunciados T_FIM_FACA!
  ;

caso_senao
  : T_SENAO^ lista_enunciados T_FIM_SENAO!
  ;


//TODO: codigo ilustrativo.
en_asm!
  : T_ASM (~(T_FIMASM))* T_FIMASM
  ;


//####################################
//##            Expressões          ##
//####################################

expressao!
  : ou:expr_ou {#expressao = #([T_EXPRESSAO,"&expr"], ou);}
  ;

expr_ou
  : expr_e (T_OU^ expr_e)*
  ;

expr_e
options {
  defaultErrorHandler=false;
}
  : expr_bit_ou (T_E^ expr_bit_ou)*
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
  : expr_eq (T_BIT_E^ expr_eq)*
  ;

expr_eq
options {
  defaultErrorHandler=false;
}
  : expr_relacional (T_IGUAL^ expr_relacional | T_DIFERENTE^ expr_relacional)*
  ;

expr_relacional
options {
  defaultErrorHandler=false;
}
  : expr_bit_shift ((T_MAIOR^ | T_MAIOR_EQ^ | T_MENOR^ | T_MENOR_EQ^) expr_bit_shift)*
  ;

expr_bit_shift
options {
  defaultErrorHandler=false;
}
  : expr_soma ((T_BIT_SHIFT_LEFT^ | T_BIT_SHIFT_RIGHT^) expr_soma)*
  ;

expr_soma
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

expr_unario!
options {
  defaultErrorHandler=false;
}
  : o:op_unario e:expr_elemento {#expr_unario = #(o,e);}
  ;

op_unario!
  : ( T_MENOS      {#op_unario = #[T_UN_NEGATIVO,"&negat"];}
    | T_MAIS       {#op_unario = #[T_UN_POSITIVO,"&pos"];}
    | n:T_NAO      {#op_unario = #n;}
    | bn:T_BIT_NAO {#op_unario = #bn;}
    )?
  ;

expr_elemento
  :  (T_IDENTIFICADOR T_ABRE_PAREN)=> chamada_subrotina
  |  lvalue
  |  literal
  | T_ABRE_PAREN! expressao T_FECHA_PAREN!
  ;


chamada_subrotina!
  : id:T_IDENTIFICADOR T_ABRE_PAREN (args:lista_argumentos)? T_FECHA_PAREN

                        {#chamada_subrotina = #([T_CALL,"&call"], id, args);}
  ;

lista_argumentos
  : expressao (T_VIRGULA! expressao)*
  ;

literal
  : T_TEXTO_LITERAL
  | T_INTEIRO_LITERAL
  | T_REAL_LITERAL
  | T_CARACTERE_LITERAL
  | T_VERDADEIRO
  | T_FALSO
  ;
