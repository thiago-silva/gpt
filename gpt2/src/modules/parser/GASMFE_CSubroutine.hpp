#ifndef GASMFE_CSUBROUTINE_H
#define GASMFE_CSUBROUTINE_H

#include <string>
#include <vector>

#include "GASMFE_COptions.hpp"
//#include "SourceInfo.hpp"

#include <antlr/LLkParser.hpp>
#include "PortugolParserTokenTypes.hpp"

#include "SymbolTable.hpp"
#include "CTextFile.hpp"

using namespace std;


class CSubroutine
{
public:
	CSubroutine( COptions *options, /*CGptAssemblyFile *file,*/ const string &name, SymbolTable *symbolTable );
	~CSubroutine( );
	void init( );
	void finish( );
	void emitPUSHMn( const string &symbol, const bool &pushType = false );
	void emitMn( const string &mn, const string &op1 = "", const string &op2 = "", const string &op3 = "");
	void emitMnWithPrefix( const string &mn, const string &var, const string &op1 = "", const string &op2 = "" );

	void emitLabel( const string &label );

	void emitAsmCode( const string &code );

//	void write( string value );
//	void writeln( string value = "" );

	const string& getName( ) const
	{
		return _name;
	}
	void emitVarDefinition( const string &name, const int &type );
	void emitParDefinition( const string &name, const int &type );
	string getCode( ) const;
private:
	string getMnWithPrefix( const string &mn, const string &var );
	COptions *_options;
	vector<string> _params;
	//SourceInfo *_sourceInfo;
	int _lastSourceLine;
	string _name;
	SymbolTable *_symbolTable;
	CTextFile _header;
	CTextFile _body;
	CTextFile _footer;
};

#endif

