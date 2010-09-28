#ifndef GASMFE_CASM_PROGRAM_H
#define GASMFE_CASM_PROGRAM_H

#include "GASMFE_CContext.hpp"
#include "GASMFE_COptions.hpp"

#include "SymbolTable.hpp"

#include <string>
#include <stack>
using namespace std;

class CAsmProgram
{
public:
	CAsmProgram( COptions *options, SymbolTable *symbolTable );
	~CAsmProgram( );
	void init( );
	void finish( );
	CSubroutine *initSubroutine( string name );
	void finishSubroutine( CSubroutine *subroutine );
	CContext *addContext( );
	CContext *remContext( );
	CContext *getContext( );
	void emitVarDefinition( const string &name, const int &type );
	string getAsm( ) const;
private:
	COptions *_options;
	CContext *_context;
	stack<CContext*> _contexts;
        SymbolTable *_symbolTable;
	CTextFile _asmPrg;
};

#endif

