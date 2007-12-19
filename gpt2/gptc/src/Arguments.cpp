#include "Arguments.hpp"

#include "Tools.hpp"


Arguments::Arguments( )
{
}

void Arguments::init( Subroutine* subroutine, const string &subroutineName )
{
	clear();
	_subroutine     = subroutine;
	_subroutineName = subroutineName;
}

void Arguments::emitMnsInSubroutineCall( )
{
	iterator argument;

	bool varArgs = _subroutineName == "imprima";

	// se for "imprima" empilha o nro de argumentos e o tipo de cada um
	// por enquanto imprima eh um caso especial, mas nas proximas versoes isso seria corrigido...
	for( argument = end() - 1; argument >= begin(); argument-- ) {
		_subroutine->emitPUSHMn( *argument, varArgs );

	}
	if (varArgs) {
		_subroutine->emit( "push", itoa( size( ) ) );
	}
}
