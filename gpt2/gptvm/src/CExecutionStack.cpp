#include "CExecutionStack.hpp"


CExecutionStack::CExecutionStack()
   : _BP(0)
   , _SP(0)
{
}


void CExecutionStack::pushStackRegs()
{
   push(_BP);
   push(_SP);
   _BP = _SP;
}


void CExecutionStack::popStackRegs()
{
   _SP = top();
   pop();

   _BP = top();
   pop();
}

