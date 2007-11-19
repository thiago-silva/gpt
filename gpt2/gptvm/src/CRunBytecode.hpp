#ifndef GPT_RUN_BYTECODE_H
#define GPT_RUN_BYTECODE_H

#include <string>
#include <vector>
#include <fstream>

#include "CHeader.hpp"
#include "CSymbolTable.hpp"
#include "CData.hpp"
#include "CCode.hpp"
#include "Common.hpp"
#include "CDataStack.hpp"

class CRunBytecode;

typedef void (CRunBytecode:: *OpcodePointer) ( );


class CRunBytecode
{
public:
   CRunBytecode();
   ~CRunBytecode();
   bool readFromFile(std::ifstream &in);
   bool run();
private:
   void trace(const std::string &message);
   void error(const std::string &message);
   void initOpcodePointer();
   void step();
   void procImprima();
   // opcodes
   void invalidOpcode();
   void nopOpcode();
   void pushSpOpcode();
   void popSpOpcode();
   void pushOpcode();
   void pushStringOpcode();
   void pushIntOpcode();
   void pushRealOpcode();
   void pushBoolOpcode();
   void pushCharOpcode();
   void push1Opcode();
   void pcallOpcode();
//   void exit0Opcode();
   void exitOpcode();
   void isetOpcode();
   void isumOpcode();
   void imulOpcode();


   CHeader       _header;
   CSymbolTable  _symbolTable;
   CData         _data;
   CCode         _code;
   OpcodePointer _opcodePointer[ OPCODE_NUMBER ];
   int           _IP;
   bool          _stop;
   CDataStack    _dataStack;
};

#endif

