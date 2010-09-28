#include <stdio.h>
#include "LangTestLexer.hpp"
#include "LangTestParser.hpp"
#include "LangTestLexerTokenTypes.hpp"
#include "TestAST.hpp"
#include <antlr/TokenBuffer.hpp>
#include <antlr/CommonAST.hpp>
#include <antlr/ANTLRException.hpp>
#include <fstream>
#include <iostream>

#include <map>
#include <string>

using namespace std;
using namespace antlr;

void dump_tokens(char* fname) {
  #include "testTokenNames.hpp"
  ifstream fi(fname);
  LangTestLexer lexer(fi);
  TokenBuffer *buffer = new TokenBuffer(lexer);

  while (true) {
    cerr << lexer.getLine() << ": [" << buffer->LA(1) << "] "
              << testTokenNames[buffer->LA(1)] << " (" << lexer.getText() << ")";
    buffer->consume();
    getchar();

    if (buffer->LA(1) == LangTestLexerTokenTypes::EOF_) {
      break;
    }
  }
}

RefAST dump_tree(char* fname) {

  std::ifstream fi(fname);
  LangTestLexer lexer(fi);
  LangTestParser parser(lexer);

  ASTFactory ast_factory(TestAST::TYPE_NAME,&TestAST::factory);
  parser.initializeASTFactory(ast_factory);
  parser.setASTFactory(&ast_factory);

  parser.teste_desc();

  RefAST ast    = parser.getAST();
  RefCommonAST cast = RefCommonAST(ast);

  if (!cast) {
    cerr << "no parse tree!" << endl;
  } else {
    cerr << cast->toStringList() << endl << endl;
  }
  return ast;
}

void f(char* fname) {

  ifstream fi(fname);
  LangTestLexer lexer(fi);
  LangTestParser parser(lexer);

  ASTFactory ast_factory(TestAST::TYPE_NAME,&TestAST::factory);
  parser.initializeASTFactory(ast_factory);
  parser.setASTFactory(&ast_factory);

  parser.teste_desc();
}

int main(int argc, char** argv) {

  if (argc < 2) {
    cerr << "./tester <file.gpt>" << endl;
    return 0;
  }

//   DEBUG_PARSER = true;
  try {
//     dump_tokens(argv[1]);
    f(argv[1]);
//     dump_tree(argv[1]);
	}
  catch(ANTLRException& e)
	{
		cerr << "Parse exception: " << e.toString() << endl;
		return -1;
	}
	catch(exception& e)
	{
		cerr << "exception: " << e.what() << endl;
		return -1;
	}

  return 0;
}
