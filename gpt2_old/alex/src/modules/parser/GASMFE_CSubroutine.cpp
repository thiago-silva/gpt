#include "GASMFE_CSubroutine.hpp"

#include <iostream>
using namespace std;

#include "Tools.hpp"

#include "PortugolParserTokenTypes.hpp"


CSubroutine::CSubroutine( COptions *options, /*CGptAssemblyFile *file,*/ const string &name, SymbolTable *symbolTable )
	: _options( options ), _name( name ), _symbolTable( symbolTable ), _lastSourceLine( 0 )
{
	if ( _options->sentences ) {
		//_sourceInfo = new SourceInfo( );
		//_sourceInfo->setFileName( _options->sourcefile );
	}
	_body.incTab( );
}


CSubroutine::~CSubroutine( )
{
	//if ( _sourceInfo ) {
	//	delete _sourceInfo;
	//}
}


void CSubroutine::init( )
{
	_header.writeln( string( "// Procedure " ) + _name );
	_header.writeln( "proc " + _name );
	_header.incTab( );
//	_body.incTab( );
}


void CSubroutine::finish( )
{
	if (_name == "main") {
		_body.writeln( "exit_0" );
	}
//	_body->decTab( );
	_footer.writeln( "end-proc" );

	_footer.writeln( "// Fim da procedure" );
	_footer.writeln( );
}


void CSubroutine::emitPUSHMn( const string &symbol, const bool &pushType )
{
   SymbolType symbolType = _symbolTable->getSymbol( getName( ), symbol, true ).getType( );

   _body.writeln( string("push ")   + symbol );

   if (pushType) {
      _body.writeln( string("push_") + symbolType.toAsmType( ) );
   }
}


void CSubroutine::emitMn( const string &mn, const string &op1, const string &op2, const string &op3)
{
	_body.write( mn );
	if (op1.empty()) {
		_body.writeln( );
		return;
	}

	_body.write( " " + op1 );
	if (op2.empty()) {
		_body.writeln( );
		return;
	}

	_body.write( ", " + op2 );
	if (op3.empty()) {
		_body.writeln( );
		return;
	}

	_body.writeln( ", " + op3 );
}

void CSubroutine::emitMnWithPrefix( const string &mn, const string &var, const string &op1, const string &op2 )
{
	emitMn( getMnWithPrefix( mn, var ), var, op1, op2 ); 
}


void CSubroutine::emitLabel( const string &label )
{
	_body.writeln( label + string(":") );
}


void CSubroutine::emitAsmCode( const string &code )
{
	_body.writeln( );
	_body.writeln( "// asm-begin" );
	_body.writeln( code );
	_body.writeln( "// asm-end" );
	_body.writeln( );
}


/*
void CSubroutine::write( string value )
{
//	if ( _sourceInfo && *_options->sourceLine > _lastSourceLine ) {
//		string line = _sourceInfo->getLine( *_options->sourceLine );
//		line = alltrim( wo_ctrl_chrs( line, ' ' ) );
//		_file->write(
//				string( "// LINE " ) + string( itoa( *_options->sourceLine ) ) + string( " --> " ) + line
//				);
//		_lastSourceLine = *_options->sourceLine;
//	}

	_body.write( value );
}

void CSubroutine::writeln( string value )
{
//	if ( _sourceInfo && *_options->sourceLine > _lastSourceLine ) {
//		string line = _sourceInfo->getLine( *_options->sourceLine );
//		line = alltrim( wo_ctrl_chrs( line, ' ' ) );
//		_file->writeln(
//				string( "// LINE " ) + string( itoa( *_options->sourceLine ) ) + string( " --> " ) + line
//				);
//		_lastSourceLine = *_options->sourceLine;
//	}

	_body.writeln( value );
}
*/

void CSubroutine::emitVarDefinition( const string &name, const int &type )
{
	_header.writeln( "var " + name + " " + typeInAsm( type ) );
}


void CSubroutine::emitParDefinition( const string &name, const int &type )
{
	_header.writeln( "par " + name + " " + typeInAsm( type ) );
}


string CSubroutine::getCode( ) const
{
	return _header.getText() + _body.getText() + _footer.getText();
}


string CSubroutine::getMnWithPrefix( const string &mn, const string &var ) // TODO: serviria para simbolos tb ???
{
	int primitiveType = _symbolTable->getSymbol (getName(), var, true).getType().getPrimitiveType();

	switch(primitiveType) {
		case PortugolParserTokenTypes::T_KW_INTEIRO:    return "i" + mn;
		case PortugolParserTokenTypes::T_KW_REAL:       return "r" + mn;
		case PortugolParserTokenTypes::T_KW_CARACTERE:  return "i" + mn;
		case PortugolParserTokenTypes::T_KW_LITERAL:    return "s" + mn;
		case PortugolParserTokenTypes::T_KW_LOGICO:     return "i" + mn;
		case PortugolParserTokenTypes::T_KW_MATRIZ:     return "m" + mn;
		default:                                        return "undefined_" + mn;
	}
}

