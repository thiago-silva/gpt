#ifndef ASM_PROGRAM_H
#define ASM_PROGRAM_H

#include "Context.hpp"
#include "Options.hpp"

#include <string>
#include <stack>
using namespace std;

class AsmProgram
{
public:
	AsmProgram(Options *options);
	~AsmProgram();
	void init();
	void finish();
	Subroutine *initSubroutine(string name);
	void finishSubroutine(Subroutine *subroutine);
	Context *addContext();
	Context *remContext();
	Context *getContext();
	void emitVarDefinition(const string &name, const string& type);
	string getAsm() const;
private:
	Options *_options;
	Context *_context;
	stack<Context*> _contexts;
	TextFile _asmPrg;
};

#endif

