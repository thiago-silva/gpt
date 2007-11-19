#ifndef GPT_CGPTASM_H
#define GPT_CGPTASM_H

#include "COptions.hpp"

class CGptAsm
{
public:
   CGptAsm( COptions *options );
   bool run( );
private:
   COptions *_options;
};

#endif

