#include "CRunBytecode.hpp"

#include <sstream>
#include <iostream>


CRunBytecode::CRunBytecode()
   : _returnCode(0)
{
}


CRunBytecode::~CRunBytecode()
{
}


bool CRunBytecode::readFromFile(std::ifstream &in)
{
   std::stringstream buf;
   buf << in.rdbuf();
   CBinString bin;
   bin.assign(buf.str());

   _header.readFromBinary(bin) &&
         _symbolTable.readFromBinary(bin);

   bin.readString(_globalData);

   _code.assign(bin.readString());

   return true;
}

void CRunBytecode::initOpcodePointer()
{
   for (int i = 0; i < OPCODE_NUMBER; i++) {
      _opcodePointer[i] = NULL; //&CRunBytecode::invalidOpcode;
   }

   _opcodePointer[OP_NOP        ] = &CRunBytecode::nopOpcode;
   _opcodePointer[OP_PUSH_SREG  ] = &CRunBytecode::pushSregOpcode;
   _opcodePointer[OP_POP_SREG   ] = &CRunBytecode::popSregOpcode;
//   _opcodePointer[OP_PUSH       ] = &CRunBytecode::pushOpcode;
   _opcodePointer[OP_PUSH_STRING] = &CRunBytecode::pushStringOpcode;
//   _opcodePointer[OP_PUSH_1     ] = &CRunBytecode::push1Opcode;
   _opcodePointer[OP_PCALL      ] = &CRunBytecode::pcallOpcode;
   _opcodePointer[OP_LIBCALL    ] = &CRunBytecode::libcallOpcode;
//   _opcodePointer[OP_EXIT_0     ] = &CRunBytecode::exit0Opcode;
   _opcodePointer[OP_EXIT       ] = &CRunBytecode::exitOpcode;

//   OP_EXIT_1,
   _opcodePointer[OP_HLT        ] = &CRunBytecode::hltOpcode;

   _opcodePointer[OP_ISUM       ] = &CRunBytecode::isumOpcode;
   _opcodePointer[OP_SSUM       ] = &CRunBytecode::ssumOpcode;
   _opcodePointer[OP_RSUM       ] = &CRunBytecode::rsumOpcode;
   _opcodePointer[OP_ISUB       ] = &CRunBytecode::isubOpcode;
   _opcodePointer[OP_SSUB       ] = &CRunBytecode::ssubOpcode;
   _opcodePointer[OP_RSUB       ] = &CRunBytecode::rsubOpcode;
   _opcodePointer[OP_IMUL       ] = &CRunBytecode::imulOpcode;
   _opcodePointer[OP_RMUL       ] = &CRunBytecode::rmulOpcode;
   _opcodePointer[OP_IDIV       ] = &CRunBytecode::idivOpcode;
   _opcodePointer[OP_RDIV       ] = &CRunBytecode::rdivOpcode;
   _opcodePointer[OP_IMOD       ] = &CRunBytecode::imodOpcode;
   _opcodePointer[OP_RMOD       ] = &CRunBytecode::rmodOpcode;
   _opcodePointer[OP_IGE        ] = &CRunBytecode::igeOpcode;
   _opcodePointer[OP_SGE        ] = &CRunBytecode::sgeOpcode;
   _opcodePointer[OP_RGE        ] = &CRunBytecode::rgeOpcode;
   _opcodePointer[OP_ILE        ] = &CRunBytecode::ileOpcode;
   _opcodePointer[OP_SLE        ] = &CRunBytecode::sleOpcode;
   _opcodePointer[OP_RLE        ] = &CRunBytecode::rleOpcode;
   _opcodePointer[OP_INE        ] = &CRunBytecode::ineOpcode;
   _opcodePointer[OP_SNE        ] = &CRunBytecode::sneOpcode;
   _opcodePointer[OP_RNE        ] = &CRunBytecode::rneOpcode;
   _opcodePointer[OP_IGT        ] = &CRunBytecode::igtOpcode;
   _opcodePointer[OP_SGT        ] = &CRunBytecode::sgtOpcode;
   _opcodePointer[OP_RGT        ] = &CRunBytecode::rgtOpcode;
   _opcodePointer[OP_ILT        ] = &CRunBytecode::iltOpcode;
   _opcodePointer[OP_SLT        ] = &CRunBytecode::sltOpcode;
   _opcodePointer[OP_RLT        ] = &CRunBytecode::rltOpcode;
   _opcodePointer[OP_IEQ        ] = &CRunBytecode::ieqOpcode;
   _opcodePointer[OP_SEQ        ] = &CRunBytecode::seqOpcode;
   _opcodePointer[OP_REQ        ] = &CRunBytecode::reqOpcode;
   _opcodePointer[OP_OR         ] = &CRunBytecode::orOpcode;
   _opcodePointer[OP_AND        ] = &CRunBytecode::andOpcode;
   _opcodePointer[OP_XOR        ] = &CRunBytecode::xorOpcode;
   _opcodePointer[OP_INEG       ] = &CRunBytecode::inegOpcode;
   _opcodePointer[OP_RNEG       ] = &CRunBytecode::rnegOpcode;
   _opcodePointer[OP_NOT        ] = &CRunBytecode::notOpcode;
   _opcodePointer[OP_IINC       ] = &CRunBytecode::iincOpcode;
   _opcodePointer[OP_IDEC       ] = &CRunBytecode::idecOpcode;
   _opcodePointer[OP_I2C        ] = &CRunBytecode::i2cOpcode;
   _opcodePointer[OP_R2C        ] = &CRunBytecode::r2cOpcode;
   _opcodePointer[OP_S2C        ] = &CRunBytecode::s2cOpcode;
   _opcodePointer[OP_B2C        ] = &CRunBytecode::b2cOpcode;
   _opcodePointer[OP_I2R        ] = &CRunBytecode::i2rOpcode;
   _opcodePointer[OP_C2R        ] = &CRunBytecode::c2rOpcode;
   _opcodePointer[OP_S2R        ] = &CRunBytecode::s2rOpcode;
   _opcodePointer[OP_B2R        ] = &CRunBytecode::b2rOpcode;
   _opcodePointer[OP_I2B        ] = &CRunBytecode::i2bOpcode;
   _opcodePointer[OP_C2B        ] = &CRunBytecode::c2bOpcode;
   _opcodePointer[OP_R2B        ] = &CRunBytecode::r2bOpcode;
   _opcodePointer[OP_S2B        ] = &CRunBytecode::s2bOpcode;
   _opcodePointer[OP_I2S        ] = &CRunBytecode::i2sOpcode;
   _opcodePointer[OP_C2S        ] = &CRunBytecode::c2sOpcode;
   _opcodePointer[OP_R2S        ] = &CRunBytecode::r2sOpcode;
   _opcodePointer[OP_B2S        ] = &CRunBytecode::b2sOpcode;
   _opcodePointer[OP_P2S        ] = &CRunBytecode::p2sOpcode;
   _opcodePointer[OP_C2I        ] = &CRunBytecode::c2iOpcode;
   _opcodePointer[OP_R2I        ] = &CRunBytecode::r2iOpcode;
   _opcodePointer[OP_S2I        ] = &CRunBytecode::s2iOpcode;
   _opcodePointer[OP_B2I        ] = &CRunBytecode::b2iOpcode;
   _opcodePointer[OP_ISET       ] = &CRunBytecode::isetOpcode;
   _opcodePointer[OP_SSET       ] = &CRunBytecode::ssetOpcode;
   _opcodePointer[OP_RSET       ] = &CRunBytecode::rsetOpcode;
   _opcodePointer[OP_GETA       ] = &CRunBytecode::getaOpcode;
   _opcodePointer[OP_IGETV      ] = &CRunBytecode::igetvOpcode;
   _opcodePointer[OP_SGETV      ] = &CRunBytecode::sgetvOpcode;
   _opcodePointer[OP_RGETV      ] = &CRunBytecode::rgetvOpcode;
   _opcodePointer[OP_ISETV      ] = &CRunBytecode::isetvOpcode;
   _opcodePointer[OP_SSETV      ] = &CRunBytecode::ssetvOpcode;
   _opcodePointer[OP_RSETV      ] = &CRunBytecode::rsetvOpcode;
   _opcodePointer[OP_JMP        ] = &CRunBytecode::jmpOpcode;
   _opcodePointer[OP_IF         ] = &CRunBytecode::ifOpcode;
   _opcodePointer[OP_IFNOT      ] = &CRunBytecode::ifnotOpcode;
   _opcodePointer[OP_POP        ] = &CRunBytecode::popOpcode;
   _opcodePointer[OP_INCSP      ] = &CRunBytecode::incspOpcode;
   _opcodePointer[OP_DECSP      ] = &CRunBytecode::decspOpcode;

//   OP_PUSH_0,
//   OP_PUSH_2,
//   OP_PUSH_3,
//   OP_PUSH_4,
//   OP_PUSH_5,

   _opcodePointer[OP_PUSH_INT   ] = &CRunBytecode::pushIntOpcode;
   _opcodePointer[OP_PUSH_REAL  ] = &CRunBytecode::pushRealOpcode;
   _opcodePointer[OP_PUSH_CHAR  ] = &CRunBytecode::pushCharOpcode;
   _opcodePointer[OP_PUSH_BOOL  ] = &CRunBytecode::pushBoolOpcode;
   _opcodePointer[OP_PUSH_MATRIX] = &CRunBytecode::pushMatrixOpcode;

   _opcodePointer[OP_PUSH_STYPE ] = &CRunBytecode::pushSTypeOpcode;
   _opcodePointer[OP_PUSH_ITYPE ] = &CRunBytecode::pushITypeOpcode;
   _opcodePointer[OP_PUSH_RTYPE ] = &CRunBytecode::pushRTypeOpcode;
   _opcodePointer[OP_PUSH_CTYPE ] = &CRunBytecode::pushCTypeOpcode;
   _opcodePointer[OP_PUSH_BTYPE ] = &CRunBytecode::pushBTypeOpcode;
   _opcodePointer[OP_PUSH_MTYPE ] = &CRunBytecode::pushMTypeOpcode;

   _opcodePointer[OP_INCSP_4    ] = &CRunBytecode::incsp4Opcode;
//   OP_INCSP_8,
   _opcodePointer[OP_DECSP_4    ] = &CRunBytecode::decsp4Opcode;
//   OP_DECSP_8,

   _opcodePointer[OP_RET        ] = &CRunBytecode::retOpcode;
   _opcodePointer[OP_SALLOC     ] = &CRunBytecode::sallocOpcode;
   _opcodePointer[OP_SFREE      ] = &CRunBytecode::sfreeOpcode;
   _opcodePointer[OP_SSETC      ] = &CRunBytecode::ssetcOpcode;
   _opcodePointer[OP_SGETC      ] = &CRunBytecode::sgetcOpcode;
   _opcodePointer[OP_M1ALLOC    ] = &CRunBytecode::m1allocOpcode;
   _opcodePointer[OP_M2ALLOC    ] = &CRunBytecode::m2allocOpcode;
   _opcodePointer[OP_MFREE      ] = &CRunBytecode::mfreeOpcode;
   _opcodePointer[OP_M1SET      ] = &CRunBytecode::m1setOpcode;
   _opcodePointer[OP_M1GET      ] = &CRunBytecode::m1getOpcode;
   _opcodePointer[OP_M2SET      ] = &CRunBytecode::m2setOpcode;
   _opcodePointer[OP_M2GET      ] = &CRunBytecode::m2getOpcode;
   _opcodePointer[OP_MCOPY      ] = &CRunBytecode::mcopyOpcode;
   _opcodePointer[OP_MGETSIZE1  ] = &CRunBytecode::mgetSize1Opcode;
   _opcodePointer[OP_MGETSIZE2  ] = &CRunBytecode::mgetSize2Opcode;
}


int CRunBytecode::run()
{
   //std::cout << "Code lido: [" << _code.getBinary() << "]" << " size=" << _code.getBinary().size() << std::endl;
//   std::cout << "Code size=" << _code.size() << std::endl;

   initOpcodePointer();
   _code.setIP(0); // TODO: pegar o endereco de main
   _stop = false;

   while (!_stop) {
      step();
   }

   return _returnCode;
}


void CRunBytecode::step()
{
    char opcode;
    opcode = _code.fetchByte();

   if (opcode >= OPCODE_NUMBER) {
      error( "Invalid opcode !!!" );
   }

    (this->*_opcodePointer[(int)opcode]) ( );
}

void CRunBytecode::trace(const std::string &message)
{
   //std::cout << message << std::endl;
}

void CRunBytecode::error(const std::string &message)
{
   std::cerr << message << std::endl;
   abort();
}

void CRunBytecode::procImprima()
{
   int argNumber = _dataStack.popInt();

   for (int arg=0; arg < argNumber; arg++) {
      int type = _dataStack.popInt();
      int boolValue;
      switch (type) {
         case CSymbol::STRING:
            std::cout << getStringData(_dataStack.popInt());
            break;
         case CSymbol::INT:
            std::cout << _dataStack.popInt();
            break;
         case CSymbol::CHAR:
            std::cout << (char)_dataStack.popInt();
            break;
         case CSymbol::BOOL:
            boolValue = _dataStack.popInt();
            if (boolValue == 0) {
               std::cout << "false";
            } else {
               std::cout << "true";
            }
            break;
         case CSymbol::REAL:
         case CSymbol::MATRIX:
         default:
            std::cout << "Tipo ainda nao suportado !!!" << std::endl;
            abort();
      }
   }
   std::cout << std::endl;
}

/////////////
// opcodes //
/////////////

void CRunBytecode::invalidOpcode(const std::string &opcode)
{
   error ("Invalid opcode: [" + opcode + "] !!!");
}

void CRunBytecode::nopOpcode()
{
   trace ("nop opcode");

   // nothing to do
}

void CRunBytecode::pushSregOpcode()
{
   trace ("push_sreg opcode");

   _executionStack.push(_dataStack.getBS());
   _executionStack.push(_dataStack.getSP());
}

void CRunBytecode::popSregOpcode()
{
   trace ("pop_sreg opcode");

   _dataStack.setSP(_executionStack.top());
   _executionStack.pop();

   _dataStack.setBS(_executionStack.top());
   _executionStack.pop();
}


void CRunBytecode::pushStringOpcode()
{
   trace ("push_string opcode");

   int address = _code.fetchInt();

   _dataStack.pushInt(address);
}

void CRunBytecode::pushSTypeOpcode()
{
   trace ("push_stype opcode");

   _dataStack.pushInt(CSymbol::STRING);
}


void CRunBytecode::pcallOpcode()
{
   trace ("pcall opcode");

   _dataStack.setBS(_executionStack.top()); // TODO: verificar melhor isso...

   int address = _code.fetchInt();

   _executionStack.push(_code.getIP());
   _code.setIP(address);
}


void CRunBytecode::libcallOpcode()
{
   trace ("libcall opcode");

   int address = _code.fetchInt();

   if (_globalData.getCString(address) == "imprima") {
      procImprima();
   } else {
      error("libcall invocando subrotina desconhecida !!!");
   }
}


void CRunBytecode::exitOpcode()
{
   trace ("exit opcode");

   _returnCode = getIntData(_code.fetchInt());

   _stop = true;
}


void CRunBytecode::hltOpcode()
{
   trace ("hlt opcode");

   exit(0);
}

void CRunBytecode::isumOpcode()
{
   trace ("isum opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) + getIntData(val2Address));
}

void CRunBytecode::ssumOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rsumOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::isubOpcode()
{
   trace ("isub opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) - getIntData(val2Address));
}

void CRunBytecode::ssubOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rsubOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::imulOpcode()
{
   trace ("imul opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) * getIntData(val2Address));
}

void CRunBytecode::rmulOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::idivOpcode()
{
   trace ("idiv opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) / getIntData(val2Address));
}

void CRunBytecode::rdivOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::imodOpcode()
{
   trace ("imod opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) % getIntData(val2Address));
}

void CRunBytecode::rmodOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::igeOpcode()
{
   trace ("ige opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) >= getIntData(val2Address));
}

void CRunBytecode::sgeOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rgeOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::ileOpcode()
{
   trace ("ile opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) <= getIntData(val2Address));
}

void CRunBytecode::sleOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rleOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::ineOpcode()
{
   trace ("ine opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) != getIntData(val2Address));
}

void CRunBytecode::sneOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rneOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::igtOpcode()
{
   trace ("igt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) > getIntData(val2Address));
}

void CRunBytecode::sgtOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rgtOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::iltOpcode()
{
   trace ("ilt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) < getIntData(val2Address));
}

void CRunBytecode::sltOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rltOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::ieqOpcode()
{
   trace ("ieq opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) == getIntData(val2Address));
}

void CRunBytecode::seqOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::reqOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::orOpcode()
{
   trace ("or opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) || getIntData(val2Address));
}

void CRunBytecode::andOpcode()
{
   trace ("and opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   setIntData(varAddress, getIntData(val1Address) && getIntData(val2Address));
}

void CRunBytecode::xorOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::inegOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rnegOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::notOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::iincOpcode()
{
   trace ("iinc opcode");

   int varAddress = _code.fetchInt();
   int valAddress = _code.fetchInt();

   setIntData(varAddress, getIntData(varAddress) + getIntData(valAddress));
}

void CRunBytecode::idecOpcode()
{
   trace ("idec opcode");

   int varAddress = _code.fetchInt();
   int valAddress = _code.fetchInt();

   setIntData(varAddress, getIntData(varAddress) - getIntData(valAddress));
}

void CRunBytecode::i2cOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::r2cOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::s2cOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::b2cOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::i2rOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::c2rOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::s2rOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::b2rOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::i2bOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::c2bOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::r2bOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::s2bOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::i2sOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::c2sOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::r2sOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::b2sOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::p2sOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::c2iOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::r2iOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::s2iOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::b2iOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::isetOpcode()
{
   trace ("iset opcode");

   int varAddress = _code.fetchInt();
   int valAddress = _code.fetchInt();

   setIntData(varAddress, getIntData(valAddress));
}


void CRunBytecode::setIntData(const int &address, const int &value)
{
   if (address & 0x80000000) {
      _dataStack.setInt(address, value);
   } else {
//      std::cout << "setIntData global address: " << address << std::endl;
      _globalData.setInt(address, value); // TODO: _globalData ou _globalData ???
   }
}


int CRunBytecode::getIntData(const int &address)
{
   if (address & 0x80000000) {
      return _dataStack.getInt(address);
   } else {
//      std::cout << "global address: " << address << std::endl;
      return _globalData.getInt(address);
   }
}


void CRunBytecode::setStringData(const int &address, const std::string &value)
{
   if (address & 0x80000000) {
      _dataStack.setCString(address, value);
   } else {
//      std::cout << "setCStringData global address: " << address << std::endl;
      _globalData.setCString(address, value);
   }
}


std::string CRunBytecode::getStringData(const int &address)
{
   if (address & 0x80000000) {
      return _dataStack.getCString(address);
   } else {
//      std::cout << "global address: " << address << std::endl;
      return _globalData.getCString(address);
   }
}


void CRunBytecode::ssetOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rsetOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::getaOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::igetvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::sgetvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rgetvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::isetvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::ssetvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rsetvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::jmpOpcode()
{
   trace ("jmp opcode");

   int labelAddress = _code.fetchInt();

   _code.setIP(labelAddress);
}

void CRunBytecode::ifOpcode()
{
   trace ("if opcode");

   int varAddress = _code.fetchInt();
   int labelAddress = _code.fetchInt();

   if (getIntData(varAddress)) {
      _code.setIP(labelAddress);
   }
}

void CRunBytecode::ifnotOpcode()
{
   trace ("ifnot opcode");

   int varAddress = _code.fetchInt();
   int labelAddress = _code.fetchInt();

   if (!getIntData(varAddress)) {
      _code.setIP(labelAddress);
   }
}

void CRunBytecode::popOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::incspOpcode()
{
   trace ("incsp opcode");

   int valAddress = _code.fetchInt();
   int size = getIntData(valAddress);

   _dataStack.pushBytes(size);
}

void CRunBytecode::decspOpcode()
{
   trace ("decsp opcode");

   int size = getIntData(_code.fetchInt());

   _dataStack.popBytes(size);
}


//   OP_PUSH_0,
//   OP_PUSH_2,
//   OP_PUSH_3,
//   OP_PUSH_4,
//   OP_PUSH_5,

void CRunBytecode::pushIntOpcode()
{
   trace ("push_int opcode");

   int address = _code.fetchInt();

   _dataStack.pushInt(getIntData(address));
}

void CRunBytecode::pushRealOpcode()
{
   trace ("push_real opcode");

   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::pushCharOpcode()
{
   trace ("push_char opcode");

   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::pushBoolOpcode()
{
   trace ("push_bool opcode");

   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::pushMatrixOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::pushITypeOpcode()
{
   trace ("push_itype opcode");

   _dataStack.pushInt(CSymbol::INT);
}

void CRunBytecode::pushRTypeOpcode()
{
   trace ("push_rtype opcode");

   _dataStack.pushInt(CSymbol::REAL);
}

void CRunBytecode::pushCTypeOpcode()
{
   trace ("push_ctype opcode");

   _dataStack.pushInt(CSymbol::CHAR);
}

void CRunBytecode::pushBTypeOpcode()
{
   trace ("push_btype opcode");

   _dataStack.pushInt(CSymbol::BOOL);
}

void CRunBytecode::pushMTypeOpcode()
{
   invalidOpcode(__FUNCTION__);
}


void CRunBytecode::incsp4Opcode()
{
   trace ("incsp_4 opcode");

   _dataStack.pushInt(0);
}

//   OP_INCSP_8,

void CRunBytecode::decsp4Opcode()
{
   trace ("decsp_4 opcode");

   _dataStack.popInt();
}

//   OP_DECSP_8,

void CRunBytecode::retOpcode()
{
   trace ("ret opcode");

   _code.setIP(_executionStack.top());
   _executionStack.pop();
}

void CRunBytecode::sallocOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::sfreeOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::ssetcOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::sgetcOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::m1allocOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::m2allocOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::mfreeOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::m1setOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::m1getOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::m2setOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::m2getOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::mcopyOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::mgetSize1Opcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::mgetSize2Opcode()
{
   invalidOpcode(__FUNCTION__);
}

