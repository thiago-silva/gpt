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
  #include "BaseGptAsmWalker.hpp"
  #include "PortugolAST.hpp"
  #include "GptAsmExpression.hpp"
}

options {
  language="Cpp";
}

class GptAsmWalker extends TreeParser("BaseGptAsmWalker");
options {
  importVocab    = Portugol;
  noConstructors = true;
  genHashLines   = false;
  ASTLabelType   = "RefPortugolAST";
}

{
public:
  GptAsmWalker::GptAsmWalker(SymbolTable* symtable,
                             const std::string& filepath)
	 : BaseGptAsmWalker(symtable, filepath) {}
}


programa
  : #(T_ALGORITMO                {_asmPrg->init();}

        (importacao)*

        (declaracoes)?

//         (corpo)?

        EOF
     )
  ;

importacao
  : #(T_USE T_TEXTO_LITERAL)
  ;

declaracoes
  : declaracao_variavel[false]    (declaracoes)?
  | declaracao_constante[false]   (declaracoes)?
  | declaracao_estrutura          (declaracoes)?
  ;

declaracao_variavel[bool isLocal]

                                          {std::list<std::string> ids;}

  : #(T_VARIAVEL tipo ids=identificadores
                                          {
                                            if (isLocal) {
                                              declareProgramVars(ids);
                                            } else {
                                              declareSubroutineVars(ids);
                                            }
                                          }
//       (
//         code=valor_inicialiacao            {addPrologue(code);}
//       )?
    )
  ;

declaracao_constante[bool isLocal]
                                          {std::list<std::string> ids;}

  : #(T_CONSTANTE tipo ids=identificadores /*code=valor_inicialiacao*/)

                                          {
                                            if (isLocal) {
                                              declareProgramVars(ids);
                                            } else {
                                              declareSubroutineVars(ids);
                                            }
                                          }
  ;

identificadores returns [std::list<std::string> ids]
  : (id:T_IDENTIFICADOR {ids.push_back(id->getText());})+
  ;

tipo
  : T_IDENTIFICADOR
  | T_MATRIZ
  | T_INTEIRO
  | T_REAL
  | T_CARACTERE
  | T_LITERAL
  | T_LOGICO
  | T_CORINGA
  ;

declaracao_estrutura
  : #(T_ESTRUTURA T_IDENTIFICADOR (campo_estrutura)+)
  ;

campo_estrutura
                      {std::list<std::string> ids;}

  : #(T_VARIAVEL tipo ids=identificadores
//       (
//         valor_inicialiacao
//       )?
    )
  ;




/////////////////////////////////////////////////////////////




/************************** EXPRESSOES *************************/


expressao [const std::string& lvalue]

                                      {GptAsmExpression ret;}
  : #(T_EXPRESSAO ret=expr)
                                      {ret.expand(lvalue,1/*_subroutine*/);}
  ;

expr returns [GptAsmExpression ret]

                        {GptAsmExpression l, r;}

//   : #(T_OU              l=expr r=expr) {expr_OU(ret, l, r);}
//   | #(T_E               l=expr r=expr) {expr_E(ret, l, r);}
//   | #(T_BIT_OU          l=expr r=expr) {expr_BIT_OU(ret, l, r);}
//   | #(T_BIT_OUX         l=expr r=expr) {expr_BIT_XOU(ret, l, r);}
//   | #(T_BIT_E           l=expr r=expr) {expr_BIT_E(ret, l, r);}
//   | #(T_IGUAL           l=expr r=expr) {expr_IGUAL(ret, l, r);}
//   | #(T_DIFERENTE       l=expr r=expr) {expr_DIFERENTE(ret, l, r);}
//   | #(T_MAIOR           l=expr r=expr) {expr_MAIOR(ret, l, r);}
//   | #(T_MENOR           l=expr r=expr) {expr_MENOR(ret, l, r);}
//   | #(T_MAIOR_EQ        l=expr r=expr) {expr_MAIOR_EQ(ret, l, r);}
//   | #(T_MENOR_EQ        l=expr r=expr) {expr_MENOR_EQ(ret, l, r);}
//   | #(T_BIT_SHIFT_LEFT  l=expr r=expr) {expr_BIT_SHIFT_LEFT(ret, l, r);}
//   | #(T_BIT_SHIFT_RIGHT l=expr r=expr) {expr_BIT_SHIFT_RIGHT(ret, l, r);}
  : #(T_MAIS            l=expr r=expr) {expr_MAIS(ret, l, r);}
//   | #(T_MENOS           l=expr r=expr) {expr_MENOS(ret, l, r);}
//   | #(T_DIV             l=expr r=expr) {expr_DIV(ret, l, r);}
//   | #(T_MULTIP          l=expr r=expr) {expr_MULTIP(ret, l, r);}
//   | #(T_MOD             l=expr r=expr) {expr_MOD(ret, l, r);}
//   | #(T_UN_NEGATIVO     l=elemento)  {expr_UN_NEGATIVO(ret,l);}
//   | #(T_UN_POSITIVO     l=elemento)  {expr_UN_POSITIVO(ret,l);}
//   | #(T_NAO             l=elemento)  {expr_UN_NAO(ret,l);}
//   | #(T_BIT_NAO         l=elemento)  {expr_BIT_NAO(ret,l);}
  | ret=elemento
  ;


elemento returns [GptAsmExpression ret]
                      {std::string r;}
  : ret=literal
  | r=lvalue
//   | ret=chamada_subrotina
  | expressao[r]
  ;

literal returns [GptAsmExpression ret]

  : l:T_TEXTO_LITERAL          {/*ret.set(l->getText());*/}
  | i:T_INTEIRO_LITERAL        {/*ret.set(i->getText());*/}
  | r:T_REAL_LITERAL           {/*ret.set(r->getText());*/}
  | c:T_CARACTERE_LITERAL      {/*ret.set(c->getText());*/}
  | v:T_VERDADEIRO             {/*ret.set(v->getText());*/}
  | f:T_FALSO                  {/*ret.set(f->getText());*/}
  ;


lvalue returns [std::string lv]

                                      {std::string offset;}

  : #(id:T_IDENTIFICADOR              {lv = id->getText();}
      (offset=lvalue_indices[lv]      {lv += ":" + offset;})?
//       (lvalue_membro)?
    )
                
  ;


lvalue_indices[const std::string id] returns [std::string ret]
                                {
                                  std::list<std::string> dimensions;
                                  std:string tmp;
                                }
  : #(T_SUBSCRITO
        (
                                {/*tmp = _subroutine->newTmp();*/}
          expressao[tmp]
                                {dimensions.push_back(tmp);}
        )+
    )
                                {ret = expandOffset(id, dimensions);}
  ;


// lvalue_membro
//   : #(T_MEMBRO lvalue)
//   ;








// corpo
//   :  corpo_subprograma      (corpo)?
//   |  bloco_codigo           (corpo_subprograma)*
//   ;
// 
// 
// bloco_codigo
//   : #(T_INICIO lista_enunciados)
//   ;
// 
// lista_enunciados
//   : (enunciado)*
//   ;
// 
// enunciado
//   : en_atribuicao
// /*  | en_retorne
//   | en_se
//   | en_enquanto
//   | en_repita
//   | en_para
//   | en_caso
//   | T_SAIR
//   | chamada_subrotina*/
//   ;
// 
// en_atribuicao
//                       {std::string lv;}
//   : #(T_ATRIBUICAO
//         lv=lvalue      //x / x:offset
//         expressao[lv]
//       )
//   ;

