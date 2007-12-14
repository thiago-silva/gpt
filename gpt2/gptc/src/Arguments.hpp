#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <vector>
using namespace std;

#include "Subroutine.hpp"

class Arguments : public vector<string> // usar list :-)
{
public:
	Arguments( );
	void init( Subroutine* subroutine, const string &subroutineName );
	void emitMnsInSubroutineCall( );
	void setSubroutineName( const string &_name ) {
		_subroutineName = _name;
	}
private:
	string _subroutineName;
	Subroutine *_subroutine;
};

#endif

