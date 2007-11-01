#include "GASMFE_CSubroutine.hpp"

#include <iostream>
using namespace std;

#include "Tools.hpp"


CSubroutine::CSubroutine( COptions *options, CGptAssemblyFile *file, const string &name )
	: _options( options ), _file( file ), _name( name ), _lastSourceLine( 0 )
{
	_file->makeProcedureHeader( _name );
	_file->writeln( "proc " + _name );
	_file->incTab( );

	if ( _options->sentences ) {
		//_sourceInfo = new SourceInfo( );
		//_sourceInfo->setFileName( _options->sourcefile );
	}
}


CSubroutine::~CSubroutine( )
{
	//if ( _sourceInfo ) {
	//	delete _sourceInfo;
	//}

	_file->decTab( );
	_file->writeln( "end-proc" );

	_file->makeProcedureFooter( );
}


void CSubroutine::beginParams( )
{
	_file->write( "(" );
}


void CSubroutine::endParams( )
{
	_file->writeln( ")" );
}


void CSubroutine::addParam( const string &name )
{
	_params.push_back(name);

	if (_params.size() != 1) {
		_file->write( ", " );
	}
	_file->write( name );
}


void CSubroutine::emitCodeToLoadParamValues( )
{
	for( int param = _params.size( )-1; param >= 0; param-- ) {
		//emitLVMn( _params[param] );
	}
}


void CSubroutine::emitPCALLMn( const string &name )
{
	writeln( string("pcall ") + name );
}


void CSubroutine::emitISETMn( const string &var, const string &value )
{
	writeln( string("iset ") + var + string(", ") + value );
}


void CSubroutine::emitPUSHMn( const antlr::RefToken &_token, const bool &_pushType )
{
//	writeln( string("push ") + literal );
   if (_token->getType( ) == PortugolParserTokenTypes::T_STRING_LIT) {
      writeln( string("push \"") + _token->getText( ) + "\"" );
   } else {
      writeln( string("push ") + _token->getText( ) );
   }
   if (_pushType) {
      writeln( string("push_") + typeToText( _token->getType( ) ) );
   }
}


void CSubroutine::emitPUSHMn( const int& value )
{
   writeln( string("push ") + itoa( value ) );
}


//void CSubroutine::emitADDMn( )
//{
//	writeln( "add" );
//}


void CSubroutine::emitISUMMn( const string &var, const string &op1, const string &op2 )
{
	writeln( "isum " + var + ", " + op1 + ", " + op2 );
}


void CSubroutine::emitIMULMn( const string &var, const string &op1, const string &op2 )
{
	writeln( "imul " + var + ", " + op1 + ", " + op2 );
}


void CSubroutine::emitIGEMn( const string &var, const string &op1, const string &op2 )
{
	writeln( "ige " + var + ", " + op1 + ", " + op2 );
}


void CSubroutine::emitIFNOTMn( const string &var, const string &label )
{
	writeln( "ifnot " + var + ", " + label );
}

void CSubroutine::emitJMPMn( const string &label )
{
	writeln( "jmp " + label );
}


void CSubroutine::emitMn( const string &mn, const string &op1, const string &op2, const string &op3)
{
	write( mn );
	if (op1.empty()) {
		writeln( );
		return;
	}

	write( " " + op1 );
	if (op2.empty()) {
		writeln( );
		return;
	}

	write( ", " + op2 );
	if (op3.empty()) {
		writeln( );
		return;
	}

	writeln( ", " + op3 );
}


void CSubroutine::emitLabel( const string &label )
{
	writeln( label + string(":") );
}




void CSubroutine::write( string value )
{
/*	if ( _sourceInfo && *_options->sourceLine > _lastSourceLine ) {
		string line = _sourceInfo->getLine( *_options->sourceLine );
		line = alltrim( wo_ctrl_chrs( line, ' ' ) );
		_file->write(
				string( "// LINE " ) + string( itoa( *_options->sourceLine ) ) + string( " --> " ) + line
				);
		_lastSourceLine = *_options->sourceLine;
	}*/

	_file->write( value );
}

void CSubroutine::writeln( string value )
{
/*	if ( _sourceInfo && *_options->sourceLine > _lastSourceLine ) {
		string line = _sourceInfo->getLine( *_options->sourceLine );
		line = alltrim( wo_ctrl_chrs( line, ' ' ) );
		_file->writeln(
				string( "// LINE " ) + string( itoa( *_options->sourceLine ) ) + string( " --> " ) + line
				);
		_lastSourceLine = *_options->sourceLine;
	}*/

	_file->writeln( value );
}

