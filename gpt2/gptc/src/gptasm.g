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
  #include "AsmProgram.hpp"
  #include "SymbolTable.hpp"
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

        (corpo)?
                                {_asmPrg->finishSubroutine(_subroutine);}
        EOF
     )
                                {_asmPrg->finish();}
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
                                              declareSubroutineVars(ids);
                                            } else {
                                              declareProgramVars(ids);
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
                                              declareSubroutineVars(ids);
                                            } else {
                                              declareProgramVars(ids);
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


expressao returns [GptAsmExpression* ret]

  : #(T_EXPRESSAO ret=expr)
  ;

expr returns [GptAsmExpression* ret]

                        {
                          GptAsmExpression *l, *r;
                          Type *optype = _t->getEvalType();
                        }

  : #(T_OU              l=expr r=expr) {ret=expr_OU(optype, l, r);}
  | #(T_E               l=expr r=expr) {ret=expr_E(optype, l, r);}
  | #(T_BIT_OU          l=expr r=expr) {ret=expr_BIT_OU(optype, l, r);}
  | #(T_BIT_OUX         l=expr r=expr) {ret=expr_BIT_XOU(optype, l, r);}
  | #(T_BIT_E           l=expr r=expr) {ret=expr_BIT_E(optype, l, r);}
  | #(T_IGUAL           l=expr r=expr) {ret=expr_IGUAL(optype, l, r);}
  | #(T_DIFERENTE       l=expr r=expr) {ret=expr_DIFERENTE(optype, l, r);}
  | #(T_MAIOR           l=expr r=expr) {ret=expr_MAIOR(optype, l, r);}
  | #(T_MENOR           l=expr r=expr) {ret=expr_MENOR(optype, l, r);}
  | #(T_MAIOR_EQ        l=expr r=expr) {ret=expr_MAIOR_EQ(optype, l, r);}
  | #(T_MENOR_EQ        l=expr r=expr) {ret=expr_MENOR_EQ(optype, l, r);}
  | #(T_BIT_SHIFT_LEFT  l=expr r=expr) {ret=expr_BIT_SHIFT_LEFT(optype, l, r);}
  | #(T_BIT_SHIFT_RIGHT l=expr r=expr) {ret=expr_BIT_SHIFT_RIGHT(optype, l, r);}
  | #(T_MAIS            l=expr r=expr) {ret=expr_MAIS(optype, l, r);}
  | #(T_MENOS           l=expr r=expr) {ret=expr_MENOS(optype, l, r);}
  | #(T_DIV             l=expr r=expr) {ret=expr_DIV(optype, l, r);}
  | #(T_MULTIP          l=expr r=expr) {ret=expr_MULTIP(optype, l, r);}
  | #(T_MOD             l=expr r=expr) {ret=expr_MOD(optype, l, r);}
  | #(T_UN_NEGATIVO     l=elemento)  {ret=expr_UN_NEGATIVO(optype,l);}
  | #(T_UN_POSITIVO     l=elemento)  {ret=expr_UN_POSITIVO(optype,l);}
  | #(T_NAO             l=elemento)  {ret=expr_UN_NAO(optype,l);}
  | #(T_BIT_NAO         l=elemento)  {ret=expr_BIT_NAO(optype,l);}
  | ret=elemento
  ;


elemento returns [GptAsmExpression* ret]
                      {
                        Symbol lv;
                      }
  : ret=literal
  | lv=lvalue         {ret = new GptAsmExpression(lv.type(), lv.identifier());}
//   | ret=chamada_subrotina
  | #(T_EXPRESSAO ret=expr)
  ;

literal returns [GptAsmExpression* ret]

                                    {
                                      Type *type = _t->getEvalType();
                                      std::string value;
                                    }

  : (   t:T_TEXTO_LITERAL           {value = '"' + t->getText() + '"';}
      | i:T_INTEIRO_LITERAL         {value = i->getText();}
      | r:T_REAL_LITERAL            {value = r->getText();}
      | c:T_CARACTERE_LITERAL       {value = "'" + c->getText() + "'";}
      | v:T_VERDADEIRO              {value = "true";}
      | f:T_FALSO                   {value = "false";}
    )
      {ret = new GptAsmExpression(type, value);}
  ;


lvalue returns [Symbol ret]

  : #(lv:T_LVALUE 
      id:T_IDENTIFICADOR  

                     {ret = _symtable->getSymbol(id->getText());}

//       (
//_subroutine->declareTmp(lv->getEvalType());
//         lvalue_indices[lv]
//       )?

//       (
//         lvalue_membro
//       )?
    )
  ;


lvalue_indices[const std::string id]
                                {
                                  std::list<std::string> dimensions;
                                  std::string tmp;
                                }
  : #(T_SUBSCRITO
        (
                                {/*tmp = _subroutine->newTmp();*/}
          expressao
                                {dimensions.push_back(tmp);}
        )+
    )
//                                 {ret = expandOffset(id, dimensions);}
  ;


lvalue_membro
  : #(T_MEMBRO lvalue)
  ;








corpo
  :  bloco_codigo           /*(corpo_subprograma)**/
//   |  corpo_subprograma      (corpo)?
  ;

bloco_codigo
  : #(T_INICIO lista_enunciados)
  ;

lista_enunciados
  : (enunciado)*
  ;

enunciado
  : en_atribuicao
/*  | en_retorne
  | en_se
  | en_enquanto
  | en_repita
  | en_para
  | en_caso
  | T_SAIR
  | chamada_subrotina*/
  ;

en_atribuicao
                      {
                        Symbol lv;
                        GptAsmExpression *expr;
                      }
  : #(T_ATRIBUICAO
        lv=lvalue

        expr=expressao
                      {                        
                        if (expr->isAtom()) {
                          emitAttribution(lv, expr->value());
                        } else {
                          expr->expand(lv.identifier(), _subroutine);
                        }
                        delete expr;
                      }
      )
  ;

