#ifndef HASMFE_CGPT_ASSEMBLY_FILE_H
#define HASMFE_CGPT_ASSEMBLY_FILE_H

#include "CTextFile.hpp"

class CGptAssemblyFile : public CTextFile
{
public:
	CGptAssemblyFile( const string &filename, const string &chrTab = "\t" );
	~CGptAssemblyFile( );
	void makeFileHeader( const string &filename );
	void makeBeingHeader( const string &being );
	void makeProcedureHeader( const string &name );
};

#endif

