#ifndef GASMFE_CARGUMENTS_H
#define GASMFE_CARGUMENTS_H

#include <vector>
using namespace std;

#include "GASMFE_CSubroutine.hpp"

class CArguments : public vector<string> // usar list :-)
{
public:
	CArguments( );
	void init( CSubroutine* subroutine, const string &subroutineName );
	void emitMnsInSubroutineCall( );
	void setSubroutineName( const string &_name ) {
		_subroutineName = _name;
	}
private:
	string _subroutineName;
	CSubroutine *_subroutine;
};

#endif

