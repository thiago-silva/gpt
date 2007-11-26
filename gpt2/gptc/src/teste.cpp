#include <stdio.h>
#include <fstream>

#include <antlr/TokenBuffer.hpp>
#include <antlr/CommonAST.hpp>

#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
#include "SemanticWalker.hpp"
#include "PortugolTokenTypes.hpp"
#include "SymbolTable.hpp"

#include "TokenNames.hpp"

void dump_tokens(char* fname) {

  std::ifstream fi(fname);
  PortugolLexer lexer(fi, true);
  antlr::TokenBuffer *buffer = new antlr::TokenBuffer(lexer);

  while (true) {
    std::cout << lexer.getLine() << ": [" << buffer->LA(1) << "] "
              << g_tokenNames[buffer->LA(1)] << " (" << lexer.getText() << ")";
    buffer->consume();
    getchar();

    if (buffer->LA(1) == PortugolTokenTypes::EOF_) {
      break;
    }
  }
}

RefPortugolAST dump_tree(char* fname, bool should_dump) {

  std::ifstream fi(fname);
  PortugolLexer lexer(fi, true);
  PortugolParser parser(lexer);

  antlr::ASTFactory ast_factory(PortugolAST::TYPE_NAME,
                                &PortugolAST::factory);

  parser.initializeASTFactory(ast_factory);
  parser.setASTFactory(&ast_factory);

  parser.programa();

  antlr::RefAST ast = parser.getAST();
  RefPortugolAST cast = RefPortugolAST(ast);

  if (!cast) {
    std::cerr << "no parse tree!" << std::endl;
  }

  if (should_dump) {
    std::cerr << "parser.g AST:\n";
    std::cerr << cast->toStringList() << std::endl << std::endl;
  }
  return cast;
}

void semantic(char* fname) {
  RefPortugolAST ast;

  ast = dump_tree(fname, true);

  SymbolTable* symtable = SymbolTable::create(fname);

  SemanticWalker semantic(symtable);
  semantic.programa(ast);

  symtable->dump();

  std::cerr << "semantic.g AST:\n";
  std::cerr << ast->toStringList() << std::endl << std::endl;
}

int main(int argc, char** argv) {

  if (argc < 3) {
    std::cerr << "./test [lps] <file.gpt>" << std::endl;
    return 0;
  }

  switch(*argv[1]) {
    case 'l':
      dump_tokens(argv[2]);
      break;
    case 'p':
      dump_tree(argv[2], true);
      break;
    case 's':
    default:
      semantic(argv[2]);
  }
}
