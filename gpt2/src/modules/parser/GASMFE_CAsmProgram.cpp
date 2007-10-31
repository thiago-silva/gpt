#include "GASMFE_CAsmProgram.hpp"


CAsmProgram::CAsmProgram( COptions *options )
	: _options( options ), _context( NULL )
{
	_file = new CGptAssemblyFile( _options->destfile );
	_file->makeFileHeader( _options->destfile );
	_file->writeln( "program " + _options->destfile );
	_file->writeln( );
}

CAsmProgram::~CAsmProgram( )
{
	_file->makeFileFooter( );

	_file->writeln( "end-program" );

	delete _file;
}

CSubroutine *CAsmProgram::initSubroutine( const char *name )
{
	CSubroutine *action = new CSubroutine( _options, _file, name );

	return action;
}


void CAsmProgram::finishSubroutine( CSubroutine *action )
{
	delete action;
}


CContext *CAsmProgram::addContext( )
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

