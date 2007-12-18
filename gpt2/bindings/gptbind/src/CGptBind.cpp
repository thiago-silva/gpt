#include <fstream>


#include "CGptBind.hpp"
#include "GptBindLexer.hpp"
#include "GptBindParser.hpp"


CGptBind::CGptBind( COptions *options ) : _options( options )
{
}


bool CGptBind::run( )
{
   ifstream in(_options->sourcefile.c_str());
   GptBindLexer lexer(in);
   GptBindParser parser(lexer);

//   string asmProgram = parser.program(_options->sourcefile);
   parser.sld_grammar();

   string cppOutput = parser.getCpp();
   string hppOutput = parser.getHpp();
   string makefileOutput = parser.getMakefile();

   ofstream cppFile((_options->destfile + ".cpp").c_str(), ios_base::out);
   if (!cppFile) {
      cout << "ERRO: não foi possível abrir o arquivo: \"" << _options->destfile << "\"" << endl;
      return false;
   }

   cppFile << cppOutput;

   ofstream hppFile((_options->destfile + ".hpp").c_str(), ios_base::out);
   if (!hppFile) {
      cout << "ERRO: não foi possível abrir o arquivo: \"" << _options->destfile << "\"" << endl;
      return false;
   }

   hppFile << hppOutput;

   return true;
}

