#include <stdio.h>
#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
#include "SemanticWalker.hpp"
#include "PortugolTokenTypes.hpp"
#include <antlr/TokenBuffer.hpp>
#include <antlr/CommonAST.hpp>

#include <fstream>

void dump_tokens(char* fname) {
  char *table[200];
  #include "table.hpp"
  std::ifstream fi(fname);
  PortugolLexer lexer(fi, true);
  antlr::TokenBuffer *buffer = new antlr::TokenBuffer(lexer);

  while (true) {
    std::cout << lexer.getLine() << ": [" << buffer->LA(1) << "] "
              << table[buffer->LA(1)] << " (" << lexer.getText() << ")";
    buffer->consume();
    getchar();

    if (buffer->LA(1) == PortugolTokenTypes::EOF_) {
      break;
    }
  }
}

antlr::RefAST dump_tree(char* fname, bool should_dump) {

  std::ifstream fi(fname);
  PortugolLexer lexer(fi, true);
  PortugolParser parser(lexer);

  antlr::ASTFactory ast_factory(antlr::CommonAST::TYPE_NAME,&antlr::CommonAST::factory);
  parser.initializeASTFactory(ast_factory);
  parser.setASTFactory(&ast_factory);

  parser.programa();

  antlr::RefAST ast = parser.getAST();
  antlr::RefCommonAST cast = antlr::RefCommonAST(ast);

  if (!cast) {
    std::cerr << "no parse tree!" << std::endl;
  }

  if (should_dump) {
    std::cerr << cast->toStringList() << std::endl << std::endl;
  }
  return ast;
}

void semantic(char* fname) {
  antlr::RefAST ast;

  ast = dump_tree(fname, true);

  SemanticWalker semantic;
  semantic.programa(ast);
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
