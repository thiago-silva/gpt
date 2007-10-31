#include "GASMFE_CArguments.hpp"


CArguments::CArguments( )
{
}

void CArguments::init( CSubroutine* subroutine, const string &subroutineName )
{
	clear( );
	_subroutine     = subroutine;
	_subroutineName = subroutineName;
}

void CArguments::emitMnsInSubroutineCall( )
{
	iterator argument;

	bool varArgs = _subroutineName == "imprima";

	// TODO: emite ao contrario
	// se for "imprima" empilha o nro de argumentos e o tipo de cada um
	// por enquanto imprima eh um caso especial, mas nas proximas versoes isso seria corrigido...
	for( argument = begin(); argument != end(); argument++ ) {
                antlr::RefToken token = *argument;
		_subroutine->emitPUSHMn( token, varArgs );

	}
	if (varArgs) {
		_subroutine->emitPUSHMn( size( ) );
	}
}
