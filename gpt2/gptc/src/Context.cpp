#include "Context.hpp"

Context::Context( ) : _arity(0), lastStoredVar(NULL)//, lastInvocationType(IT_QU)
{
}


Context::~Context( )
{
}


void Context::addArg( const char *name, const int &type, const string &indexVar )
{
	//arguments.push_back( new CArgument( name, type, indexVar ) );
}


void Context::initArity( const int &value )
{
	_arity = value;
}


void Context::addArity( )
{
	_arity++;
}


int Context::getArity( )
{
	return _arity;
}

