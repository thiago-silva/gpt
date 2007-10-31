#include "GASMFE_CContext.hpp"

CContext::CContext( ) : _arity(0), lastStoredVar(NULL)//, lastInvocationType(IT_QU)
{
}


CContext::~CContext( )
{
}


void CContext::addArg( const char *name, const int &type, const string &indexVar )
{
	//arguments.push_back( new CArgument( name, type, indexVar ) );
}


void CContext::initArity( const int &value )
{
	_arity = value;
}


void CContext::addArity( )
{
	_arity++;
}


int CContext::getArity( )
{
	return _arity;
}

