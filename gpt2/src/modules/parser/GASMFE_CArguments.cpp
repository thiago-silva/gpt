#include "GASMFE_CArguments.hpp"


CArguments::CArguments( CSubroutine *subroutine )
	: _subroutine( subroutine )
{
}

void CArguments::emitMnsInSubroutineCall( )
{
	iterator argument;

	// TODO: emite ao contrario... e se for "imprima" empilha o nro de argumentos e o tipo de cada um
	// por enquanto imprima eh um caso especial, mas nas proximas versoes isso seria corrigido...
	for( argument = begin(); argument != end(); argument++ ) {
		_subroutine->emitPUSHMn( "aaa" );
//              string ttt = argument->getText( );
//		_subroutine->emitPUSHMn( argument );
/*
		if ((*argument)->getType( ) == PortugolParserTokenTypes::T_KW_INTEIROS) {
			xxx
		} else if ((*argument)->_type == AT_DESTRUCTIVE || (*argument)->_type == AT_REFERENCE) {
			if (!(*argument)->_indexVar.empty()) {
				// Variaveis indexadas
				subroutine->emitLVMn( (*argument)->_indexVar );
				subroutine->emitSIVMn( (*argument)->_name );
			} else {
				// Variaveis nao indexadas
				subroutine->emitSVMn( (*argument)->_name );
			}
		} else {
			subroutine->emitSNMn( );
		}
*/
	}
}
