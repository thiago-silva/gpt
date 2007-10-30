#ifndef GASMFE_CMAKE_LABELS_H
#define GASMFE_CMAKE_LABELS_H

#include <string>
using namespace std;

#include "CTemporarySymbol.hpp"

class CMakeLabels : private CTemporarySymbol
{
public:
	CMakeLabels( const int &initialValue = 1 );
	~CMakeLabels( );
	string newLabel( );
};

#endif

