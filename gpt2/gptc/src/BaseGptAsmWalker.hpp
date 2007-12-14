#ifndef BASEGPTASMWALKER_HPP
#define BASEGPTASMWALKER_HPP

#include <antlr/TreeParser.hpp>
#include <string>
#include <list>

#include "Options.hpp"
#include "AsmProgram.hpp"

class SymbolTable;
class AsmProgram;
class Subroutine;
class GptAsmExpression;

class BaseGptAsmWalker : public antlr::TreeParser {
public:
  BaseGptAsmWalker(SymbolTable*, const std::string&);
protected:
  void finishParser();
  void declareProgramVars(const std::list<std::string>&);
  void declareSubroutineVars(const std::list<std::string>&);
  void declareParameter(const string&);
  int getSymbolType(const string&);

  std::string expandOffset(const std::string&, const std::list<std::string>&);
  void expr_MAIS(GptAsmExpression&, const GptAsmExpression&, const GptAsmExpression&);

//   void declareGlobals(const IDList&);
//   void addPrologue(code);
//   void initStructMembers(const string&, const IDList&, code);

//   std::string getAsmType(const std::string&);

  SymbolTable                 *_symtable;
  std::string                  _filepath;

  Options         _options;
  AsmProgram      *_asmPrg;
  Subroutine      *_subroutine;
};

#endif
