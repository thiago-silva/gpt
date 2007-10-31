#ifndef GASMFE_CSUBROUTINE_H
#define GASMFE_CSUBROUTINE_H

#include <string>
#include <vector>

#include "GASMFE_CGptAssemblyFile.hpp"
#include "GASMFE_COptions.hpp"
//#include "SourceInfo.hpp"

#include <antlr/LLkParser.hpp>
#include "PortugolParserTokenTypes.hpp"

using namespace std;


class CSubroutine
{
public:
	CSubroutine( COptions *options, CGptAssemblyFile *file, const string &name );
	~CSubroutine( );
	void beginParams( );
	void endParams( );
	void addParam( const string &name );
	void emitCodeToLoadParamValues( );
        void emitPCALLMn( const string &name );
        void emitISETMn( const string &var, const string &value );
        void emitPUSHMn( const antlr::RefToken &_token, const bool &_pushType = false );
	void emitPUSHMn( const int& value );
	void emitLabel( const string &label );

	void write( string value );
	void writeln( string value );

	const string& getName( ) const
	{
		return _name;
	}
private:
	COptions *_options;
	vector<string> _params;
	CGptAssemblyFile* _file;
	//SourceInfo *_sourceInfo;
	int _lastSourceLine;
	string _name;
};

#endif

