#include <stdio.h>
#include "LangTestLexer.hpp"
#include "LangTestParser.hpp"
#include "LangTestLexerTokenTypes.hpp"
#include <antlr/TokenBuffer.hpp>
#include <antlr/CommonAST.hpp>
#include <antlr/ANTLRException.hpp>
#include <fstream>
#include <iostream>

using namespace std;

void dump_tokens(char* fname) {
  #include "tokenNames.hpp"
  std::ifstream fi(fname);
  LangTestLexer lexer(fi);
  antlr::TokenBuffer *buffer = new antlr::TokenBuffer(lexer);

  while (true) {
    std::cout << lexer.getLine() << ": [" << buffer->LA(1) << "] "
              << tokenNames[buffer->LA(1)] << " (" << lexer.getText() << ")";
    buffer->consume();
    getchar();

    if (buffer->LA(1) == LangTestLexerTokenTypes::EOF_) {
      break;
    }
  }
}

void f(char* fname) {

  std::ifstream fi(fname);
  LangTestLexer lexer(fi);
  LangTestParser parser(lexer);

  parser.teste_desc();
}

int main(int argc, char** argv) {

  if (argc < 2) {
    std::cerr << "./tester <file.gpt>" << std::endl;
    return 0;
  }

  try {
//     dump_tokens(argv[1]);
    f(argv[1]);
	}
  catch(antlr::ANTLRException& e)
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
