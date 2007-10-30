#ifndef GASMFE_CSUBROUTINE_H
#define GASMFE_CSUBROUTINE_H

#include <string>
#include <vector>

#include "GASMFE_CGptAssemblyFile.hpp"
#include "GASMFE_COptions.hpp"
//#include "SourceInfo.hpp"

using namespace std;

// argument type
const int AT_UNKNOWN       = 0;
const int AT_DESTRUCTIVE   = 1;
const int AT_REFERENCE     = 2;
const int AT_ACTION_RETURN = 3;

// invocation type
const int IT_NBQU = 1;
const int IT_NBDQU = 2;
const int IT_QU = 3;
const int IT_DQU = 4;
const int IT_AF = 5;


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
	void emitPUSHMn( const string &literal );



	void emitSVMn(const string &var);
	void emitLVMn(const string &var);
	void emitSIVMn(const string &var);
	void emitLIVMn(const string &var);
	void emitADDMn( );
	void emitSUBMn( );
	void emitMULMn( );
	void emitDIVMn( );
	void emitINCMn( );
	void emitDECMn( );
	void emitLSBMn( const string &symbol );
	void emitLabel( const string &label );
	void emitGTMn( );
	void emitGEMn( );
	void emitEQMn( );
	void emitLEMn( );
	void emitLTMn( );
	void emitNEMn( );
	void emitIFNOTMn( const string &label );
	void emitIFMn( const string &label );
	void emitGOTOMn( const string &label );
	void emitSNMn( );
	void emitLNMn( );
	void emitLBMn( );
	void emitLOBMn( );
	void emitLHMn( );
	void emitLOHMn( );
	void emitAFMn( );
	void emitQUMn( );
	void emitNBQUMn( );
	void emitNBDQUMn( );
	void emitDQUMn( );
	void emitWHOAMIMn( );
	void emitHOWMANYMn( );
	void emitWHEREAMIMn( );
	void emitTIMEMn( );
	void emitREADMn( );
	void emitWRMn( );
	void emitWRLNMn( );
	void emitANDMn( );
	void emitORMn( );
	void emitCLONEMn( );
	void emitMOVEMn( );
	void emitLISTSIZEMn( );
	void emitLISTINDEXMn( );
	void emitLISTVALUEMn( );
	void emitInvocationTypeMn( const int &invocationType );
	void write( string value );
	void writeln( string value );
private:
	COptions *_options;
	vector<string> _params;
	CGptAssemblyFile* _file;
	//SourceInfo *_sourceInfo;
	int _lastSourceLine;
};

#endif

