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
}

{
public:
  SemanticWalker::SemanticWalker(SymbolTable* stable)
	 : BaseSemanticWalker(stable), _analisingInitializationList(false) { }

private:
  bool _analisingInitializationList;
}


programa
                                      {RefPortugolAST inicio;}
  : #(T_ALGORITMO

        (importacao)*

        (declaracoes)?

                                      //recuperando declaracoes globais e
                                      //de subprogramas para a symtable
                                      {inicio = _t; /*backup da posicao*/}
        (declaracoes_subprogramas)?

        EOF
                                      //analise semantica dos blocos
                                      {_t = inicio; /*rollback*/}
        corpo
        EOF
     )
  ;

importacao
  : #(T_USE lib:T_TEXTO_LITERAL)      {useLib(lib->getText());}
  ;


/*********************** DECLARACOES *******************************/

declaracoes
  : declaracao_variavel    (declaracoes)?
  | declaracao_constante   (declaracoes)?
  | declaracao_estrutura   (declaracoes)?
  ;

declaracao_variavel
                                {
                                  IDList ids;
                                  Type *type;
                                  Type *rtype;
                                }

  : #(T_VARIAVEL
        type=tipo
        ids=identificadores     {declare(ids, type);}

        (
          rtype=valor_inicialiacao

                                {evalAttribution(
                                    ids.back()->getLine(), type, rtype);}
        )?
    )
  ;

declaracao_constante
                                {
                                  IDList ids;
                                  Type *type;
                                  Type *rtype;                                  
                                }

  : #(T_CONSTANTE
        type=tipo
        ids=identificadores
        rtype=valor_inicialiacao)

                                {
                                  type->setConst(true);
                                  declare(ids, type);
                                  evalAttribution(
                                      ids.back()->getLine(), type, rtype);
                                }
  ;

identificadores returns [IDList list]
  : (
      id:T_IDENTIFICADOR        {list.push_back(id);}
    )*
  ;

tipo returns [Type *type]
  : id:T_IDENTIFICADOR          {type = getStructType(id);}
  | type=tipo_primitivo
  | type=tipo_matriz
  ;

tipo_primitivo returns [Type *type]

                      {RefPortugolAST prim = _t;}
  : (
        T_INTEIRO
      | T_REAL
      | T_CARACTERE
      | T_LITERAL
      | T_LOGICO
      | T_CORINGA
    )
                      {type = _typeBuilder->primitiveType(prim->getType());}
  ;

tipo_matriz returns [Type *type]

                        {
                          std::list<int> dims;
                          Type* ofType;
                        }

  : #(T_MATRIZ ofType=tipo dims=dimensoes)

                  {type =_typeBuilder->matrixType(ofType,dims);}

  ;

dimensoes returns [std::list<int> dims]

                        {int dsize;}
  : (
      dsize=dimensao
                        {dims.push_back(dsize);}
    )*
  ;

dimensao returns [int size]
  : i:T_INTEIRO_LITERAL     {size = atoi(i->getText().c_str());}
  | n:T_NULO                {size = 0;}
  ;




declaracao_estrutura
                                {SymbolList fields;}

  : #(T_ESTRUTURA id:T_IDENTIFICADOR fields=campos_estrutura)

                                {declareStruct(id, fields);}
  ;

campos_estrutura returns [SymbolList fields]

                                {
                                  IDList ids;
                                  Type *type;
                                  Type *rtype;                                  
                                }

  : (
      #(T_VARIAVEL type=tipo ids=identificadores
            (
              rtype=valor_inicialiacao

                                {evalAttribution(
                                  ids.back()->getLine(), type, rtype);}

            )?
    )

                                {
                                  for (IDList::iterator it = ids.begin();
                                         it != ids.end(); ++it) {
                                    fields.push_back(
                                      Symbol((*it)->getText(),
                                                    type,
                                                    _symtable->currentScope(),
                                                    _symtable->unit(),
                                                    (*it)->getLine()));
                                  }
                                }
    )+
  ;


/********************** INICIALIZACAO *************************/


valor_inicialiacao returns [Type *rtype]

                          {
                            _analisingInitializationList = true;
                          }

  : #(v:T_VALOR rtype=valor)

                          {
                            _analisingInitializationList = false;
                          }
  ;

valor returns [Type *rtype]
                          {
                            InitMatrixList         mtx;
                            InitStructList         stc;
                          }
  : rtype=expressao

  | #(m:T_VAL_MATRIZ
        (valor_matriz[1,mtx])+)

                          {rtype = evalInitMatrix(m->getLine(), mtx);}

  | #(T_VAL_ESTRUTURA
      (id:T_IDENTIFICADOR valor_estrutura[id,stc])+)

                          {rtype = evalInitStruct(stc);}
  ;

valor_matriz [int dimension, InitMatrixList& mtx]

                              {
                                Type *type;
                                InitStructList stc;
                              }

  : type=expressao     {mtx.push_back(std::pair<int,Type*>(dimension, type));}

  | #(T_VAL_MATRIZ
      (valor_matriz[dimension+1, mtx])+)

  | #(T_VAL_ESTRUTURA
      (id:T_IDENTIFICADOR valor_estrutura[id, stc])+)

                       {mtx.push_back(std::pair<int,Type*>(dimension,
                            evalInitStruct(stc)));}
  ;

valor_estrutura [RefPortugolAST field, InitStructList& stc]
                              {
                                Type *type;
                                InitMatrixList mtx;
                                InitStructList stc_;
                              }
  : type=expressao
                              {
                                stc.push_back(
                                  std::pair<RefPortugolAST,Type*>(
                                    field, type));
                              }

  | #(m:T_VAL_MATRIZ
      (valor_matriz[1, mtx])+)

                              {
                                type = evalInitMatrix(m->getLine(), mtx);
                                stc.push_back(
                                  std::pair<RefPortugolAST,Type*>(
                                    field, type));
                              }

  | #(T_VAL_ESTRUTURA
      (id:T_IDENTIFICADOR valor_estrutura[id,stc_])+)

                              {
                                type = evalInitStruct(stc_);
                                stc.push_back(
                                  std::pair<RefPortugolAST,Type*>(
                                    field, type));
                              }
  ;




/********************* DECLARACAO SUBPROGRAMAS ****************************/

declaracoes_subprogramas
  : declaracao_subprograma (declaracoes_subprogramas)?
  | T_INICIO               (declaracoes_subprogramas)?//pula o bloco principal
  ;

declaracao_subprograma
  : declaracao_funcao
  | declaracao_procedimento
  ;

declaracao_procedimento
                                      {SymbolList params;}

  : #(T_PROCEDIMENTO
        id:T_IDENTIFICADOR
        params=lista_parametros

                                      {
                                        declareProc(id, params);
                                      }

      (declaracao_variavel | declaracao_constante)*)

                                      {_symtable->setGlobalScope();}
  ;

declaracao_funcao
                                      {
                                        SymbolList params;
                                        Type *type;
                                      }
  : #(T_FUNCAO
        id:T_IDENTIFICADOR
        params=lista_parametros
        type=tipo_retorno
                                      {
                                        declareProc(id, params, type);
                                      }

      (declaracao_variavel | declaracao_constante)*)

                                      {_symtable->setGlobalScope();}
  ;

tipo_retorno returns [Type* type]
  : #(T_TIPO_RETORNO type=tipo)
  ;

lista_parametros returns [SymbolList list]

                                 {Symbol symbol;}

  : (#(T_PARAM symbol=parametro) {list.push_back(symbol);})*
  ;

parametro returns [Symbol symbol]

                                  {Type* type;}

  : type=tipo id:T_IDENTIFICADOR (r:T_REF)? (c:T_CONSTANTE)?

                                  {
                                    if (c != antlr::nullAST) {
                                      type->setConst(true);
                                    }

                                    if (r != antlr::nullAST) {
                                      type->setRef(true);
                                    }

                                    symbol = 
                                        Symbol(id->getText(),
                                          type,
                                          _symtable->unit(),
                                          id->getLine());
                                  }

  | T_RETICENCIAS idret:T_IDENTIFICADOR

                                  {
                                    symbol = Symbol(idret->getText(),
                                              _typeBuilder->reticencesType(),
                                              _symtable->unit(),
                                              idret->getLine());
                                  }
  ;


/* ************************* EXPRESSOES *************************/



expressao returns [Type *type]
                                      {
                                        Type *l, *r;
                                        RefPortugolAST op = _t;
                                      }
  : (

      #(T_OU              l=expressao r=expressao)
                                      {type = evalExpr_OU(op->getLine(), l,r);}

    | #(T_E               l=expressao r=expressao)
                                      {type = evalExpr_E(op->getLine(), l,r);}

    | #(T_BIT_OU          l=expressao r=expressao)
                                      {type = evalExpr_BIT_OU(op->getLine(), l,r);}

    | #(T_BIT_OUX         l=expressao r=expressao)
                                      {type = evalExpr_BIT_OUX(op->getLine(), l,r);}

    | #(T_BIT_E           l=expressao r=expressao)
                                      {type = evalExpr_BIT_E(op->getLine(), l,r);}

    | #(T_IGUAL           l=expressao r=expressao)
                                      {type = evalExpr_IGUAL(op->getLine(), l,r);}

    | #(T_DIFERENTE       l=expressao r=expressao)
                                      {type = evalExpr_DIFERENTE(op->getLine(), l,r);}

    | #(T_MAIOR           l=expressao r=expressao)
                                      {type = evalExpr_MAIOR(op->getLine(), l,r);}

    | #(T_MENOR           l=expressao r=expressao)
                                      {type = evalExpr_MENOR(op->getLine(), l,r);}

    | #(T_MAIOR_EQ        l=expressao r=expressao)
                                      {type = evalExpr_MAIOR_EQ(op->getLine(), l,r);}

    | #(T_MENOR_EQ        l=expressao r=expressao)
                                      {type = evalExpr_MENOR_EQ(op->getLine(), l,r);}

    | #(T_BIT_SHIFT_LEFT  l=expressao r=expressao)
                                      {type = evalExpr_BIT_SHIFT_LEFT(op->getLine(), l,r);}

    | #(T_BIT_SHIFT_RIGHT l=expressao r=expressao)
                                      {type = evalExpr_BIT_SHIFT_RIGHT(op->getLine(), l,r);}

    | #(T_MAIS            l=expressao r=expressao)
                                      {type = evalExpr_MAIS(op->getLine(), l,r);}

    | #(T_MENOS           l=expressao r=expressao)
                                      {type = evalExpr_MENOS(op->getLine(), l,r);}

    | #(T_DIV             l=expressao r=expressao)
                                      {type = evalExpr_DIV(op->getLine(), l,r);}

    | #(T_MULTIP          l=expressao r=expressao)
                                      {type = evalExpr_MULTIP(op->getLine(), l,r);}

    | #(T_MOD             l=expressao r=expressao)
                                      {type = evalExpr_MOD(op->getLine(), l,r);}

    | #(T_UN_NEGATIVO     l=elemento)
                                      {type = evalExpr_UN_NEGATIVO(op->getLine(),l);}

    | #(T_UN_POSITIVO     l=elemento)
                                      {type = evalExpr_UN_POSITIVO(op->getLine(),l);}

    | #(T_NAO             l=elemento)
                                      {type = evalExpr_NAO(op->getLine(),l);}

    | #(T_BIT_NAO         l=elemento)
                                      {type = evalExpr_BIT_NAO(op->getLine(),l);}

    )
                       {op->setEvalType(type);}

  | type=elemento
  ;


elemento returns [Type *type]
  : type=literal
  | type=lvalue
  | type=chamada_subrotina
  | #(T_ABRE_PAREN type=expressao)
  ;

literal returns [Type *type]
  : T_TEXTO_LITERAL         {type = _typeBuilder->primitiveType(T_LITERAL);}
  | T_INTEIRO_LITERAL       {type = _typeBuilder->primitiveType(T_INTEIRO);}
  | T_REAL_LITERAL          {type = _typeBuilder->primitiveType(T_REAL);}
  | T_CARACTERE_LITERAL     {type = _typeBuilder->primitiveType(T_CARACTERE);}
  | T_VERDADEIRO            {type = _typeBuilder->primitiveType(T_LOGICO);}
  | T_FALSO                 {type = _typeBuilder->primitiveType(T_LOGICO);}
  ;


lvalue returns [Type *type]

                            {int dimensions;}

  : #(id:T_IDENTIFICADOR    {type = getSymbolType(id);}

      (
        dimensions=lvalue_indices

                           {type = evalMatrixSubscript(id,
                                                       type, dimensions);}
      )?

      (type=lvalue_membro[id,type])?
    )
  ;


lvalue_membro [RefPortugolAST parent, Type *sttype] returns [Type *type]

  : #(T_MEMBRO type=lvalue_struct[parent,sttype])
  ;

lvalue_struct[RefPortugolAST parent, Type *sttype] returns [Type *type]

                             {int dimensions;}

  : #(id:T_IDENTIFICADOR     {type = getSymbolType(parent,sttype,id);}

      (
        dimensions=lvalue_indices

                             {type = evalMatrixSubscript(id,
                                                         type, dimensions);}
      )?

      (type=lvalue_membro[id,type])?
    )
  ;

lvalue_indices returns [int dimensions]

                                {
                                  Type *type;
                                  dimensions = 0;
                                }
  : #(T_SUBSCRITO
      (
        type=expressao          {dimensions++;}
      )+
    )
  ;




chamada_subrotina returns [Type *type]

                                  {
                                    TypeList params;
                                  }

  : #(T_CALL id:T_IDENTIFICADOR params=lista_argumentos)

                                  {
                                    if (!_analisingInitializationList) {
                                      type = evalCall(id, params);
                                    } else {
                                      type = _typeBuilder->errorType();
                                      report(id->getLine(),
                                        "ilegal: fcall em lista de inicialização");
                                    }
                                  }
  ;

lista_argumentos returns [TypeList list]

                                   {Type *type;}
  : (
      type=expressao               {list.push_back(type);}
    )*
  ;




/************************** CORPO *************************/

corpo
  :  corpo_subprograma      (corpo)?
  |  bloco_codigo           (corpo_subprograma)*
  ;


corpo_subprograma
                      {SymbolList params;}

  : #(T_FUNCAO   f:T_IDENTIFICADOR
                 params=lista_parametros {setCurrentScope(f, params);}
                 T_TIPO_RETORNO
                 bloco_codigo            {_symtable->setGlobalScope();}
    )

  | #(T_PROCEDIMENTO p:T_IDENTIFICADOR
                 params=lista_parametros {setCurrentScope(p, params);}
                 bloco_codigo)           {_symtable->setGlobalScope();}
  ;



bloco_codigo
  : #(T_INICIO lista_enunciados)
  ;

lista_enunciados
  : (enunciado)*
  ;

enunciado
                    {Type *devnull;}
  : en_atribuicao
  | en_retorne
  | en_se
  | en_enquanto
  | en_repita
  | en_para
  | en_caso
  | T_SAIR                                
  | devnull=chamada_subrotina
  ;

en_atribuicao
                        {Type *ltype, *rtype;}

  : #(at:T_ATRIBUICAO ltype=lvalue rtype=expressao)

                        {evalAttribution(at->getLine(), ltype, rtype);}
  ;

en_retorne
                        {Type *type;}

  : #(ret:T_RETORNE type=expressao) 

                        {evalRetorne(ret->getLine(), type);}
  ;

en_se
                        {Type *type;}

  : #(se:T_SE 

      type=expressao    {evalCondicional(se->getLine(),type);}
      lista_enunciados 
      (T_SENAO lista_enunciados)?
    )
  ;

en_enquanto
                        {Type *type;}
  : #(enq:T_ENQUANTO 
      type=expressao    {evalCondicional(enq->getLine(), type);}
      lista_enunciados
    )
  ;

en_repita
                              {Type *type;}
  : #(T_REPITA      
      lista_enunciados
      a:T_ATE type=expressao    {evalCondicional(a->getLine(), type);}
    )
  ;

en_para
                              {Type *ltype, *from, *to;}

  : #(p:T_PARA ltype=lvalue 
        from=expressao        {evalAttribution(p->getLine(), ltype, from);}
        to=expressao          {evalAttribution(p->getLine(), ltype, to);}
        T_PASSO
        lista_enunciados)
  ;

  //TODO: checar por opções repetidas nos testes

en_caso
                              {Type *type;}

  : #(c:T_CASO type=expressao (teste_caso[c->getLine(), type])+ (caso_senao)?)
  ;

teste_caso[int line, Type* ltype]
                             {Type *rtype;}

  : #(T_FACA 
      rtype=literal            {evalExpr_IGUAL(line, ltype, rtype);}
      lista_enunciados
    )
  ;

caso_senao
  : #(T_SENAO lista_enunciados)
  ;
