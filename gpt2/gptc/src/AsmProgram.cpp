#include "AsmProgram.hpp"

#include "Tools.hpp"


AsmProgram::AsmProgram(Options *options)
	: _options(options), _context(NULL)
{
}

void AsmProgram::init()
{
	_asmPrg.writeln("// Programa gerado a partir de " + _options->filename);
	_asmPrg.writeln("program");
	_asmPrg.writeln();
}

void AsmProgram::finish()
{
  _asmPrg.writeln();
	_asmPrg.writeln("end-program");
}

AsmProgram::~AsmProgram()
{
}

Subroutine *AsmProgram::initSubroutine(string name)
{
	Subroutine *action = new Subroutine(_options, /*_file,*/ name);
	action->init();

	return action;
}


void AsmProgram::finishSubroutine(Subroutine *action)
{
	action->finish();
	_asmPrg.write(action->getCode());
	delete action;
}


/*CContext *AsmProgram::addContext()
{
	CContext* context = new CContext();
	_contexts.push(context);
	_context = context;

	return context;
}


CContext *AsmProgram::remContext()
{
	delete _contexts.top();
	_contexts.pop();
	if (_contexts.size() > 0) {
		_context = _contexts.top();
		return _contexts.top();
	} else {
		_context = NULL;
		return NULL;
	}
}


CContext *AsmProgram::getContext()
{
	return _context;
}
*/

void AsmProgram::emitVarDefinition(const string &name, const string &type)
{
	_asmPrg.writeln("var " + name + " " + type);
}


string AsmProgram::getAsm() const
{
	return _asmPrg.getText();
}

