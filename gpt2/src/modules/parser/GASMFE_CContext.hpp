#ifndef GASMFE_CCONTEXT_H
#define GASMFE_CCONTEXT_H

#include <string>
using namespace std;

#include "GASMFE_CArguments.hpp"

class CContext
{
private:
	int _arity;
public:
	CContext( );
	~CContext( );
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
	int lastInvocationType;
	int blackboardType;
};

#endif

