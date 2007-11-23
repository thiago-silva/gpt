#ifndef GPT_EXECUTION_STACK_HPP
#define GPT_EXECUTION_STACK_HPP

#include <stack>


class CExecutionStack : public std::stack<int>
{
public:
   CExecutionStack();
   void pushStackRegs();
   void popStackRegs();
private:
   int _BP;
   int _SP;
};

#endif

