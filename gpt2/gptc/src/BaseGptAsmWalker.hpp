#ifndef BASEGPTASMWALKER_HPP
#define BASEGPTASMWALKER_HPP

#include <antlr/TreeParser.hpp>
#include <string>
#include <list>

#include "Options.hpp"

class SymbolTable;
class Symbol;
class AsmProgram;
class Subroutine;
class GptAsmExpression;
class Type;

class BaseGptAsmWalker : public antlr::TreeParser {
public:
  BaseGptAsmWalker(SymbolTable*, const std::string&);

  std::string getCode();

protected:
  void finishParser();
  void declareProgramVars(const std::list<std::string>&);
  void declareSubroutineVars(const std::list<std::string>&);
  void declareParameter(const string&);
  int getSymbolType(const string&);

  void addMatrixInitialization(const Symbol& symbol);

//   std::string expandOffset(const std::string&, const std::list<std::string>&);

  void emitAttribution(const Symbol&, const std::string&);

  GptAsmExpression* expr_OU(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_E(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_BIT_OU(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_BIT_XOU(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_BIT_E(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_IGUAL(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_DIFERENTE(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MAIOR(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MENOR(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MAIOR_EQ(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MENOR_EQ(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_BIT_SHIFT_LEFT(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_BIT_SHIFT_RIGHT(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MAIS(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MENOS(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_DIV(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MULTIP(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_MOD(Type*, GptAsmExpression*, GptAsmExpression*);
  GptAsmExpression* expr_UN_NEGATIVO(Type*,GptAsmExpression*);
  GptAsmExpression* expr_UN_POSITIVO(Type*,GptAsmExpression*);
  GptAsmExpression* expr_UN_NAO(Type*,GptAsmExpression*);
  GptAsmExpression* expr_BIT_NAO(Type*,GptAsmExpression*);

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
