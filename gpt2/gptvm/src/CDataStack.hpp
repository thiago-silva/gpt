#ifndef GPT_DATA_STACK_H
#define GPT_DATA_STACK_H


#include <stack>


class CDataStack : public std::stack<void*>
{
public:
   CDataStack();
   ~CDataStack();
private:
};

#endif

