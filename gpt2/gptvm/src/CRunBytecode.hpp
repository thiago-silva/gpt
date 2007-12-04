#ifndef GPT_RUN_BYTECODE_H
#define GPT_RUN_BYTECODE_H

#include <string>
#include <vector>
#include <fstream>
#include <stack>

#include "CHeader.hpp"
#include "CSymbolTable.hpp"
#include "CBinString.hpp"
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
   int run();
private:
   void trace(const std::string &message);
   void error(const std::string &message);
   void initOpcodePointer();
   void step();
   void procImprima();
   void procLeia();
//   void setIntData(const int &address, const int &value);
//   int  getIntData(const int &address);
   void setRealData(const int &address, const double &value);
   double getRealData(const int &address);
   void setStringData(const int &address, const std::string &value);
   std::string getStringData(const int &address);
   // opcodes
   void invalidOpcode(const std::string &opcode="");
   void nopOpcode();
   void pcallOpcode();
   void lcallOpcode();
   void exitOpcode();
   void exit_0Opcode();
   void exit_1Opcode();
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
   void popsvOpcode();
   void popivOpcode();
   void poprvOpcode();
   void popdvOpcode();
   void popmvOpcode();
   void incspOpcode();
   void decspOpcode();
   void push_0Opcode();
   void push_1Opcode();
   void push_2Opcode();
   void push_3Opcode();
   void push_4Opcode();
   void push_5Opcode();
   void pushsvOpcode();
   void pushivOpcode();
   void pushrvOpcode();
   void pushdvOpcode();
   void pushmvOpcode();

   void pushsrOpcode();
   void pushirOpcode();
   void pushrrOpcode();
   void pushdrOpcode();
   void pushmrOpcode();

   void pushstOpcode();
   void pushitOpcode();
   void pushrtOpcode();
   void pushctOpcode();
   void pushbtOpcode();
   void pushdtOpcode();
   void pushmtOpcode();

   void incsp_4Opcode();
   void incsp_8Opcode();
   void decsp_4Opcode();
   void decsp_8Opcode();

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


   CHeader         _header;
   CSymbolTable    _symbolTable;
//   CBinString      _globalData;
   CBytecode       _code;
   OpcodePointer   _opcodePointer[OPCODE_NUMBER];
   bool            _stop;
   int             _returnCode;
   CDataStack      _dataStack;
   std::stack<int> _executionStack;
};

#endif

