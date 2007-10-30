#include <Tools.hpp>
#include "CTemporarySymbol.hpp"

CTemporarySymbol::CTemporarySymbol( )
{
}

CTemporarySymbol::CTemporarySymbol( const string &baseName, const int &initialValue)
	: _baseName(baseName), _count(initialValue)
{
}

CTemporarySymbol::~CTemporarySymbol( )
{
}

void CTemporarySymbol::setBaseName( string baseName )
{
	_baseName = baseName;
}

void CTemporarySymbol::setInitialValue( int initialValue )
{
	_count = initialValue;
}

string CTemporarySymbol::getNewSymbol( )
{
	return( _baseName + itoa( _count++ ) );
}

void CTemporarySymbol::removeLastSymbol( )
{
	_count--;
}

unsigned int CTemporarySymbol::getMaxSymbolsUsed( )
{
	return( _count );
}

string CTemporarySymbol::getLastSymbol( )
{
	return( _baseName + itoa( _count - 1 ) );
}

