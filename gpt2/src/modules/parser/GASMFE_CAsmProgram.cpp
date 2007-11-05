#include "GASMFE_CAsmProgram.hpp"

#include "Tools.hpp"


CAsmProgram::CAsmProgram( COptions *options, SymbolTable *symbolTable )
	: _options( options ), _symbolTable( symbolTable ), _context( NULL )
{
}

void CAsmProgram::init( )
{
	_asmPrg.writeln( "// Programa " + _options->filename + ".gasm gerado a partir de " + _options->filename + ".gpt" );
	_asmPrg.writeln( "program " + _options->filename );
	_asmPrg.writeln( );
}

void CAsmProgram::finish( )
{
	_asmPrg.writeln( "end-program" );
}

CAsmProgram::~CAsmProgram( )
{
}

CSubroutine *CAsmProgram::initSubroutine( string name )
{
	CSubroutine *action = new CSubroutine( _options, /*_file,*/ name, _symbolTable );
	action->init( );

	return action;
}


void CAsmProgram::finishSubroutine( CSubroutine *action )
{
	action->finish( );
	_asmPrg.write( action->getCode( ) );
	delete action;
}


/*CContext *CAsmProgram::addContext( )
{
	CContext* context = new CContext( );
	_contexts.push( context );
	_context = context;

	return context;
}


CContext *CAsmProgram::remContext( )
{
	delete _contexts.top( );
	_contexts.pop( );
	if ( _contexts.size( ) > 0 ) {
		_context = _contexts.top( );
		return _contexts.top( );
	} else {
		_context = NULL;
		return NULL;
	}
}


CContext *CAsmProgram::getContext( )
{
	return _context;
}
*/

void CAsmProgram::emitVarDefinition( const string &name, const int &type )
{
	_asmPrg.writeln( "var " + name + " " + typeInAsm( type ) );
}


string CAsmProgram::getAsm( ) const
{
	return _asmPrg.getText( );
}

