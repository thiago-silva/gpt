#ifndef GPT_RUN_BYTECODE_H
#define GPT_RUN_BYTECODE_H

#include <string>
#include <vector>
#include <fstream>

#include "CHeader.hpp"
#include "CSymbolTable.hpp"
#include "CData.hpp"
#include "CBytecode.hpp"
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
   void invalidOpcode(const std::string &opcode="");
   void nopOpcode();
   void pushSpOpcode();
   void popSpOpcode();
   void pushOpcode();
   void pushStringOpcode();
//   void push1Opcode();
   void pcallOpcode();
//   void exit0Opcode();
   void exitOpcode();

//   OP_EXIT_1,
   void hltOpcode();

   void isumOpcode();
   void ssumOpcode();
   void rsumOpcode();
   void isubOpcode();
   void ssubOpcode();
   void rsubOpcode();
   void imulOpcode();
   void rmulOpcode();
   void idivOpcode();
   void rdivOpcode();
   void imodOpcode();
   void rmodOpcode();
   void igeOpcode();
   void sgeOpcode();
   void rgeOpcode();
   void ileOpcode();
   void sleOpcode();
   void rleOpcode();
   void ineOpcode();
   void sneOpcode();
   void rneOpcode();
   void igtOpcode();
   void sgtOpcode();
   void rgtOpcode();
   void iltOpcode();
   void sltOpcode();
   void rltOpcode();
   void ieqOpcode();
   void seqOpcode();
   void reqOpcode();
   void orOpcode();
   void andOpcode();
   void xorOpcode();
   void inegOpcode();
   void rnegOpcode();
   void notOpcode();
   void iincOpcode();
   void idecOpcode();
   void i2cOpcode();
   void r2cOpcode();
   void s2cOpcode();
   void b2cOpcode();
   void i2rOpcode();
   void c2rOpcode();
   void s2rOpcode();
   void b2rOpcode();
   void i2bOpcode();
   void c2bOpcode();
   void r2bOpcode();
   void s2bOpcode();
   void i2sOpcode();
   void c2sOpcode();
   void r2sOpcode();
   void b2sOpcode();
   void p2sOpcode();
   void c2iOpcode();
   void r2iOpcode();
   void s2iOpcode();
   void b2iOpcode();
   void isetOpcode();
   void ssetOpcode();
   void rsetOpcode();
   void getaOpcode();
   void igetvOpcode();
   void sgetvOpcode();
   void rgetvOpcode();
   void isetvOpcode();
   void ssetvOpcode();
   void rsetvOpcode();
   void jmpOpcode();
   void ifOpcode();
   void ifnotOpcode();
   void popOpcode();
   void incspOpcode();
   void decspOpcode();

//   OP_PUSH_0,
//   OP_PUSH_2,
//   OP_PUSH_3,
//   OP_PUSH_4,
//   OP_PUSH_5,

   void pushIntOpcode();
   void pushRealOpcode();
   void pushCharOpcode();
   void pushBoolOpcode();
   void pushMatrixOpcode();

//   OP_INCSP_4,
//   OP_INCSP_8,
//   OP_DECSP_4,
//   OP_DECSP_8,

   void retOpcode();
   void sallocOpcode();
   void sfreeOpcode();
   void ssetcOpcode();
   void sgetcOpcode();
   void m1allocOpcode();
   void m2allocOpcode();
   void mfreeOpcode();
   void m1setOpcode();
   void m1getOpcode();
   void m2setOpcode();
   void m2getOpcode();
   void mcopyOpcode();
   void mgetSize1Opcode();
   void mgetSize2Opcode();


   CHeader       _header;
   CSymbolTable  _symbolTable;
   CData         _data;
   CBytecode     _code;
   OpcodePointer _opcodePointer[ OPCODE_NUMBER ];
   bool          _stop;
   CDataStack    _dataStack;
};

#endif

