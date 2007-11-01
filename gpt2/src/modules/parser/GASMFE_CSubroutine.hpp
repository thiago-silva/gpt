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
	void emitISUMMn( const string &var, const string &op1, const string &op2 );
	void emitIMULMn( const string &var, const string &op1, const string &op2 );
	void emitIGEMn( const string &var, const string &op1, const string &op2 );
	void emitIFNOTMn( const string &var, const string &label );
	void emitJMPMn( const string &label );
	void emitMn( const string &mn, const string &op1 = "", const string &op2 = "", const string &op3 = "");

	void emitLabel( const string &label );

	void write( string value );
	void writeln( string value = "" );

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

