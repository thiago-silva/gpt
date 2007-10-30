#ifndef GASMFE_CARGUMENTS_H
#define GASMFE_CARGUMENTS_H

#include <vector>
using namespace std;



// TODO: talvez nao precise de todos esses headers abaixo...
#include <antlr/config.hpp>
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include <antlr/NoViableAltException.hpp>
#include <antlr/LLkParser.hpp>
#include "PortugolParserTokenTypes.hpp"





//#include "GASMFE_CArgument.hpp"
#include "GASMFE_CSubroutine.hpp"

class CArguments : public vector<antlr::RefToken> // usar list :-)
{
public:
	CArguments( CSubroutine *subroutine );
	// TODO: subroutine no construtor
	void emitMnsInSubroutineCall( );
private:
	CSubroutine *_subroutine;
};

#endif

