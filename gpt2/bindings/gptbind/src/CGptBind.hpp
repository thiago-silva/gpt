#ifndef GPT_CGPTBIND_H
#define GPT_CGPTBIND_H

#include "COptions.hpp"

class CGptBind
{
public:
   CGptBind( COptions *options );
   bool run( );
private:
   COptions *_options;
};

#endif

