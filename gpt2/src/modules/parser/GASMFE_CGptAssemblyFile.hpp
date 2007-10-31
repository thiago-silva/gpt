#ifndef GASMFE_CGPT_ASSEMBLY_FILE_H
#define GASMFE_CGPT_ASSEMBLY_FILE_H

#include "CTextFile.hpp"

class CGptAssemblyFile : public CTextFile
{
public:
	CGptAssemblyFile( const string &filename, const string &chrTab = "\t" );
	~CGptAssemblyFile( );
	void makeFileHeader( const string &filename );
	void makeFileFooter( );
	void makeProcedureHeader( const string &name );
	void makeProcedureFooter( );
};

#endif

