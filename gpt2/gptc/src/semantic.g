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
  SemanticWalker::SemanticWalker(SymbolTable* symtable,
                                 const std::string& filepath)
	 : BaseSemanticWalker(symtable, filepath),
    _analisingInitializationList(false) { }

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
        (corpo)?
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
          rtype=v:valor_inicialiacao

                                {evalAttribution(ids.back(), type, rtype);}
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
                                  evalAttribution(ids.back(),type, rtype);
                                }
  ;

identificadores returns [IDList list]
  : (
      id:T_IDENTIFICADOR        {list.push_back(id);}
    )+
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
    )+
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

                                {evalAttribution(ids.back(), type, rtype);}

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
                                                    (*it)->getLine(),
                                                    (*it)->getColumn()));
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
  : rtype=expressao_

  | #(m:T_VAL_MATRIZ
        (valor_matriz[1,mtx])+)

                          {rtype = evalInitMatrix(m, mtx);}

  | #(T_VAL_ESTRUTURA
      (id:T_IDENTIFICADOR valor_estrutura[id,stc])+)

                          {rtype = evalInitStruct(stc);}
  ;

valor_matriz [int dimension, InitMatrixList& mtx]

                              {
                                Type *type;
                                InitStructList stc;
                              }

  : type=expressao_      {mtx.push_back(std::pair<int,Type*>(dimension, type));}

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
  : type=expressao_
                              {
                                stc.push_back(
                                  std::pair<RefPortugolAST,Type*>(
                                    field, type));
                              }

  | #(m:T_VAL_MATRIZ
      (valor_matriz[1, mtx])+)

                              {
                                type = evalInitMatrix(m, mtx);
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
                                          id->getLine(),
                                          id->getColumn());
                                  }

  | T_RETICENCIAS idret:T_IDENTIFICADOR

                                  {
                                    symbol = Symbol(idret->getText(),
                                              _typeBuilder->reticencesType(),
                                              _symtable->unit(),
                                              idret->getLine(),
                                              idret->getColumn());
                                  }
  ;


/* ************************* EXPRESSOES *************************/


expressao returns [ExpressionReturn ret]

                                {
                                  Type *type;
                                }

  : #(e:T_EXPRESSAO type=expr)  {ret.first = e; ret.second = type;}
  ;

expressao_ returns [Type *type]
  : #(T_EXPRESSAO type=expr)
  ;


expr returns [Type *type]
                                      {
                                        Type *l, *r;
                                        RefPortugolAST op = _t;
                                      }
  : (

      #(T_OU              l=expr r=expr)
                                      {type = evalExpr_OU(op, l,r);}

    | #(T_E               l=expr r=expr)
                                      {type = evalExpr_E(op, l,r);}

    | #(T_BIT_OU          l=expr r=expr)
                                      {type = evalExpr_BIT_OU(op, l,r);}

    | #(T_BIT_OUX         l=expr r=expr)
                                      {type = evalExpr_BIT_OUX(op, l,r);}

    | #(T_BIT_E           l=expr r=expr)
                                      {type = evalExpr_BIT_E(op, l,r);}

    | #(T_IGUAL           l=expr r=expr)
                                      {type = evalExpr_IGUAL(op, l,r);}

    | #(T_DIFERENTE       l=expr r=expr)
                                      {type = evalExpr_DIFERENTE(op, l,r);}

    | #(T_MAIOR           l=expr r=expr)
                                      {type = evalExpr_MAIOR(op, l,r);}

    | #(T_MENOR           l=expr r=expr)
                                      {type = evalExpr_MENOR(op, l,r);}

    | #(T_MAIOR_EQ        l=expr r=expr)
                                      {type = evalExpr_MAIOR_EQ(op, l,r);}

    | #(T_MENOR_EQ        l=expr r=expr)
                                      {type = evalExpr_MENOR_EQ(op, l,r);}

    | #(T_BIT_SHIFT_LEFT  l=expr r=expr)
                                      {type = evalExpr_BIT_SHIFT_LEFT(op, l,r);}

    | #(T_BIT_SHIFT_RIGHT l=expr r=expr)
                                      {type = evalExpr_BIT_SHIFT_RIGHT(op, l,r);}

    | #(T_MAIS            l=expr r=expr)
                                      {type = evalExpr_MAIS(op, l,r);}

    | #(T_MENOS           l=expr r=expr)
                                      {type = evalExpr_MENOS(op, l,r);}

    | #(T_DIV             l=expr r=expr)
                                      {type = evalExpr_DIV(op, l,r);}

    | #(T_MULTIP          l=expr r=expr)
                                      {type = evalExpr_MULTIP(op, l,r);}

    | #(T_MOD             l=expr r=expr)
                                      {type = evalExpr_MOD(op, l,r);}

    | #(T_UN_NEGATIVO     l=elemento)
                                      {type = evalExpr_UN_NEGATIVO(op,l);}

    | #(T_UN_POSITIVO     l=elemento)
                                      {type = evalExpr_UN_POSITIVO(op,l);}

    | #(T_NAO             l=elemento)
                                      {type = evalExpr_NAO(op,l);}

    | #(T_BIT_NAO         l=elemento)
                                      {type = evalExpr_BIT_NAO(op,l);}

    )
                       {op->setEvalType(type);}

  | type=elemento
  ;


elemento returns [Type *type]
                               {ExpressionReturn lv;}
  : type=literal
  | lv=lvalue                  {type = lv.second;}
  | type=chamada_subrotina
  | type=expressao_
  ;

literal returns [Type *type]
  : T_TEXTO_LITERAL         {type = _typeBuilder->primitiveType(T_LITERAL);}
  | T_INTEIRO_LITERAL       {type = _typeBuilder->primitiveType(T_INTEIRO);}
  | T_REAL_LITERAL          {type = _typeBuilder->primitiveType(T_REAL);}
  | T_CARACTERE_LITERAL     {type = _typeBuilder->primitiveType(T_CARACTERE);}
  | T_VERDADEIRO            {type = _typeBuilder->primitiveType(T_LOGICO);}
  | T_FALSO                 {type = _typeBuilder->primitiveType(T_LOGICO);}
  ;


lvalue returns [ExpressionReturn ret]

                            {
                              Type *type;
                              int dimensions;
                            }

  : #(id:T_IDENTIFICADOR    {type = getSymbolType(id);}

      (
        dimensions=lvalue_indices

                           {type = evalMatrixSubscript(id, type, dimensions);}
      )?

      (type=lvalue_membro[id,type])?
    )

      {ret.first = id; ret.second = type;}
  ;


lvalue_membro [RefPortugolAST parent, Type *sttype] returns [Type *type]

  : #(T_MEMBRO type=lvalue_struct[parent,sttype])
  ;

lvalue_struct[RefPortugolAST parent, Type *sttype] returns [Type *type]

                              {
                                int dimensions;
                              }

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
                                  ExpressionReturn ex;
                                  dimensions = 0;
                                }
  : #(s:T_SUBSCRITO
      (
        ex=expressao
                                {
                                  dimensions++;
                                  evalMatrixSubscriptType(ex.first, ex.second);
                                }
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
      type=expressao_                   {list.push_back(type);}
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
                 (T_VARIAVEL|T_CONSTANTE)*
                 bloco_codigo            {_symtable->setGlobalScope();}
    )

  | #(T_PROCEDIMENTO p:T_IDENTIFICADOR
                 params=lista_parametros {setCurrentScope(p, params);}
                 (T_VARIAVEL|T_CONSTANTE)*
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
  | T_SAIR //TODO: deve estar dentro de uma iteração
  | devnull=chamada_subrotina
  ;

en_atribuicao
                        {ExpressionReturn lret, rret;}

  : #(at:T_ATRIBUICAO lret=lvalue rret=expressao)

                        {evalAttribution(at, lret.second, rret.second);}
  ;

en_retorne
                        {Type *type;}

  : #(ret:T_RETORNE type=expressao_)

                        {evalRetorne(ret, type);}
  ;

en_se
                        {ExpressionReturn ex;}

  : #(se:T_SE

      ex=expressao    {evalCondicional(ex);}
      lista_enunciados
      (T_SENAO lista_enunciados)?
    )
  ;

en_enquanto
                        {ExpressionReturn ex;}
  : #(enq:T_ENQUANTO
      ex=expressao      {evalCondicional(ex);}
      lista_enunciados
    )
  ;

en_repita
                              {ExpressionReturn ex;}
  : #(T_REPITA
      lista_enunciados
      T_ATE ex=expressao      {evalCondicional(ex);}
    )
  ;

en_para
                              {ExpressionReturn lv, from, to;}

  : #(p:T_PARA lv=lvalue
        from=expressao        {evalAttribution(lv, from);}
        to=expressao          {evalAttribution(lv, to);}
        (T_PASSO)?
        lista_enunciados)
  ;

  //TODO: checar por opções repetidas nos testes

en_caso
                              {Type *type;}

  : #(c:T_CASO type=expressao_ (teste_caso[type])+ (caso_senao)?)
  ;

teste_caso[Type *ltype]

                               {Type *rtype;}

  : #(T_FACA
      rtype=lit:literal            {evalExpr_IGUAL(lit, ltype, rtype);}
      lista_enunciados
    )
  ;

caso_senao
  : #(T_SENAO lista_enunciados)
  ;
