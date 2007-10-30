#include "GASMFE_CMakeTempVars.hpp"


CMakeTempVars::CMakeTempVars( const int &initialValue )
	: CTemporarySymbol( "__tempvar_", initialValue )
{
}


CMakeTempVars::~CMakeTempVars( )
{
}


string CMakeTempVars::newVar( )
{
	return getNewSymbol( );
}

