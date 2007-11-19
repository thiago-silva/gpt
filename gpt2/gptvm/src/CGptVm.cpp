#include <fstream>

#include "CGptVm.hpp"
#include "CRunBytecode.hpp"


CGptVm::CGptVm( COptions *options ) : _options( options )
{
}


bool CGptVm::run( )
{
   ifstream in(_options->sourcefile.c_str(),ios::in | ios::binary);

   CRunBytecode bytecode;

   return bytecode.readFromFile(in) && bytecode.run();
}

