#ifndef GASMFE_CASM_PROGRAM_H
#define GASMFE_CASM_PROGRAM_H

#include "GASMFE_CContext.hpp"
#include "GASMFE_CGptAssemblyFile.hpp"
#include "GASMFE_COptions.hpp"

#include <string>
#include <stack>
using namespace std;

class CAsmProgram
{
public:
	CAsmProgram( COptions *options );
	~CAsmProgram( );
	CSubroutine *initSubroutine( const char *name );
	void finishSubroutine( CSubroutine *subroutine );
	CContext *addContext( );
	CContext *remContext( );
	CContext *getContext( );
	void emitVarDefinition( const string &name, const int &type );
private:
	COptions *_options;
	CContext *_context;
	stack<CContext*> _contexts;
	CGptAssemblyFile *_file;
};

#endif

