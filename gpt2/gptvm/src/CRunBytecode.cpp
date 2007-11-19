#include "CRunBytecode.hpp"

#include <sstream>
#include <iostream>


CRunBytecode::CRunBytecode()
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

   return _header.readFromBinary(bin) &&
          _symbolTable.readFromBinary(bin) &&
          _data.readFromBinary(bin) &&
          _code.readFromBinary(bin);
}

void CRunBytecode::initOpcodePointer()
{
   for (int i = 0; i < OPCODE_NUMBER; i++) {
      _opcodePointer[i] = &CRunBytecode::invalidOpcode;
   }

   _opcodePointer[OP_NOP         ] = &CRunBytecode::nopOpcode;
   _opcodePointer[OP_PUSH_SP     ] = &CRunBytecode::pushSpOpcode;
   _opcodePointer[OP_POP_SP      ] = &CRunBytecode::popSpOpcode;
   _opcodePointer[OP_PUSH        ] = &CRunBytecode::pushOpcode;
   _opcodePointer[OP_PUSH_STRING ] = &CRunBytecode::pushStringOpcode;
//   _opcodePointer[OP_PUSH_1      ] = &CRunBytecode::push1Opcode;
   _opcodePointer[OP_PCALL       ] = &CRunBytecode::pcallOpcode;
//   _opcodePointer[OP_EXIT_0      ] = &CRunBytecode::exit0Opcode;
   _opcodePointer[OP_EXIT        ] = &CRunBytecode::exitOpcode;

//   OP_EXIT_1,
// _opcodePointer[OP_HLT         ] = &CRunBytecode::hltOpcode;

   _opcodePointer[OP_ISUM        ] = &CRunBytecode::isumOpcode;
//   _opcodePointer[OP_SSUM        ] = &CRunBytecode::ssumOpcode;
//   _opcodePointer[OP_RSUM        ] = &CRunBytecode::rsumOpcode;
//   _opcodePointer[OP_ISUB        ] = &CRunBytecode::isubOpcode;
//   _opcodePointer[OP_SSUB        ] = &CRunBytecode::ssubOpcode;
//   _opcodePointer[OP_RSUB        ] = &CRunBytecode::rsubOpcode;
   _opcodePointer[OP_IMUL        ] = &CRunBytecode::imulOpcode;
//   _opcodePointer[OP_RMUL        ] = &CRunBytecode::rmulOpcode;
/*
   OP_IDIV,
   OP_RDIV,
   OP_IMOD,
   OP_RMOD,
   OP_IGE,
   OP_SGE,
   OP_RGE,
   OP_ILE,
   OP_SLE,
   OP_RLE,
   OP_INE,
   OP_SNE,
   OP_RNE,
   OP_IGT,
   OP_SGT,
   OP_RGT,
   OP_ILT,
   OP_SLT,
   OP_RLT,
   OP_IEQ,
   OP_SEQ,
   OP_REQ,
   OP_OR,
   OP_AND,
   OP_XOR,
   OP_INEG,
   OP_RNEG,
   OP_NOT,
   OP_IINC,
   OP_IDEC,
   OP_I2C,
   OP_R2C,
   OP_S2C,
   OP_B2C,
   OP_I2R,
   OP_C2R,
   OP_S2R,
   OP_B2R,
   OP_I2B,
   OP_C2B,
   OP_R2B,
   OP_S2B,
   OP_I2S,
   OP_C2S,
   OP_R2S,
   OP_B2S,
   OP_P2S,
   OP_C2I,
   OP_R2I,
   OP_S2I,
   OP_B2I,
*/
   _opcodePointer[OP_ISET        ] = &CRunBytecode::isetOpcode;
/*
   OP_SSET,
   OP_RSET,
   OP_GETA,
   OP_IGETV,
   OP_SGETV,
   OP_RGETV,
   OP_ISETV,
   OP_SSETV,
   OP_RSETV,
   OP_JMP,
   OP_IF,
   OP_IFNOT,
   OP_POP,
   OP_INCSP,
   OP_DECSP,
   OP_PUSH_0,
   OP_PUSH_2,
   OP_PUSH_3,
   OP_PUSH_4,
   OP_PUSH_5,
*/
   _opcodePointer[OP_PUSH_INT      ] = &CRunBytecode::pushIntOpcode;
   _opcodePointer[OP_PUSH_REAL     ] = &CRunBytecode::pushRealOpcode;
   _opcodePointer[OP_PUSH_CHAR     ] = &CRunBytecode::pushCharOpcode;
   _opcodePointer[OP_PUSH_BOOL     ] = &CRunBytecode::pushBoolOpcode;
/*
   OP_PUSH_MATRIX,
   OP_INCSP_4,
   OP_INCSP_8,
   OP_DECSP_4,
   OP_DECSP_8,
   OP_RET,
   OP_SALLOC,
   OP_SFREE,
   OP_SSETC,
   OP_SGETC,
   OP_M1ALLOC,
   OP_M2ALLOC,
   OP_MFREE,
   OP_M1SET,
   OP_M1GET,
   OP_M2SET,
   OP_M2GET,
   OP_MCOPY,
   OP_MGETSIZE1,
   OP_MGETSIZE2,
*/
}


bool CRunBytecode::run()
{
//   std::cout << "Code lido: [" << _code.getBinary() << "]" << " size=" << _code.getBinary().size() << std::endl;

   initOpcodePointer();
   _IP = 0;
   _stop = false;

   while (!_stop) {
      step();
   }

   return true;
}

void CRunBytecode::step()
{
    char opcode;
    opcode = _code.getByte(_IP);
    _IP++;

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
   int argNumber = _data.getInt((int)_dataStack.top());
   _dataStack.pop();

   for (int arg=0; arg < argNumber; arg++) {
      int type = (int)_dataStack.top();
      _dataStack.pop();
      int address = (int)_dataStack.top();
      _dataStack.pop();
      int value;
      switch (type) {
         case CSymbol::STRING:
            std::cout << _data.getString(address);
            break;
         case CSymbol::INT:
            std::cout << _data.getInt(address);
            break;
         case CSymbol::CHAR:
            std::cout << (char)_data.getInt(address);
            break;
         case CSymbol::BOOL:
            value = _data.getInt(address);
            if (value == 0) {
               std::cout << "false";
            } else {
               std::cout << "true";
            }
            break;
         case CSymbol::REAL:
         case CSymbol::MATRIX:
         default:
            std::cout << "Tipo nao suportado !!!" << std::endl;
            abort();
      }
   }
   std::cout << std::endl;
}

// opcodes

void CRunBytecode::invalidOpcode()
{
   error ("Invalid opcode !!!");
}

void CRunBytecode::nopOpcode()
{
//   trace ("nop opcode");
   invalidOpcode();

    //nothing to do
}

void CRunBytecode::pushSpOpcode()
{
   trace ("push_sp opcode");
   // TODO
}

void CRunBytecode::popSpOpcode()
{
   trace ("pop_sp opcode");
   // TODO
}

void CRunBytecode::pushOpcode()
{
   trace ("push opcode");

   int address = _code.getInt(_IP);
   _IP += sizeof(int);
   _dataStack.push((void*)address);
}

void CRunBytecode::pushStringOpcode()
{
   trace ("push_string opcode");

   _dataStack.push((void*)CSymbol::STRING);
}

void CRunBytecode::pushIntOpcode()
{
   trace ("push_int opcode");

   _dataStack.push((void*)CSymbol::INT);
}

void CRunBytecode::pushBoolOpcode()
{
   trace ("push_bool opcode");

   _dataStack.push((void*)CSymbol::BOOL);
}

void CRunBytecode::pushRealOpcode()
{
   trace ("push_real opcode");

   _dataStack.push((void*)CSymbol::REAL);
}

void CRunBytecode::pushCharOpcode()
{
   trace ("push_char opcode");

   _dataStack.push((void*)CSymbol::CHAR);
}

void CRunBytecode::push1Opcode()
{
   trace ("push_1 opcode");

   _dataStack.push((void*)1);
}

void CRunBytecode::pcallOpcode()
{
   trace ("pcall opcode");

   int address = _code.getInt(_IP);
   _IP += sizeof(int);

   std::string temp = _data.getString(address);

   if (_data.getString(address) == "imprima") {
      procImprima();
   }
}


void CRunBytecode::exitOpcode()
{
   trace ("exit opcode");

   int returnCode = _code.getInt(_IP); // TODO
   _IP += sizeof(int);

   _stop = true;
}


void CRunBytecode::isetOpcode()
{
   trace ("iset opcode");

   int varAddress = _code.getInt(_IP);
   _IP += sizeof(int);

   int valAddress = _code.getInt(_IP);
   _IP += sizeof(int);

   _data.setInt(varAddress, _data.getInt(valAddress));
}


void CRunBytecode::isumOpcode()
{
   trace ("isum opcode");

   int varAddress = _code.getInt(_IP);
   _IP += sizeof(int);

   int val1Address = _code.getInt(_IP);
   _IP += sizeof(int);

   int val2Address = _code.getInt(_IP);
   _IP += sizeof(int);

   _data.setInt(varAddress, _data.getInt(val1Address) + _data.getInt(val2Address));
}


void CRunBytecode::imulOpcode()
{
   trace ("imul opcode");

   int varAddress = _code.getInt(_IP);
   _IP += sizeof(int);

   int val1Address = _code.getInt(_IP);
   _IP += sizeof(int);

   int val2Address = _code.getInt(_IP);
   _IP += sizeof(int);

   _data.setInt(varAddress, _data.getInt(val1Address) * _data.getInt(val2Address));
}

