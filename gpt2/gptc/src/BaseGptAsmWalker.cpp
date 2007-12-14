#include "BaseGptAsmWalker.hpp"

#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "AsmProgram.hpp"
#include "Types.hpp"

BaseGptAsmWalker::BaseGptAsmWalker(SymbolTable* symtable,
                                   const std::string& filepath)
  : antlr::TreeParser(), _symtable(symtable), _filepath(filepath) {

  _symtable->setGlobalScope();

  _options.filename = _filepath;
  //options.sourcefile = filename + ".gpt";
  //options.destfile   = filename + ".gasm";
  _asmPrg = new AsmProgram(&_options);
}


void BaseGptAsmWalker::finishParser() {
  delete _asmPrg;
  _asmPrg = NULL;
}

void BaseGptAsmWalker::declareProgramVars(const std::list<string>& ids) {
  for (std::list<string>::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    _asmPrg->emitVarDefinition(
      *it, _symtable->getSymbol(*it).type()->asmName());
  }
}

void BaseGptAsmWalker::declareSubroutineVars(const std::list<string>& ids) {
  for (std::list<string>::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    _subroutine->emitVarDefinition(
      *it, _symtable->getSymbol(*it).type()->asmName());
  }  
}

void BaseGptAsmWalker::declareParameter(const string& lexeme) {
  _subroutine->emitParDefinition(
    lexeme, _symtable->getSymbol(lexeme).type()->asmName());
}

int BaseGptAsmWalker::getSymbolType(const string& lexeme) {
  return _symtable->getSymbol(lexeme).type()->primitiveType();
}

std::string BaseGptAsmWalker::expandOffset(const std::string&, const std::list<std::string>&) {
}

void BaseGptAsmWalker::expr_MAIS(GptAsmExpression&, const GptAsmExpression&, const GptAsmExpression&) {
}






//      string getAsmProgram( ) const
//      {
//         return asmPrg->getAsm( );
//      }

// void BaseGptAsmWalker::declareGlobals(const IDList& ids) {
//   for (IDList::const_iterator it = ids.begin(); it != ids.end(); ++it) {
//     _globalarea << "var " << (*it) << " " << getAsmType(*it) << std::endl;
//   }
// }
// 
// std::string BaseGptAsmWalker::getAsmType(const std::string& id) {
//   return _symtable->getSymbol(
//       id, _symtable->currentScope()).type()->asmName();
// }

