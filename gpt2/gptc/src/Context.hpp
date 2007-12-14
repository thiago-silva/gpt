#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
using namespace std;

#include "Arguments.hpp"

class Context
{
private:
	int _arity;
public:
	Context( );
	~Context( );
	void addArg( const char *name, const int &type = 0, const string &indexVar = "" );
	void initArity( const int &value = 0 );
	void addArity( );
	int getArity( );

	char *lastStoredVar;
	string label1;
	string label2;
	string tempVar1;
	string tempVar2;
//	CArguments arguments;
//	int lastInvocationType;
//	int blackboardType;
};

#endif

