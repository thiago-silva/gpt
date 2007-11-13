#ifndef TESTAST_HPP
#define TESTAST_HPP

#include <antlr/CommonAST.hpp>
#include <string>

using namespace std;
using namespace antlr
;
class TestAST : public CommonAST {
public:
  TestAST();
  TestAST(RefToken t );
  TestAST(const CommonAST& other);
  TestAST(const TestAST& other);

  ~TestAST();

  virtual RefAST clone( void ) const;

  virtual void initialize( RefToken t );

  virtual const char* typeName( void ) const;

  virtual bool equals(RefAST t) const;
  virtual bool equalsList(RefAST t) const;

  static RefAST factory();
  static const char* const TYPE_NAME;
};

typedef ASTRefCount<TestAST> RefTestAST;

#endif
