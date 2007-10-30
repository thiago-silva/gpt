#include "GASMFE_CSubroutine.hpp"

#include <iostream>
using namespace std;

#include "Tools.hpp"


CSubroutine::CSubroutine( COptions *options, CGptAssemblyFile *file, const string &name )
	: _options( options ), _file( file ), _lastSourceLine( 0 )
{
	_file->makeProcedureHeader( name );
	_file->writeln( "proc " + name );
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
	_file->writeln( "end" );
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
		emitLVMn( _params[param] );
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


void CSubroutine::emitPUSHMn( const string &literal )
{
	writeln( string("push ") + literal );
}






















void CSubroutine::emitSVMn(const string &var)
{
	writeln( string("sv ") + var );
}


void CSubroutine::emitLVMn(const string &var)
{
	writeln( string("lv ") + var );
}


void CSubroutine::emitSIVMn(const string &var)
{
	writeln( string("siv ") + var );
}


void CSubroutine::emitLIVMn(const string &var)
{
	writeln( string("liv ") + var );
}


void CSubroutine::emitADDMn( )
{
	writeln( "add" );
}


void CSubroutine::emitSUBMn( )
{
	writeln( "sub" );
}


void CSubroutine::emitMULMn( )
{
	writeln( "mul" );
}


void CSubroutine::emitDIVMn( )
{
	writeln( "div" );
}


void CSubroutine::emitINCMn( )
{
	writeln( "inc" );
}


void CSubroutine::emitDECMn( )
{
	writeln( "dec" );
}


void CSubroutine::emitLSBMn( const string &symbol )
{
	if ( symbol == "null" ) {
		emitLNMn( );
	} else {
		writeln( string("lsb ") + symbol );
	}
}


void CSubroutine::emitLabel( const string &label )
{
	writeln( string(":") + label );
}


void CSubroutine::emitGTMn( )
{
	writeln( "gt" );
}


void CSubroutine::emitGEMn( )
{
	writeln( "ge" );
}


void CSubroutine::emitEQMn( )
{
	writeln( "eq" );
}


void CSubroutine::emitLEMn( )
{
	writeln( "le" );
}


void CSubroutine::emitLTMn( )
{
	writeln( "lt" );
}


void CSubroutine::emitNEMn( )
{
	writeln( "ne" );
}


void CSubroutine::emitIFNOTMn( const string &label )
{
	writeln( string("ifnot ") + label );
}


void CSubroutine::emitIFMn( const string &label )
{
	writeln( string("if ") + label );
}


void CSubroutine::emitGOTOMn( const string &label )
{
	writeln( string("goto ") + label );
}


void CSubroutine::emitSNMn( )
{
	writeln( "sn" );
}


void CSubroutine::emitLNMn( )
{
	writeln( "ln" );
}


void CSubroutine::emitLBMn( )
{
	writeln( "lb" );
}


void CSubroutine::emitLOBMn( )
{
	writeln( "lob" );
}


void CSubroutine::emitLHMn( )
{
	writeln( "lh" );
}


void CSubroutine::emitLOHMn( )
{
	writeln( "loh" );
}


void CSubroutine::emitAFMn( )
{
	writeln( "af" );
}


void CSubroutine::emitQUMn( )
{
	writeln( "qu" );
}


void CSubroutine::emitNBQUMn( )
{
	writeln( "nbqu" );
}


void CSubroutine::emitNBDQUMn( )
{
	writeln( "nbdqu" );
}


void CSubroutine::emitDQUMn( )
{
	writeln( "dqu" );
}


void CSubroutine::emitWHOAMIMn( )
{
	writeln( "whoami" );
}


void CSubroutine::emitHOWMANYMn( )
{
	writeln( "howmany" );
}


void CSubroutine::emitWHEREAMIMn( )
{
	writeln( "whereami" );
}


void CSubroutine::emitTIMEMn( )
{
	writeln( "time" );
}


void CSubroutine::emitREADMn( )
{
	writeln( "read" );
}


void CSubroutine::emitWRMn( )
{
	writeln( "wr" );
}


void CSubroutine::emitWRLNMn( )
{
	writeln( "wrln" );
}


void CSubroutine::emitANDMn( )
{
	writeln( "and" );
}


void CSubroutine::emitORMn( )
{
	writeln( "or" );
}


void CSubroutine::emitCLONEMn( )
{
	writeln( "clone" );
}


void CSubroutine::emitMOVEMn( )
{
	writeln( "move" );
}

void CSubroutine::emitLISTSIZEMn( )
{
	writeln( "ivs" );
}

void CSubroutine::emitLISTINDEXMn( )
{
	writeln( "ivi" );
}

void CSubroutine::emitLISTVALUEMn( )
{
	writeln( "ivv" );
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


void CSubroutine::emitInvocationTypeMn( const int &invocationType )
{
	switch( invocationType ) {
		case IT_AF:
			emitAFMn( );
			break;
		case IT_QU:
			emitQUMn( );
			break;
		case IT_NBQU:
			emitNBQUMn( );
			break;
		case IT_NBDQU:
			emitNBDQUMn( );
			break;
		case IT_DQU:
			emitDQUMn( );
			break;
		default:
			break;
			// TODO: Nao deveria chegar aqui... abortar
	}
}

