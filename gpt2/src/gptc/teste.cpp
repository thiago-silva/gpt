#include <stdio.h>
#include "PortugolLexer.hpp"
#include "PortugolParser.hpp"
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

void dump_tree(char* fname) {

  std::ifstream fi(fname);
  PortugolLexer lexer(fi, true);
  PortugolParser parser(lexer);

  antlr::ASTFactory ast_factory(antlr::CommonAST::TYPE_NAME,&antlr::CommonAST::factory);
  parser.initializeASTFactory(ast_factory);
  parser.setASTFactory(&ast_factory);

  parser.programa();

  antlr::RefCommonAST ast = antlr::RefCommonAST(parser.getAST());

  if (ast) {
    std::cerr << ast->toStringList() << std::endl << std::endl;
  } else {
    std::cerr << ": no parse tree" << std::endl;
  }
}

int main(int argc, char** argv) {

  if (argc < 3) {
    std::cerr << "./test [lp] <file.gpt>" << std::endl;
    return 0;
  }

  if (*argv[1] == 't') {
    dump_tree(argv[2]);
  } else {
    dump_tokens(argv[2]);
  }

  return 0;
}
