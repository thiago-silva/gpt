#ifndef PORTUGOLAST_HPP
#define PORTUGOLAST_HPP

#include <antlr/CommonAST.hpp>

using namespace antlr;

class PortugolAST : public CommonAST {
public:
  PortugolAST();
  PortugolAST( RefToken t );
  PortugolAST( const CommonAST& other );
  PortugolAST( const PortugolAST& other );

  ~PortugolAST();

  void setLine(int line);
  int getLine();

  void setEvalType(int type) {
    eval_type = type;
  }
  int getEvalType() {
    return eval_type;
  }

  virtual RefAST clone( void ) const;

  virtual void initialize( RefToken t );
 
  virtual const char* typeName( void ) const;

  static RefAST factory();
  static const char* const TYPE_NAME;
protected:
  int line;
  int eval_type; //evaluated type of expression
};

typedef ASTRefCount<PortugolAST> RefPortugolAST;

#endif
