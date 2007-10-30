#include "GASMFE_CMakeLabels.hpp"

CMakeLabels::CMakeLabels( const int &initialValue ) : CTemporarySymbol( "__lbl_", initialValue )
{
}


CMakeLabels::~CMakeLabels( )
{
}


string CMakeLabels::newLabel( )
{
	return getNewSymbol( );
}

