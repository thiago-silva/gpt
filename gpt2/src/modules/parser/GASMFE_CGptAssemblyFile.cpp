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
	writeln( "// Programa XXX.gasm gerado a partir de XXX.gpt" );
}


void CGptAssemblyFile::makeProcedureHeader( const string &name )
{
	writeln( string( "// Procedure " ) + name );
}


void CGptAssemblyFile::makeProcedureFooter( )
{
	writeln( "// Fim da procedure" );
	writeln( );
}


void CGptAssemblyFile::makeFileFooter( )
{
}

