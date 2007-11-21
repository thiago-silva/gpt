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
  #include "PortugolAST.hpp"
  #include "Symbol.hpp"
  #include "SymbolTable.hpp"
}


options {
  language="Cpp";
}

class SemanticWalker extends TreeParser("BaseSemanticWalker");
options {
  importVocab    = Portugol;
  noConstructors = true;
  genHashLines   = false;
  ASTLabelType   = "RefPortugolAST";
//   defaultErrorHandler=false;
}

{
public:
  SemanticWalker::SemanticWalker(SymbolTable* stable)
	 : BaseSemanticWalker(stable) { }
}


programa
                                      {RefPortugolAST inicio;}
  : #(T_ALGORITMO

        (importacao)*

        declaracoes_globais
                                      //recuperando declaracoes globais e
                                      //de subprogramas para a symtable
                                      {inicio = _t; /*backup da posicao*/}
        definicoes_subprogramas

        EOF
                                      //analise semantica do codigo
                                      {_t = inicio; /*rollback*/}
        corpo
        EOF
     )
  ;

importacao
  : #(T_USE lib:T_TEXTO_LITERAL)                  {useLib(lib->getText());}
  ;

//TODO: checar inicialização:
//      ex: variável z : inteiro := f(); //erro!

declaracoes_globais
  : declaracao_variavel  (declaracoes_globais)?
  | declaracao_constante (declaracoes_globais)?
  | definicao_estrutura  (declaracoes_globais)?
  ;

definicoes_subprogramas
  : definicao_subprograma (definicoes_subprogramas)?
  | T_INICIO (definicoes_subprogramas)?
  ;

declaracao_variavel
                                {
                                  IDList ids;
                                  Type *type;
                                }

  : #(T_VARIAVEL type=tipo ids=identificadores (valor_inicialiacao)?)

                                {declare(ids, type, false);}
  ;

identificadores returns [IDList list]
  : (
      id:T_IDENTIFICADOR      {list.push_back(id);}
    )*
  ;

declaracao_constante
                                {
                                  IDList ids;
                                  Type *type;
                                }

  : #(T_CONSTANTE type=tipo ids=identificadores valor_inicialiacao)

                                {declare(ids, type, true);}
  ;

definicao_estrutura
                                {SymbolList symbols;}

  : #(T_ESTRUTURA id:T_IDENTIFICADOR symbols=campos_estrutura)

                                {defineStruct(id->getText(),
                                    symbols, id->getLine());}
  ;

campos_estrutura returns [SymbolList symbols]

                                {
                                  IDList ids;
                                  Type *type;
                                }

  : (
      #(T_VARIAVEL type=tipo ids=identificadores (valor_inicialiacao)?)

                                {
                                  for (IDList::iterator it = ids.begin();
                                         it != ids.end(); ++it) {
                                    symbols.push_back(
                                      _symtable->newSymbol(
                                          (*it)->getText(), type,
                                               (*it)->getLine()));
                                  }
                                }
    )+
  ;

valor_inicialiacao
  : #(T_VALOR valor)
  ;

valor returns [Type *type]
  : type=expressao
  | #(T_VAL_MATRIZ    (valor)*)
  | #(T_VAL_ESTRUTURA (valor)*)
  ;

tipo returns [Type *type]
  : id:T_IDENTIFICADOR       {type = getType(id->getText());}
  | type=tipo_primitivo
  | type=tipo_matriz
  ;

tipo_primitivo returns [Type *type]
  : i:T_INTEIRO             {type = getType(i->getText());}
  | r:T_REAL                {type = getType(r->getText());}
  | ca:T_CARACTERE          {type = getType(ca->getText());}
  | li:T_LITERAL            {type = getType(li->getText());}
  | lo:T_LOGICO             {type = getType(lo->getText());}
  | co:T_CORINGA            {type = getType(co->getText());}
  ;

tipo_matriz returns [Type *type]

                            {
                              int d = 0;
                              Type * ofType;
                            }

  : #(T_MATRIZ ofType=tipo (dimensao {d++;})*)

                            {type = _symtable->retrieveMatrixType(ofType,d);}
  ;

dimensao
  : T_INTEIRO_LITERAL
  | T_NULO
  ;

definicao_subprograma
  : declaracao_funcao
  | declaracao_procedimento
  ;

declaracao_procedimento
                                      {SymbolList params;}

  : #(T_PROCEDIMENTO
        id:T_IDENTIFICADOR
        params=lista_parametros[id->getText()]

                                      {
                                        declareProc(id->getText(),
                                                   params, id->getLine());

                                        _symtable->setScope(id->getText());
                                      }

    (declaracao_variavel | declaracao_constante)*)

                                      {_symtable->setGlobalScope();}
  ;

declaracao_funcao
                                      {
                                        SymbolList params;
                                        Type * rettype;
                                      }
  : #(T_FUNCAO
        id:T_IDENTIFICADOR
        params=lista_parametros[id->getText()]
        rettype=tipo_retorno
                                      {
                                        declareProc(id->getText(),
                                            params, id->getLine(), rettype);

                                        _symtable->setScope(id->getText());
                                      }

    (declaracao_variavel | declaracao_constante)*)

                                      {_symtable->setGlobalScope();}
  ;

tipo_retorno returns [Type * ret]
  : #(T_TIPO_RETORNO ret=tipo)
  ;

lista_parametros[std::string scope] returns [SymbolList list]

                                  {Type *type;}
  : (
      #(T_PARAM type=tipo id:T_IDENTIFICADOR (r:T_REF)? (c:T_CONSTANTE)?)

                                  {
                                    list.push_back(
                                        _symtable->newSymbol(
                                          id->getText(), type,
                                          id->getLine(),
                                          scope,
                                          c != antlr::nullAST,
                                          r != antlr::nullAST));

                                      c = antlr::nullAST;
                                      r = antlr::nullAST;
                                  }
    )*
  ;


corpo
  :  corpo_subprograma      (corpo)?
  |  bloco_codigo           (corpo_subprograma)*
  ;


corpo_subprograma
  : #(T_FUNCAO   f:T_IDENTIFICADOR     {_symtable->setScope(f->getText());}
                 (~(T_INICIO))*
                 bloco_codigo          {_symtable->setGlobalScope();}
    )
  | #(T_PROCEDIMENTO p:T_IDENTIFICADOR {_symtable->setScope(p->getText());}
                 (~(T_INICIO))*
                 bloco_codigo)         {_symtable->setGlobalScope();}
  ;



bloco_codigo
  : #(T_INICIO (enunciado)*)
  ;

enunciado
  : en_atribuicao
//   | en_retorne
//   | en_se
//   | en_enquanto
//   | en_repita
//   | en_para
//   | en_caso
//   | en_asm
//
//   | T_SAIR
//   | chamada_subrotina T_PONTO_VIRGULA!
  ;

en_atribuicao
                        {Type *ltype, *rtype;}

  : #(T_ATRIBUICAO ltype=lvalue rtype=expressao)

                        {evalAttribution(ltype, rtype);}
  ;

expressao returns [Type *type]
                                                 {Type *l, *r;}

  : #(T_OU              l=expressao r=expressao)
                                    {type = evalExpr<T_OU>(l,r);}

  | #(T_E               l=expressao r=expressao)
                                    {type = evalExpr<T_E>(l,r);}

  | #(T_BIT_OU          l=expressao r=expressao)
                                    {type = evalExpr<T_BIT_OU>(l,r);}

  | #(T_BIT_OUX         l=expressao r=expressao)
                                    {type = evalExpr<T_BIT_OUX>(l,r);}

  | #(T_BIT_E           l=expressao r=expressao)
                                    {type = evalExpr<T_BIT_E>(l,r);}

  | #(T_IGUAL           l=expressao r=expressao)
                                    {type = evalExpr<T_IGUAL>(l,r);}

  | #(T_DIFERENTE       l=expressao r=expressao)
                                    {type = evalExpr<T_DIFERENTE>(l,r);}

  | #(T_MAIOR           l=expressao r=expressao)
                                    {type = evalExpr<T_MAIOR>(l,r);}

  | #(T_MENOR           l=expressao r=expressao)
                                    {type = evalExpr<T_MENOR>(l,r);}

  | #(T_MAIOR_EQ        l=expressao r=expressao)
                                    {type = evalExpr<T_MAIOR_EQ>(l,r);}

  | #(T_MENOR_EQ        l=expressao r=expressao)
                                    {type = evalExpr<T_MENOR_EQ>(l,r);}

  | #(T_BIT_SHIFT_LEFT  l=expressao r=expressao)
                                    {type = evalExpr<T_BIT_SHIFT_LEFT>(l,r);}

  | #(T_BIT_SHIFT_RIGHT l=expressao r=expressao)
                                    {type = evalExpr<T_BIT_SHIFT_RIGHT>(l,r);}

  | #(T_MAIS            l=expressao r=expressao)
                                    {type = evalExpr<T_MAIS>(l,r);}

  | #(T_MENOS           l=expressao r=expressao)
                                    {type = evalExpr<T_MENOS>(l,r);}

  | #(T_DIV             l=expressao r=expressao)
                                    {type = evalExpr<T_DIV>(l,r);}

  | #(T_MULTIP          l=expressao r=expressao)
                                    {type = evalExpr<T_MULTIP>(l,r);}

  | #(T_MOD             l=expressao r=expressao)
                                    {type = evalExpr<T_MOD>(l,r);}

  | #(T_UN_NEGATIVO    l=elemento)
                                    {type = evalExpr<T_UN_NEGATIVO>(l);}

  | #(T_UN_POSITIVO   l=elemento)
                                    {type = evalExpr<T_UN_POSITIVO>(l);}

  | #(T_NAO            l=elemento)
                                    {type = evalExpr<T_NAO>(l);}

  | #(T_BIT_NAO        l=elemento)
                                    {type = evalExpr<T_BIT_NAO>(l);}

  | type=elemento
  ;


elemento returns [Type *type]
  : type=literal
  | type=chamada_subrotina
  | type=lvalue
  | #(T_ABRE_PAREN type=expressao)
  ;

lvalue returns [Type *type]

  : #(id:T_IDENTIFICADOR

      (   lvalue_indices         {type = getSymbolType(id->getText());}
        | type=lvalue_membro
        | /*vazio*/              {type = getSymbolType(id->getText());}
      )
    )
  ;

lvalue_membro returns [Type *type]
  : #(T_MEMBRO type=lvalue)
  ;

lvalue_indices
                                {Type *type;}
  : #(T_SUBSCRITO
      (
        type=expressao          {evalMatrixSubscript(type);}
      )+
    )
  ;

chamada_subrotina returns [Type *type]

                                  {
                                    TypeList params;
                                  }

  : #(T_CALL id:T_IDENTIFICADOR params=lista_argumentos)

                                  {
                                    type = getSymbolType(id->getText());
                                    evalFCall(id->getText(), params);
                                  }
  ;


lista_argumentos returns [TypeList list]

                                   {Type *type;}
  : (
      type=expressao               {list.push_back(type);}
    )*
  ;

literal returns [Type *type]
  : T_TEXTO_LITERAL      {type = getType(T_LITERAL);}
  | T_INTEIRO_LITERAL    {type = getType(T_INTEIRO);}
  | T_REAL_LITERAL       {type = getType(T_REAL);}
  | T_CARACTERE_LITERAL  {type = getType(T_CARACTERE);}
  | T_VERDADEIRO         {type = getType(T_LOGICO);}
  | T_FALSO              {type = getType(T_CARACTERE);}
  | T_NULO               {type = getType(T_NULO);}
  ;
