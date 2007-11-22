#include <fstream>

#include "CGptVm.hpp"
#include "CRunBytecode.hpp"


CGptVm::CGptVm(COptions *options) : _options(options)
{
}


int CGptVm::run()
{
   ifstream in(_options->sourcefile.c_str(),ios::in | ios::binary);

   CRunBytecode bytecode;

   bytecode.readFromFile(in);

   return bytecode.run();
}

