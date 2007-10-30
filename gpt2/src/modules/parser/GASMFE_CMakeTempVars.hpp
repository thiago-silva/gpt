#ifndef GASMFE_CMAKE_TEMP_VARS_H
#define GASMFE_CMAKE_TEMP_VARS_H

#include <string>
using namespace std;

#include "CTemporarySymbol.hpp"

class CMakeTempVars : private CTemporarySymbol
{
public:
	CMakeTempVars( const int &initialValue = 1 );
	~CMakeTempVars( );
	string newVar( );
};

#endif

