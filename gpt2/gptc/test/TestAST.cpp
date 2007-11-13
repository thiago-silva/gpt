#include "TestAST.hpp"
#include <iostream>

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

  #include "tokenNames.hpp"

  int token = -1;
  std::map<int, std::string >::iterator it;
  for (it = tokenNames.begin(); it != tokenNames.end(); it++) {
    if (it->second == getText()) {
      token = it->first;
    }
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
		else if (t->getFirstChild()) {
			return false;
    }
	}

	if (!sibling && !t) {
		return true;
  } else if (!sibling && (t->getType() == 1)) {
    return true;
  }

	return false;
}
