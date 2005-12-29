#ifndef SEMANTICEVAL_HPP
#define SEMANTICEVAL_HPP

#include "SymbolTable.hpp"
#include "PortugolAST.hpp"

#include <string>
#include <map>
#include <list>

//---------- helpers ------------

class ExpressionValue {
public:
  ExpressionValue();
  ExpressionValue(int type);

  void setPrimitiveType(int type);
  int primitiveType() const;
  
  void setPrimitive(bool);
  bool isPrimitive() const;

  void setDimensions(const list<int>&);
  list<int>& dimensions();
  
  bool isNumeric(bool integerOnly = false) const;
  bool isCompatibleWidth(ExpressionValue& other) const;
  bool isCompatibleWidth(SymbolType& other) const;

  string toString() const;

  void set(SymbolType&);

  
protected:
  bool matchesType(bool other_isprimitive) const;
  bool matchesDimensions(list<int>& other_dimensions) const;
  bool matchesPrimitiveType(int other_type) const;

  bool _isPrimitive;
  int _primitiveType;
  list<int> _dimensions; //conjunto/matriz
};


class Funcao {
public:
  void setId(RefPortugolAST t) {
    id = t;
  }
  void addParams(const pair<int, list<RefPortugolAST> >& p) {
    prim_params.push_back(p);
  }

  void addParams(const pair< pair<int, list<int> >, list<RefPortugolAST> >& m) {
    mt_params.push_back(m);
  }

//   void setReturnType(pair<int, list<int> > type) {
//     return_type.setPrimitive(false);
//     return_type.setPrimitiveType(type.first);
//     return_type.setDimensions(type.second);
//   }

  void setReturnType(int type) {
    return_type.setPrimitiveType(type);
  }


  RefPortugolAST id;
  SymbolType return_type;

  list< pair<int, list<RefPortugolAST> > > prim_params;
  list< pair< pair<int, list<int> >, list<RefPortugolAST> > > mt_params;

};

//---------------------------------------------------------------------//

class SemanticEval {
public:
  SemanticEval(SymbolTable& st);

  SymbolTable& getSymbolTable();

  void setCurrentScope(const string&);

  void declareVars(pair<int, list<RefPortugolAST> >& prims);
  void declareVars(pair< pair<int, list<int> >, list<RefPortugolAST> >& ms);
 
  void evaluateAttribution(ExpressionValue&  lv, ExpressionValue& rv, int line);

  ExpressionValue  evaluateLValue(RefPortugolAST id, list<ExpressionValue>& dim);

  void evaluateBooleanExpr(ExpressionValue& ev, int line);
  void evaluateNumericExpr(ExpressionValue& ev, int line);  
  ExpressionValue evaluateExpr(ExpressionValue& left, ExpressionValue& right, RefPortugolAST op);
  ExpressionValue evaluateExpr(ExpressionValue& ev, RefPortugolAST unary_op);  

  void evaluateReturnCmd(ExpressionValue& ev, int line);

  void declareFunction(Funcao& f);
  ExpressionValue  evaluateFCall(RefPortugolAST f, list<ExpressionValue>& args);  
  void evaluateAllFCalls();
  
  void evaluatePasso(int line, const string& str);
protected:

  bool evalVariableRedeclaration(const string& scope, RefPortugolAST id);

  ExpressionValue evaluateNumTypes(ExpressionValue& left, ExpressionValue& right);  

  SymbolTable& stable;
  string currentScope;
  list<pair<RefPortugolAST,list<ExpressionValue> > > fcallsList;
};

#endif
