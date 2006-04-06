/***************************************************************************
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
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


#include "SemanticEval.hpp"

#include "GPTDisplay.hpp"
#include "SemanticWalkerTokenTypes.hpp"

#include <sstream>

ExpressionValue::ExpressionValue()
  : _isPrimitive(true)
  , _primitiveType(TIPO_NULO)
  {}

ExpressionValue::ExpressionValue(int type)
  : _isPrimitive(true), _primitiveType(type)
  {
}

void ExpressionValue::setPrimitiveType(int type) {
  _primitiveType=type;
  _isPrimitive=true;
}

int ExpressionValue::primitiveType() const {
  return _primitiveType;
}

void ExpressionValue::setPrimitive(bool val) {
  _isPrimitive = val;
}

bool ExpressionValue::isPrimitive() const {
  return _isPrimitive;
}

void ExpressionValue::setDimensions(const list<int>& d) {
  _isPrimitive = false;
  _dimensions = d;
}

list<int>& ExpressionValue::dimensions() {
  return _dimensions;
}

bool ExpressionValue::isNumeric(bool integerOnly) const {
  if(!_isPrimitive) {
    return false;
  }

  if(integerOnly) {
    switch(_primitiveType) {
      case TIPO_CARACTERE:
      case TIPO_INTEIRO:
      case TIPO_LOGICO:
      case TIPO_ALL:
        return true;
      case TIPO_REAL:
      case TIPO_LITERAL:
      case TIPO_NULO:
      default:
        return false;
    }
  } else {
    switch(_primitiveType) {
      case TIPO_CARACTERE:
      case TIPO_INTEIRO:
      case TIPO_LOGICO:
      case TIPO_REAL:
      case TIPO_ALL:
        return true;
      case TIPO_LITERAL:
      case TIPO_NULO:
      default:
        return false;
    }
  }
}
bool ExpressionValue::isCompatibleWidth(ExpressionValue& other) const {
  if(!matchesType(other._isPrimitive)) {
    return false;
  } else if(!matchesDimensions(other._dimensions)) {
    return false;
  } else if(!matchesPrimitiveType(other._primitiveType)) {
    return false;
  }
  return true;
}

bool ExpressionValue::isCompatibleWidth(SymbolType& other) const {
  if(!matchesType(other.isPrimitive())) {
    return false;
  } else if(!matchesDimensions(other.dimensions())) {
    return false;
  } else if(!matchesPrimitiveType(other.primitiveType())) {
    return false;
  }
  return true;
}

bool ExpressionValue::matchesType(bool other_isprimitive) const {
  return _isPrimitive == other_isprimitive;
}

bool ExpressionValue::matchesDimensions(list<int>& other_dimensions) const {
  //numero de dimensoes
  if(_dimensions.size() != other_dimensions.size()) {
    return false;
  }

  //range das dimensoes
  list<int>::const_iterator lit = _dimensions.begin();
  list<int>::const_iterator rit = other_dimensions.begin();
  while((lit != _dimensions.end()) && (rit != other_dimensions.end())) {
    if((*lit) != (*rit)) {
      return false;
    }
    ++lit;
    ++rit;
  }
  return true;
}

bool ExpressionValue::matchesPrimitiveType(int other_type) const {
//1: todos os tipos numericos (inteiro, real, logico, caractere) sao compativeis entre si
//2: tipos nulos são compativeis apenas com tipo nulo
//3: tipo_all eh compativel com todos

  //para funcoes sem retorno
  if(_primitiveType == TIPO_NULO) {
    return _primitiveType == other_type;
  }

  //rvalue retorna qualquer tipo
  if(other_type == TIPO_ALL) {
    return true;
  }


  if(_primitiveType == TIPO_LITERAL) {
    if(other_type == TIPO_LITERAL) {
      return true; //ambos literal, ok
    } else {
      return false; //um eh literal, o outro nao...incompativeis!
    }
  } else if(other_type == TIPO_LITERAL) {
    return false; //um eh literal, o outro nao...incompativeis!
  } else {
    return true; //ambos são numericos (int,real,logico,caractere) ok!
  }
}

void ExpressionValue::set(SymbolType& s) {
  _isPrimitive  = s.isPrimitive();
  _primitiveType = s.primitiveType();
  _dimensions = s.dimensions();
}

string ExpressionValue::toString() const {
  string str;
  str = Symbol::typeToString(_primitiveType);
  if(!_isPrimitive) {
    list<int>::const_iterator it;
    for(it = _dimensions.begin(); it != _dimensions.end(); ++it) {
      str += "[";
//       str += *it;
      str += "]";
    }
  }
  return str;
}

void ExpressionValue::setID(const string& id)
{
  _id = id;
}

string ExpressionValue::id()
{
  return _id;
}

//******************************************************************************************//


SemanticEval::SemanticEval(SymbolTable& st)
  : stable(st)
{
}

SymbolTable& SemanticEval::getSymbolTable() {
  return stable;
}

void SemanticEval::setCurrentScope(const string& sc) {
  currentScope = sc;
}

void SemanticEval::declareVar(int type, RefPortugolAST prim) {
	if(!evalVariableRedeclaration(currentScope, prim)) {
		stable.declareVar(currentScope, prim->getText(), prim->getLine(), type);
	}
}

void SemanticEval::declareVar(int type, list<int> dims, RefPortugolAST mt) {
  stringstream msg;
  for(list<int>::iterator itd = dims.begin(); itd != dims.end(); ++itd) {
    if((*itd) == 0) {
      msg << "Dimensões de matrizes/conjuntos não podem ter tamanho 0";
      GPTDisplay::self()->add(msg.str(), mt->getLine());
    }
  }

  if(!evalVariableRedeclaration(currentScope, mt)) {
    stable.declareVar(currentScope, mt->getText(), mt->getLine(), type, dims);
  }
}

//declaration of primitives
void SemanticEval::declareVars(pair<int, list<RefPortugolAST> >& prims) {
  list<RefPortugolAST>::iterator it;
  for(it = prims.second.begin(); it != prims.second.end(); ++it) {
    if(!evalVariableRedeclaration(currentScope, (*it))) {
      stable.declareVar(currentScope, (*it)->getText(), (*it)->getLine(), prims.first);
    }
  }
}

//declaration of matrixes
//pair< pair<type,list<dimensions> >, list<ids> >
void SemanticEval::declareVars(pair< pair<int, list<int> >, list<RefPortugolAST> >& ms) {

  stringstream msg;
  for(list<int>::iterator itd = ms.first.second.begin(); itd != ms.first.second.end(); ++itd) {
    if((*itd) == 0) {
      msg << "Dimensões de matrizes/conjuntos não podem ter tamanho 0";
      GPTDisplay::self()->add(msg.str(), (*(ms.second.begin()))->getLine());
    }
  }

  list<RefPortugolAST>::iterator it;
  for(it = ms.second.begin(); it != ms.second.end(); ++it) {
    if(!evalVariableRedeclaration(currentScope, (*it))) {
      stable.declareVar(currentScope, (*it)->getText(), (*it)->getLine(), ms.first.first, ms.first.second);
    }
  }
}

void SemanticEval::evaluateAttribution(ExpressionValue&  lv, ExpressionValue& rv, int line) {
//lvalue e rvalue devem ter tipos compativeis
//  -numericos (inteiro, logico, real, caractere) sao compativeis entre si.

  stringstream msg;

  if(!lv.isPrimitive()) {
    msg << "Faltando indicar subscrito da matriz/conjunto \"" << lv.id() << "\"";
    GPTDisplay::self()->add(msg.str(), line);
    return;
  }

  if(rv.primitiveType() == TIPO_NULO) {
    msg << "Expressão não retorna resultado para variável";
    GPTDisplay::self()->add(msg.str(), line);
  } else  if(!lv.isCompatibleWidth(rv)) {
    msg << "Variável \"" << lv.id() << "\" não pode receber valores do tipo '"
        << rv.toString() << "'";
    GPTDisplay::self()->add(msg.str(), line);
  }
}

ExpressionValue SemanticEval::evaluateLValue(RefPortugolAST id, list<ExpressionValue>& dim) {
/*
  1: lvalue (id) deve ter sido declarado no escopo global ou local
  2: o tipo do lvalue (id) deve ser o mesmo da declaracao (primitivo/dimensoes)
  3: as expressoes das dimensoes devem ser numericas inteiras. [1.2] ou ["aa"] ->erro
*/

  bool islocal;
  Symbol lvalue;
  ExpressionValue ret;
  ret.setID(id->getText());

  try {
    lvalue = stable.getSymbol(currentScope, id->getText(), true);
    if(lvalue.scope == SymbolTable::GlobalScope) {
      islocal = false;
    } else {
      islocal = true;
    }
  } catch(SymbolTableException& e) {
    stringstream msg;
    msg << "Variável \"" << id->getText() << "\" não foi declarada";
    GPTDisplay::self()->add(msg.str(), id->getLine());
    return ret;
  }

  ret.setPrimitiveType(lvalue.type.primitiveType());

  if(lvalue.isFunction) {
    stringstream msg;
    msg << "Faltando abrir parêntesis após função \"" << id->getText() << "\"";
    GPTDisplay::self()->add(msg.str(), id->getLine());
    return ret;
  }

  if(lvalue.type.isPrimitive()) {
    ret.set(lvalue.type);
    if(dim.size() > 0) {
      stringstream msg;
      msg << "Variável \"" << id->getText() << "\" não é uma matriz/conjunto";
      GPTDisplay::self()->add(msg.str(), id->getLine());
      return ret;
    } else {
      return ret;
    }
  } else {//matriz/conjunto

//     ret.setPrimitive(true);
//     ret.setPrimitiveType(lvalue.type.primitiveType());

    //checar expressoes dos subscritos
    list<ExpressionValue>::iterator it;
    for(it = dim.begin(); it != dim.end();++it) {
      if(!(*it).isNumeric(true)) {
        stringstream msg;
        msg << "Subscritos da matriz/conjunto \"" << id->getText() 
            << "\" devem ser valores numéricos inteiros ou equivalente";
        GPTDisplay::self()->add(msg.str(), id->getLine());
      }
    }

    /* Nota: checar pelas dimensoes impede que
       se passe matriz como parametros. Checar subscritos apenas
       em atribuicoes.
    */

    //checar numero de dimensoes usado
    // - So eh permitido matrizes como lvalue sem subscritos, ou com todos os seus subscritos

    if(dim.size() > 0) {
      //matriz com seus subscritos, retorna apenas o tipo primitivo
      ret.setPrimitive(true);

      if(lvalue.type.dimensions().size() != dim.size()) {
        stringstream msg;
        msg << "Matriz/conjunto \"" << id->getText() << "\" possui " << lvalue.type.dimensions().size();

        if(lvalue.type.dimensions().size() == 1) {
          msg << " dimensão";
        } else {
          msg << " dimensões";
        }
        msg << ". Use " << lvalue.type.dimensions().size() << " subscrito(s)";
        GPTDisplay::self()->add(msg.str(), id->getLine());
      }
    } else {
      //variavel matriz sem subscritos, retorna tipo matriz
      ret.setPrimitive(true);
      ret.setDimensions(lvalue.type.dimensions());
    }
    return ret;
  }
}

void SemanticEval::evaluateBooleanExpr(ExpressionValue& /*v*/, int /*line*/) {
  //qualquer coisa pode ser avaliado como expressão booleana
}

void SemanticEval::evaluateParaExpr(ExpressionValue& ev, int line, const string& term) {
  stringstream err;
  if(!ev.isPrimitive()) {    
    err << "Faltando indicar subscritos da matriz/conjunto \"" << ev.id() << "\"";
    GPTDisplay::self()->add(err.str(), line);
  } else {
    if(ev.primitiveType() != TIPO_INTEIRO) {
      err << "Expressão \"" << term << "\" deve ser do tipo inteiro";
      GPTDisplay::self()->add(err.str(), line);
    }
  } /*else if(!ev.isNumeric()) {
    stringstream err;
    err << "Esperando uma expressão numérica. Encontrado expressão \"" << ev.toString() << "\"";
    GPTDisplay::self()->add(err.str(), line);
  }*/
}

ExpressionValue SemanticEval::evaluateExpr(ExpressionValue& left, ExpressionValue& right, RefPortugolAST op) {
  //analisa expressoes binarias

  ExpressionValue ret, nulo;

  //operadores suportam apenas primitivos
//   if(!left.isPrimitive() || !right.isPrimitive()) {
//     return ret;
//   }

  //não permitir TIPO_ALL em expressões binarias
  if((left.primitiveType() == TIPO_ALL) || (right.primitiveType() == TIPO_ALL)) {
    stringstream msg;
    msg << "Função interna \"leia\" não pode participar de expressão";
    GPTDisplay::self()->add(msg.str(), op->getLine());
    return nulo;
  }

  switch(op->getType()) {
    //qualquer tipo, contanto que left e right sejam compativeis
    case SemanticWalkerTokenTypes::T_IGUAL:
    case SemanticWalkerTokenTypes::T_DIFERENTE:

    //nota sobre literais:
    // operacoes aplicadas sobre o length() do literal
    case SemanticWalkerTokenTypes::T_MAIOR:
    case SemanticWalkerTokenTypes::T_MENOR:
    case SemanticWalkerTokenTypes::T_MAIOR_EQ:
    case SemanticWalkerTokenTypes::T_MENOR_EQ:

    case SemanticWalkerTokenTypes::T_KW_OU:
    case SemanticWalkerTokenTypes::T_KW_E:
      if(left.isCompatibleWidth(right)) {
        ret.setPrimitiveType(TIPO_LOGICO);
        return ret;
      } else {
        stringstream msg;
        msg << "Operador \"" << op->getText() << "\" não pode ser usado em expressões no formato "
            << "'" << left.toString()
            << " " << op->getText()
            << " " << right.toString() << "'";
            GPTDisplay::self()->add(msg.str(), op->getLine());
            return nulo;
      }
      break;

    //qualquer numerico não-real (inteiro, caractere, lógico)
    case SemanticWalkerTokenTypes::T_BIT_OU:
    case SemanticWalkerTokenTypes::T_BIT_XOU:
    case SemanticWalkerTokenTypes::T_BIT_E:
      ret = evaluateNumTypes(left, right);
      if((ret.primitiveType() == TIPO_REAL) || (ret.primitiveType() == TIPO_NULO)) {
        stringstream msg;
        msg << "Operador \"" << op->getText() << "\" só pode ser usado com termos númericos não-reais";
            GPTDisplay::self()->add(msg.str(), op->getLine());
         return nulo;
      } else {
        return ret;
      }
      break;

    //qualquer numérico
    case SemanticWalkerTokenTypes::T_MAIS:
    case SemanticWalkerTokenTypes::T_MENOS:
    case SemanticWalkerTokenTypes::T_DIV:
    case SemanticWalkerTokenTypes::T_MULTIP:
      ret = evaluateNumTypes(left, right);
      if(!ret.isNumeric()) {
        stringstream msg;
        msg << "Operador \"" << op->getText() << "\" só pode ser usado com termos numéricos";
            GPTDisplay::self()->add(msg.str(), op->getLine());
            return nulo;
      } else {
        return ret;
      }
      break;
    case SemanticWalkerTokenTypes::T_MOD:
      ret = evaluateNumTypes(left, right);
      if(!ret.isNumeric(true)) {
        stringstream msg;
        msg << "Operador \"" << op->getText() << "\" só pode ser usado com termos "
            << "numéricos inteiros e compatíveis";
            GPTDisplay::self()->add(msg.str(), op->getLine());
            return nulo;
      } else {
        return ret;
      }
      break;
  }

  stringstream msg;
  msg << "Erro interno: operador não suportado: " << op->getText();
  GPTDisplay::self()->add(msg.str(), op->getLine());
  return nulo;
}

ExpressionValue SemanticEval::evaluateExpr(ExpressionValue& ev, RefPortugolAST unary_op) {
  ExpressionValue nulo;

  //não permitir TIPO_ALL em expressões binarias
  if(ev.primitiveType() == TIPO_ALL) {
    stringstream msg;
    msg << "Função interna \"leia\" não pode participar de expressão";
    GPTDisplay::self()->add(msg.str(), unary_op->getLine());
    return nulo;
  }

  switch(unary_op->getType()) {

    //operadores unarios para expressões numéricas (retorna o tipo da expressão 'expr')
    case SemanticWalkerTokenTypes::TI_UN_POS://+
    case SemanticWalkerTokenTypes::TI_UN_NEG://-
      if(!ev.isNumeric()) {
        stringstream msg;
        msg <<  "Operador unário \"" << unary_op->getText()
            << "\" deve ser usado em termos numéricos";
        GPTDisplay::self()->add(msg.str(), unary_op->getLine());
        return nulo;
      } else {
        return ev;
      }
      break;
    case SemanticWalkerTokenTypes::TI_UN_BNOT://~
      if(!ev.isNumeric(true)) {
        stringstream msg;
        msg <<  "Operador unário \"" << unary_op->getText()
            << "\" deve ser usado em termos numéricos inteiros e compatíveis";
        GPTDisplay::self()->add(msg.str(), unary_op->getLine());
        return nulo;
      } else {
        return ev;
      }
      break;

    //operador "not", para todos os tipos. Retorna TIPO_LOGICO
    case SemanticWalkerTokenTypes::TI_UN_NOT:
      ev.setPrimitiveType(TIPO_LOGICO);
      return ev;
      break;
  }

  stringstream msg;
  msg << "Erro interno: operador não suportado: " << unary_op->getText() << "";
  GPTDisplay::self()->add(msg.str(), unary_op->getLine());
  return nulo;
}

void SemanticEval::evaluateReturnCmd(ExpressionValue& ev, int line) {
  if(currentScope == SymbolTable::GlobalScope) {
    //tentando retornar no bloco principal
    GPTDisplay::self()->add("Bloco principal não deve ter retorno", line);
  } else {
    //currentScope eh o nome da funcao atual
    try {
      SymbolType sctype = stable.getSymbol(SymbolTable::GlobalScope, currentScope).type;

      if(!ev.isCompatibleWidth(sctype)) {
          stringstream msg;
          msg << "Expressão de retorno deve ser compatível com o tipo \""
            << sctype.toString() <<  "\"";
          GPTDisplay::self()->add(msg.str(), line);
      } //else ok!
    } catch(SymbolTableException& e) {
      cerr << "Erro interno: SemanticEval::evaluateReturnCmd exception\n";
    }
  }
}

void SemanticEval::declareFunction(Funcao& f) {
  //checa redeclaracao
  if(evalVariableRedeclaration(SymbolTable::GlobalScope, f.id)) {
    return;
  }

  Symbol sfunc(SymbolTable::GlobalScope, f.id->getText(), f.id->getLine(), true, f.return_type.primitiveType(), f.return_type.dimensions());

	list< pair<RefPortugolAST, SymbolType> >::iterator it = f.params.begin();
	for( ; it != f.params.end(); ++it) {		
		if((*it).second.isPrimitive()) {			
			sfunc.param.add((*it).first->getText(), (*it).second.primitiveType());
			declareVar((*it).second.primitiveType(), (*it).first);		
		} else {
			sfunc.param.add((*it).first->getText(),(*it).second);
			declareVar((*it).second.primitiveType(), (*it).second.dimensions(), (*it).first);
		}
	}

  stable.insertSymbol(sfunc, SymbolTable::GlobalScope);
}

// ExpressionValue SemanticEval::evaluateFCall(RefPortugolAST f, list<ExpressionValue>& args) {
//   //avaliar, apenas no final da analise, as chamadas de funcoes (quando todas elas tiverem sido declaradas)
//
//   ExpressionValue v;
//
//   //funcoes sao declaradas em escopo global
//   try {
//     SymbolType s = stable.getSymbol(SymbolTable::GlobalScope, f->getText()).type;
//     v.set(s);
//     fcallsList.push_back(pair<RefPortugolAST,list<ExpressionValue> >(f,args));
//   } catch(SymbolTableException& e) {
//       stringstream msg;
//       msg << "Função \"" << f->getText() << "\" não foi declarada";
//       GPTDisplay::self()->add(msg.str(), f->getLine());
//   }
//
//   return v;
// }

ExpressionValue SemanticEval::evaluateFCall(RefPortugolAST f, list<ExpressionValue>& args) {
  ExpressionValue v;
  Symbol s;
  try {
    s = stable.getSymbol(SymbolTable::GlobalScope, f->getText());
    v.set(s.type);
  } catch(SymbolTableException& e) {
    stringstream msg;
    msg << "Função \"" << f->getText() << "\" não foi declarada";
    GPTDisplay::self()->add(msg.str(), f->getLine());
    return v;
  }

  ParameterSig params = s.param;

  if(params.isVariable()) {
    //nao permitir 0 argumentos
    if(args.size() == 0) {
      stringstream msg;
      msg << "Pelo menos um argumento deve ser passado para a função \"" << f->getText() << "\"";
      GPTDisplay::self()->add(msg.str(), f->getLine());
      return v;
    }
    //nao permitir matrizes como argumentos de funcoes com parametros variaveis
    int count = 1;
    for(list<ExpressionValue>::iterator it = args.begin();
        it != args.end(); ++it) {
      if(!(*it).isPrimitive()) {
        stringstream msg;
        msg << "Argumento " << count;
        msg << " da função \"" << f->getText() << "\" não pode ser matriz/conjunto";
        GPTDisplay::self()->add(msg.str(), f->getLine());
        return v;
      }
      count++;
    }
    return v;
  }

  if(params.symbolList().size() != args.size()) {
    stringstream msg;
    msg << "Número de argumentos diferem do número de parâmetros da função \""
      << f->getText() << "\"";
    GPTDisplay::self()->add(msg.str(), f->getLine());
    return v;
  }

  list< pair<string,SymbolType> >::iterator pit = params.symbolList().begin();
  list< pair<string,SymbolType> >::iterator pend = params.symbolList().end();

  list<ExpressionValue>::iterator ait = args.begin();
  list<ExpressionValue>::iterator aend = args.end();

  int count = 1;
  while((pit != pend) && (ait != aend)) {
    if(!(*ait).isCompatibleWidth((*pit).second)) {
      stringstream msg;
      msg << "Argumento " << count << " da função \"" << f->getText() << "\" deve ser do tipo \""
        << (*pit).second.toString() << "\"";
      GPTDisplay::self()->add(msg.str(), f->getLine());
      return v;
    }
    ++count;
    ++pit;
    ++ait;
  }
  return v;
}


void SemanticEval::evaluatePasso(int line, const string& str) {
  if(atoi(str.c_str()) == 0) {
    stringstream msg;
    msg << "Passo com valor \"0\" não é permitido";
    GPTDisplay::self()->add(msg.str(), line);
  }
}

/************************************** Protected ***********************************/

bool SemanticEval::evalVariableRedeclaration(const string& scope, RefPortugolAST id) {

  try {
    Symbol s = stable.getSymbol(scope, id->getText());
    stringstream err;
    err << "Variável/função redeclarada: \"" << id->getText() << "\". Primeira declaração na linha "
        << s.line;
    GPTDisplay::self()->add(err.str(), id->getLine());
    return true;
  } catch(SymbolTableException& e) {
    return false;
  }
}


ExpressionValue SemanticEval::evaluateNumTypes(ExpressionValue& left, ExpressionValue& right) {
  ExpressionValue ret;

  if(!left.isNumeric() || !right.isNumeric()) return ret;

  //a ordem eh importante. Primeiro o tipo mais forte.

  if((left.primitiveType() == TIPO_REAL) || (right.primitiveType() == TIPO_REAL)) {
    ret.setPrimitiveType(TIPO_REAL); //promoted to real

  } else if((left.primitiveType() == TIPO_INTEIRO) || (right.primitiveType() == TIPO_INTEIRO)) {
    ret.setPrimitiveType(TIPO_INTEIRO); //promoted to int

  } else  if((left.primitiveType() == TIPO_CARACTERE) || (right.primitiveType() == TIPO_CARACTERE)) {
    ret.setPrimitiveType(TIPO_CARACTERE); //promoted to char

  } else if((left.primitiveType() == TIPO_LOGICO) || (right.primitiveType() == TIPO_LOGICO)) {
    ret.setPrimitiveType(TIPO_LOGICO); //promoted to bool
  }

  return ret; //no number here...
}
