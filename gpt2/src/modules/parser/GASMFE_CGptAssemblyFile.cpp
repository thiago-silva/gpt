#include "GASMFE_CGptAssemblyFile.hpp"


CGptAssemblyFile::CGptAssemblyFile( const string &filename, const string &chrTab )
	: CTextFile( filename, chrTab )
{
}


CGptAssemblyFile::~CGptAssemblyFile( )
{
}


void CGptAssemblyFile::makeFileHeader( const string &filename )
{
//		writeln( "///////////////////////////////////////////////////" );
//		writeln( "// Programa XXX.hasm gerado a partir de XXX.gpt //" );
//		writeln( "///////////////////////////////////////////////////" );
}


void CGptAssemblyFile::makeBeingHeader( const string &being )
{
//		writeln( "///////////////" );
//		writeln( "// Being XXX //" );
//		writeln( "///////////////" );
}


void CGptAssemblyFile::makeProcedureHeader( const string &name )
{
		writeln( "//****" );
		writeln( string( "// Procedure " ) + name );
		writeln( "//****" );
}
