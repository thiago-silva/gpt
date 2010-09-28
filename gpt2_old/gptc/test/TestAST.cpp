#include "TestAST.hpp"
#include <iostream>
#include "TokenNames.hpp"

#include <map>
#include <string>

using namespace std;

const char* const TestAST::TYPE_NAME = "TestAST";

TestAST::TestAST()
    : CommonAST()
{

}

TestAST::TestAST( RefToken t )
    : CommonAST(t)
{
}

TestAST::TestAST( const CommonAST& other )
    : CommonAST(other)
{

}

TestAST::TestAST( const TestAST& other )
    : CommonAST(other)
{

}

TestAST::~TestAST()
{}

RefAST TestAST::clone( void ) const
{
  TestAST *ast = new TestAST( *this );
  return RefAST(ast);
}

const char* TestAST::typeName( void ) const
{
  return TestAST::TYPE_NAME;
}

void TestAST::initialize( RefToken t )
{
  CommonAST::initialize(t);
}

RefAST TestAST::factory()
{
  return RefAST(new TestAST);
}


bool TestAST::equals(RefAST other) const
{
	if (!other) {
		return false;
  }


  int token = -1;
  for (int i = 0; i < g_tokenNamesSize; i++) {
    if (g_tokenNames[i] == getText()) {
      token = i;
      break;
    }
  }
  
  if (token == -1) {
    cerr << "\n\n[TestAST::equals] ops: token não encontrado: "
         << getText() << endl << endl;
  }

  return token  == other->getType();
}

bool TestAST::equalsList(RefAST t) const
{
	// the empty tree is not a match of any non-null tree.
	if (!t) {
		return false;
  }
	// Otherwise, start walking sibling lists.  First mismatch, return false.
	RefAST sibling=this;
	for (;sibling && t;
			sibling=sibling->getNextSibling(), t=t->getNextSibling()) {
		// as a quick optimization, check roots first.
		if (!sibling->equals(t)) {
			return false;
    }
		// if roots match, do full list match test on children.
		if (sibling->getFirstChild()) {
			if (!sibling->getFirstChild()->equalsList(t->getFirstChild()))  {
				return false;
      }
		}
		// sibling has no kids, make sure t doesn't either
		else if (t->getFirstChild() && t->getFirstChild()->getType() != 1) {//1==eof
			return false;
    }
	}

	if (!sibling && !t) {
		return true;
  } else if (!sibling && (t->getType() == 1)) {//1==eof
    return true;
  }
	return false;
}
