#include "Subroutine.hpp"

#include <iostream>
using namespace std;

#include "Tools.hpp"
#include "Types.hpp"

#include "PortugolTokenTypes.hpp"


Subroutine::Subroutine(Options *options, /*CGptAssemblyFile *file,*/ const string &name)
	: _options(options), _name(name), _tmpSuffix(1), _lastSourceLine(0)
{
	if (_options->sentences) {
		//_sourceInfo = new SourceInfo();
		//_sourceInfo->setFileName(_options->sourcefile);
	}
	_body.incTab();
}


Subroutine::~Subroutine()
{
	//if (_sourceInfo) {
	//	delete _sourceInfo;
	//}
}


void Subroutine::init()
{
  _header.writeln();
	_header.writeln(string("// Procedure ") + _name);
	_header.writeln("proc " + _name);
	_header.incTab();
//	_body.incTab();
}


void Subroutine::finish()
{
	if (_name == "main") {
		_body.writeln("exit_0");
	}
//	_body->decTab();
	_footer.writeln("end-proc");

	_footer.writeln("// Fim da procedure");
	_footer.writeln();
}


void Subroutine::emitPUSHMn(const string &symbol, const bool &pushType)
{
//    SymbolType symbolType = _symbolTable->getSymbol(getName(), symbol, true).getType();
// 
//    _body.writeln(string("push ")   + symbol);
// 
//    if (pushType) {
//       _body.writeln(string("push_") + symbolType.toAsmType());
//    }
}


void Subroutine::emit(const string &mn, const string &op1, const string &op2, const string &op3)
{
	_body.write(mn);
	if (op1.empty()) {
		_body.writeln();
		return;
	}

	_body.write(" " + op1);
	if (op2.empty()) {
		_body.writeln();
		return;
	}

	_body.write(", " + op2);
	if (op3.empty()) {
		_body.writeln();
		return;
	}

	_body.writeln(", " + op3);
}

void Subroutine::emitMnWithPrefix(const string &mn, const string &var, const string &op1, const string &op2)
{
	emit(getMnWithPrefix(mn, var), var, op1, op2);
}


void Subroutine::emitLabel(const string &label)
{
	_body.writeln(label + string(":"));
}


void Subroutine::emitAsmCode(const string &code)
{
	_body.writeln();
	_body.writeln(code);
	_body.writeln();
}


/*
void Subroutine::write(string value)
{
//	if (_sourceInfo && *_options->sourceLine > _lastSourceLine) {
//		string line = _sourceInfo->getLine(*_options->sourceLine);
//		line = alltrim(wo_ctrl_chrs(line, ' '));
//		_file->write(
//				string("// LINE ") + string(itoa(*_options->sourceLine)) + string(" --> ") + line
//				);
//		_lastSourceLine = *_options->sourceLine;
//	}

	_body.write(value);
}

void Subroutine::writeln(string value)
{
//	if (_sourceInfo && *_options->sourceLine > _lastSourceLine) {
//		string line = _sourceInfo->getLine(*_options->sourceLine);
//		line = alltrim(wo_ctrl_chrs(line, ' '));
//		_file->writeln(
//				string("// LINE ") + string(itoa(*_options->sourceLine)) + string(" --> ") + line
//				);
//		_lastSourceLine = *_options->sourceLine;
//	}

	_body.writeln(value);
}
*/

void Subroutine::emitVarDefinition(const string &name, Type* type)
{
	_header.writeln("var " + name + " " + type->asmName());

  if (type->isPrimitive() && type->equals(PortugolTokenTypes::T_LITERAL)) {
    _prologue.writeln("salloc " + name);
  }
}

void Subroutine::emitParDefinition(const string &name, Type* type)
{
	_header.writeln("par " + name + " " + type->asmName());
}

string Subroutine::declareTmp(Type* type) {
  std::stringstream s;
  s << "tmp" << _tmpSuffix++;
 
  std::string name = s.str();
  emitVarDefinition(name, type);
  return name;
}

string Subroutine::getCode() const
{
	return _header.getText() + _prologue.getText() 
         + _body.getText() + _footer.getText();
}


string Subroutine::getMnWithPrefix(const string &mn, const string &var) // TODO: serviria para simbolos tb ???
{
// 	int primitiveType = _symbolTable->getSymbol (getName(), var, true).getType().getPrimitiveType();

// 	switch(primitiveType) {
// 		case PortugolParserTokenTypes::T_KW_INTEIRO:    return "i" + mn;
// 		case PortugolParserTokenTypes::T_KW_REAL:       return "r" + mn;
// 		case PortugolParserTokenTypes::T_KW_CARACTERE:  return "i" + mn;
// 		case PortugolParserTokenTypes::T_KW_LITERAL:    return "s" + mn;
// 		case PortugolParserTokenTypes::T_KW_LOGICO:     return "i" + mn;
// 		case PortugolParserTokenTypes::T_KW_MATRIZ:     return "m" + mn;
// 		default:                                        return "undefined_" + mn;
// 	}
}

