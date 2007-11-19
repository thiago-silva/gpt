#include <fstream>


#include "CGptAsm.hpp"
#include "GptAssemblyLexer.hpp"
#include "GptAssemblyParser.hpp"


CGptAsm::CGptAsm( COptions *options ) : _options( options )
{
}


bool CGptAsm::run( )
{
   ifstream in(_options->sourcefile.c_str());
   GptAssemblyLexer lexer(in);
   GptAssemblyParser parser(lexer);

   string asmProgram = parser.program(_options->sourcefile);

   ofstream asmFile(_options->destfile.c_str(), ios_base::out);
   if (!asmFile) {
      cout << "ERRO: não foi possível abrir o arquivo: \"" << _options->destfile << "\"" << endl;
      return false;
   }

   asmFile << asmProgram;

   return true;
}

