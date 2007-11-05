#include <Tools.hpp>

#include "CTemporarySymbol.hpp"

CTemporarySymbol::CTemporarySymbol( )
	: _baseName("_ts"), _count(1)
{
}

CTemporarySymbol::CTemporarySymbol( const string &baseName, const int &nextValue)
	: _baseName(baseName), _count(nextValue)
{
}

CTemporarySymbol::~CTemporarySymbol( )
{
}

void CTemporarySymbol::setBaseName( string baseName )
{
	_baseName = baseName;
}

void CTemporarySymbol::setNextValue( int nextValue )
{
	_count = nextValue;
}

string CTemporarySymbol::getNew( )
{
	_last = _baseName + itoa( _count++ );

	return _last;
}

void CTemporarySymbol::removeLast( )
{
	_count--;
}

unsigned int CTemporarySymbol::getMaxUsed( )
{
	return( _count );
}

string CTemporarySymbol::getLast( )
{
	return( _last );
}

