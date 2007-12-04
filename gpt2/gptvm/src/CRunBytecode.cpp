#include "CRunBytecode.hpp"

#include <sstream>
#include <iostream>

#include "Tools.hpp"


// TODO: ao somar dados em address os limites deveriam ser testados
// TODO: Armazenar todas as strings em memoria com o tamanho na frente ???

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

   _dataStack.assign(bin.readString());
   _dataStack.setBS(_dataStack.size());
   _dataStack.setSP(_dataStack.size());

   _code.assign(bin.readString());

   return true;
}

void CRunBytecode::initOpcodePointer()
{
   for (int i = 0; i < OPCODE_NUMBER; i++) {
      _opcodePointer[i] = NULL; //&CRunBytecode::invalidOpcode;
   }

   _opcodePointer[OP_NOP        ] = &CRunBytecode::nopOpcode;
   _opcodePointer[OP_PCALL      ] = &CRunBytecode::pcallOpcode;
   _opcodePointer[OP_LCALL      ] = &CRunBytecode::lcallOpcode;
   _opcodePointer[OP_EXIT       ] = &CRunBytecode::exitOpcode;
   _opcodePointer[OP_EXIT_0     ] = &CRunBytecode::exit_0Opcode;
   _opcodePointer[OP_EXIT_1     ] = &CRunBytecode::exit_1Opcode;

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
   _opcodePointer[OP_POPSV      ] = &CRunBytecode::popsvOpcode;
   _opcodePointer[OP_POPIV      ] = &CRunBytecode::popivOpcode;
   _opcodePointer[OP_POPRV      ] = &CRunBytecode::poprvOpcode;
   _opcodePointer[OP_POPDV      ] = &CRunBytecode::popdvOpcode;
   _opcodePointer[OP_POPMV      ] = &CRunBytecode::popmvOpcode;
   _opcodePointer[OP_INCSP      ] = &CRunBytecode::incspOpcode;
   _opcodePointer[OP_DECSP      ] = &CRunBytecode::decspOpcode;
   _opcodePointer[OP_PUSH_0     ] = &CRunBytecode::push_0Opcode;
   _opcodePointer[OP_PUSH_1     ] = &CRunBytecode::push_1Opcode;
   _opcodePointer[OP_PUSH_2     ] = &CRunBytecode::push_2Opcode;
   _opcodePointer[OP_PUSH_3     ] = &CRunBytecode::push_3Opcode;
   _opcodePointer[OP_PUSH_4     ] = &CRunBytecode::push_4Opcode;
   _opcodePointer[OP_PUSH_5     ] = &CRunBytecode::push_5Opcode;
   _opcodePointer[OP_PUSHSV     ] = &CRunBytecode::pushsvOpcode;
   _opcodePointer[OP_PUSHIV     ] = &CRunBytecode::pushivOpcode;
   _opcodePointer[OP_PUSHRV     ] = &CRunBytecode::pushrvOpcode;
   _opcodePointer[OP_PUSHDV     ] = &CRunBytecode::pushdvOpcode;
   _opcodePointer[OP_PUSHMV     ] = &CRunBytecode::pushmvOpcode;

   _opcodePointer[OP_PUSHST     ] = &CRunBytecode::pushstOpcode;
   _opcodePointer[OP_PUSHIT     ] = &CRunBytecode::pushitOpcode;
   _opcodePointer[OP_PUSHRT     ] = &CRunBytecode::pushrtOpcode;
   _opcodePointer[OP_PUSHCT     ] = &CRunBytecode::pushctOpcode;
   _opcodePointer[OP_PUSHBT     ] = &CRunBytecode::pushbtOpcode;
   _opcodePointer[OP_PUSHDT     ] = &CRunBytecode::pushdtOpcode;
   _opcodePointer[OP_PUSHMT     ] = &CRunBytecode::pushmtOpcode;

   _opcodePointer[OP_INCSP_4     ] = &CRunBytecode::incsp_4Opcode;
   _opcodePointer[OP_INCSP_8     ] = &CRunBytecode::incsp_8Opcode;
   _opcodePointer[OP_DECSP_4     ] = &CRunBytecode::decsp_4Opcode;
   _opcodePointer[OP_DECSP_8     ] = &CRunBytecode::decsp_8Opcode;

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
   int address = sizeof(int);
   int argNumber = _dataStack.getInt(address|SET_LOCAL_BIT|SET_NEG_BIT);

   for (int arg=0; arg < argNumber; arg++) {
      address += sizeof(int);
      int type = _dataStack.getInt(address|SET_LOCAL_BIT|SET_NEG_BIT);
      int boolValue;
      switch (type) {
         case CSymbol::STRING:
            address += sizeof(char) + sizeof(int);
            std::cout << _dataStack.getString(address|SET_LOCAL_BIT|SET_NEG_BIT);
            break;
         case CSymbol::INT:
            address += sizeof(int);
            std::cout << _dataStack.getInt(address|SET_LOCAL_BIT|SET_NEG_BIT);
            break;
         case CSymbol::CHAR:
//            std::cout << (char)_dataStack.popInt();
            address += sizeof(int);
            std::cout << "char [" << (int)_dataStack.getInt(address|SET_LOCAL_BIT|SET_NEG_BIT) << "]";
            break;
         case CSymbol::BOOL:
            address += sizeof(int);
            boolValue = _dataStack.getInt(address|SET_LOCAL_BIT|SET_NEG_BIT);
            if (boolValue == 0) {
               std::cout << "false";
            } else {
               std::cout << "true";
            }
            break;
         case CSymbol::REAL:
            address += sizeof(double);
            std::cout << _dataStack.getReal(address|SET_LOCAL_BIT|SET_NEG_BIT);
            break;
         case CSymbol::DATA:
         case CSymbol::MATRIX:
         default:
            std::cout << "Tipo ainda nao suportado !!!" << std::endl;
            abort();
      }
   }
   std::cout << std::endl;
   _dataStack.discardBytes(address);
}


void CRunBytecode::procLeia()
{
   int type = _dataStack.popInt();
   int iValue = 0;
   double dValue = 0.0;
   switch (type) {
//      case CSymbol::STRING:
//         _dataStack.pushString(std::cin);
//         break;
      case CSymbol::INT:
         std::cin >> iValue;
         _dataStack.pushInt(iValue);
         break;
//      case CSymbol::CHAR:
//         std::cout << (char)_dataStack.popInt();
//         break;
//      case CSymbol::BOOL:
//         boolValue = _dataStack.popInt();
//         if (boolValue == 0) {
//            std::cout << "false";
//         } else {
//            std::cout << "true";
//         }
//         break;
      case CSymbol::REAL:
         std::cin >> dValue;
         _dataStack.pushReal(dValue);
         break;
      case CSymbol::MATRIX:
      default:
         std::cout << "Tipo ainda nao suportado !!!" << std::endl;
         abort();
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


void CRunBytecode::pcallOpcode()
{
   trace ("pcall opcode");

   _executionStack.push(_dataStack.getBS());
//   _executionStack.push(_dataStack.getSP());

   _dataStack.setBS(_dataStack.getSP());

   int address = _code.fetchInt();

   _executionStack.push(_code.getIP());
   _code.setIP(address);
}


void CRunBytecode::lcallOpcode()
{
   trace ("lcall opcode");

   _executionStack.push(_dataStack.getBS());
   _dataStack.setBS(_dataStack.getSP());

   int address = _code.fetchInt();

   if (_dataStack[address] != CSymbol::CONST) {
      error( "Endereco para lcall deve conter uma string constante !!!" );
   }

   address=sumAddress(address,1);

   if (_dataStack.getCString(address) == "imprima") {
      procImprima();
   } else if (_dataStack.getCString(address) == "leia") {
      procLeia();
   } else {
      error("lcall invocando subrotina desconhecida !!!");
   }

   _dataStack.setBS(_executionStack.top());
   _executionStack.pop();
}


void CRunBytecode::exitOpcode()
{
   trace ("exit opcode");

   _returnCode = _dataStack.getInt(_code.fetchInt());
   _stop       = true;
}


void CRunBytecode::exit_0Opcode()
{
   trace ("exit_0 opcode");

   _returnCode = 0;
   _stop       = true;
}


void CRunBytecode::exit_1Opcode()
{
   trace ("exit_1 opcode");

   _returnCode = 1;
   _stop       = true;
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

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) + _dataStack.getInt(val2Address));
}

void CRunBytecode::ssumOpcode()
{
   trace ("ssum opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setString(varAddress, _dataStack.getString(val1Address) + _dataStack.getString(val2Address));
}

void CRunBytecode::rsumOpcode()
{
   trace ("rsum opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setReal(varAddress, _dataStack.getReal(val1Address) + _dataStack.getReal(val2Address));
}

void CRunBytecode::isubOpcode()
{
   trace ("isub opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) - _dataStack.getInt(val2Address));
}

void CRunBytecode::ssubOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::rsubOpcode()
{
   trace ("rsub opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setReal(varAddress, _dataStack.getReal(val1Address) - _dataStack.getReal(val2Address));
}

void CRunBytecode::imulOpcode()
{
   trace ("imul opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) * _dataStack.getInt(val2Address));
}

void CRunBytecode::rmulOpcode()
{
   trace ("rmul opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setReal(varAddress, _dataStack.getReal(val1Address) * _dataStack.getReal(val2Address));
}

void CRunBytecode::idivOpcode()
{
   trace ("idiv opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) / _dataStack.getInt(val2Address));
}

void CRunBytecode::rdivOpcode()
{
   trace ("rdiv opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setReal(varAddress, _dataStack.getReal(val1Address) / _dataStack.getReal(val2Address));
}

void CRunBytecode::imodOpcode()
{
   trace ("imod opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) % _dataStack.getInt(val2Address));
}


void CRunBytecode::igeOpcode()
{
   trace ("ige opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) >= _dataStack.getInt(val2Address));
}

void CRunBytecode::sgeOpcode()
{
   trace ("sge opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getString(val1Address) >= _dataStack.getString(val2Address));
}

void CRunBytecode::rgeOpcode()
{
   trace ("rge opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getReal(val1Address) >= _dataStack.getReal(val2Address));
}

void CRunBytecode::ileOpcode()
{
   trace ("ile opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) <= _dataStack.getInt(val2Address));
}

void CRunBytecode::sleOpcode()
{
   trace ("ile opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getString(val1Address) <= _dataStack.getString(val2Address));
}

void CRunBytecode::rleOpcode()
{
   trace ("rle opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getReal(val1Address) <= _dataStack.getReal(val2Address));
}

void CRunBytecode::ineOpcode()
{
   trace ("ine opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) != _dataStack.getInt(val2Address));
}

void CRunBytecode::sneOpcode()
{
   trace ("sne opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getString(val1Address) != _dataStack.getString(val2Address));
}

void CRunBytecode::rneOpcode()
{
   trace ("rne opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getReal(val1Address) != _dataStack.getReal(val2Address));
}

void CRunBytecode::igtOpcode()
{
   trace ("igt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) > _dataStack.getInt(val2Address));
}

void CRunBytecode::sgtOpcode()
{
   trace ("sgt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getString(val1Address) > _dataStack.getString(val2Address));
}

void CRunBytecode::rgtOpcode()
{
   trace ("rgt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getReal(val1Address) > _dataStack.getReal(val2Address));
}

void CRunBytecode::iltOpcode()
{
   trace ("ilt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) < _dataStack.getInt(val2Address));
}

void CRunBytecode::sltOpcode()
{
   trace ("slt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getString(val1Address) < _dataStack.getString(val2Address));
}

void CRunBytecode::rltOpcode()
{
   trace ("rlt opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getReal(val1Address) < _dataStack.getReal(val2Address));
}

void CRunBytecode::ieqOpcode()
{
   trace ("ieq opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) == _dataStack.getInt(val2Address));
}

void CRunBytecode::seqOpcode()
{
   trace ("seq opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getString(val1Address) == _dataStack.getString(val2Address));
}

void CRunBytecode::reqOpcode()
{
   trace ("req opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getReal(val1Address) == _dataStack.getReal(val2Address));
}

void CRunBytecode::orOpcode()
{
   trace ("or opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) || _dataStack.getInt(val2Address));
}

void CRunBytecode::andOpcode()
{
   trace ("and opcode");

   int varAddress  = _code.fetchInt();
   int val1Address = _code.fetchInt();
   int val2Address = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(val1Address) && _dataStack.getInt(val2Address));
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

   _dataStack.setInt(varAddress, _dataStack.getInt(varAddress) + _dataStack.getInt(valAddress));
}

void CRunBytecode::idecOpcode()
{
   trace ("idec opcode");

   int varAddress = _code.fetchInt();
   int valAddress = _code.fetchInt();

   _dataStack.setInt(varAddress, _dataStack.getInt(varAddress) - _dataStack.getInt(valAddress));
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
   trace ("i2r opcode");

   int var1Address = _code.fetchInt();
   int var2Address = _code.fetchInt();

   _dataStack.setReal(var1Address, (double)_dataStack.getInt(var2Address));
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
   trace ("r2i opcode");

   int var1Address = _code.fetchInt();
   int var2Address = _code.fetchInt();

   _dataStack.setInt(var1Address, (int)_dataStack.getReal(var2Address));
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

   _dataStack.setInt(varAddress, _dataStack.getInt(valAddress));
}


void CRunBytecode::ssetOpcode()
{
   trace ("sset opcode");

   int varAddress = _code.fetchInt();
   int valAddress = _code.fetchInt();

   _dataStack.setString(varAddress, _dataStack.getString(valAddress));
}

void CRunBytecode::rsetOpcode()
{
   trace ("rset opcode");

   int varAddress = _code.fetchInt();
   int valAddress = _code.fetchInt();

   _dataStack.setReal(varAddress, _dataStack.getReal(valAddress));
}

void CRunBytecode::getaOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::igetvOpcode()
{
   trace ("igetv opcode");

   int resultAddress = _code.fetchInt();
   int varAddress    = _code.fetchInt();
   int offset        = _dataStack.getInt(_code.fetchInt());

   _dataStack.setInt(resultAddress, _dataStack.getInt(sumAddress(varAddress,offset)));
}

void CRunBytecode::sgetvOpcode()
{
   trace ("sgetv opcode");

   int resultAddress = _code.fetchInt();
   int varAddress    = _code.fetchInt();
   int offset        = _dataStack.getInt(_code.fetchInt());

   _dataStack.setBytes(resultAddress, _dataStack.getBytes(sumAddress(varAddress,offset), 5));
}

void CRunBytecode::rgetvOpcode()
{
   trace ("rgetv opcode");

   int resultAddress = _code.fetchInt();
   int varAddress    = _code.fetchInt();
   int offset        = _dataStack.getInt(_code.fetchInt());

   _dataStack.setReal(resultAddress, _dataStack.getReal(sumAddress(varAddress,offset)));
}

void CRunBytecode::isetvOpcode()
{
   trace ("isetv opcode");

   int varAddress    = _code.fetchInt();
   int offset        = _dataStack.getInt(_code.fetchInt());
   int value         = _dataStack.getInt(_code.fetchInt());

   _dataStack.setInt(sumAddress(varAddress,offset), value);
}

void CRunBytecode::ssetvOpcode()
{
   trace ("ssetv opcode");

   int varAddress    = _code.fetchInt();
   int offset        = _dataStack.getInt(_code.fetchInt());
   int valueAddress  = _code.fetchInt();

   _dataStack.setBytes(sumAddress(varAddress,offset), _dataStack.getBytes(valueAddress, 5));
}

void CRunBytecode::rsetvOpcode()
{
   trace ("rsetv opcode");

   int varAddress = _code.fetchInt();
   int offset     = _dataStack.getInt(_code.fetchInt());
   double value   = _dataStack.getReal(_code.fetchInt());

   _dataStack.setReal(sumAddress(varAddress,offset), value);
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

   if (_dataStack.getInt(varAddress)) {
      _code.setIP(labelAddress);
   }
}

void CRunBytecode::ifnotOpcode()
{
   trace ("ifnot opcode");

   int varAddress = _code.fetchInt();
   int labelAddress = _code.fetchInt();

   if (!_dataStack.getInt(varAddress)) {
      _code.setIP(labelAddress);
   }
}

void CRunBytecode::popsvOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::popivOpcode()
{
   trace ("popiv opcode");

   int address = _code.fetchInt();

   _dataStack.setInt(address, _dataStack.popInt());
}

void CRunBytecode::poprvOpcode()
{
   trace ("poprv opcode");

   int address = _code.fetchInt();

   _dataStack.setReal(address, _dataStack.popReal());
}

void CRunBytecode::popdvOpcode()
{
   trace ("popdv opcode");

   int varAddress  = _code.fetchInt();
   int sizeAddress = _code.fetchInt();

   _dataStack.setBytes(varAddress, _dataStack.popBytes(_dataStack.getInt(sizeAddress)));
}

void CRunBytecode::popmvOpcode()
{
   trace ("popmv opcode");

   int matrixAddress = _code.fetchInt();

   char *retMatrix = (char*) _dataStack.popInt();
   int elementSize = *((int*)(retMatrix+0));
   int elements    = *((int*)(retMatrix+sizeof(int)));

   int size = sizeof(int) + sizeof(int) + elements*elementSize;
   char *matrix = (char*) _dataStack.getInt(matrixAddress);

   if (matrix) {
      delete []matrix;
   }
   matrix = new char[size];
   memcpy(matrix, retMatrix, size);
   delete []retMatrix;
   _dataStack.setInt(matrixAddress, (int)matrix);
}

void CRunBytecode::incspOpcode()
{
   trace ("incsp opcode");

   int valAddress = _code.fetchInt();
   int size = _dataStack.getInt(valAddress);

   _dataStack.pushBytes(size);
}

void CRunBytecode::decspOpcode()
{
   trace ("decsp opcode");

   int size = _dataStack.getInt(_code.fetchInt());

   _dataStack.discardBytes(size);
}


void CRunBytecode::push_0Opcode()
{
   trace ("push_0 opcode");

   _dataStack.pushInt(0);
}


void CRunBytecode::push_1Opcode()
{
   trace ("push_1 opcode");

   _dataStack.pushInt(1);
}


void CRunBytecode::push_2Opcode()
{
   trace ("push_2 opcode");

   _dataStack.pushInt(2);
}


void CRunBytecode::push_3Opcode()
{
   trace ("push_3 opcode");

   _dataStack.pushInt(3);
}


void CRunBytecode::push_4Opcode()
{
   trace ("push_4 opcode");

   _dataStack.pushInt(4);
}


void CRunBytecode::push_5Opcode()
{
   trace ("push_5 opcode");

   _dataStack.pushInt(5);
}



void CRunBytecode::pushsvOpcode()
{
   trace ("pushsv opcode");

   int address = _code.fetchInt();

   _dataStack.pushByte(CSymbol::VAR);

   std::string *value = new std::string(); // TODO: quando eh desalocado ??? 
   _dataStack.pushInt((int)value);

   *value = _dataStack.getString(address);
}


//void CRunBytecode::pushsvOpcode()
//{
//   // TODO: pushsv nao deveria empilhar uma copia do tipo e endereco da variavel ???
//   // e como empilhar uma constante global string, uma variavel local e uma variavel global ???
//   // Acho que sempre que empilhado, tudo deveria virar variavel. Ou seja, para empilhar uma
//   // constante, deveria ser criado uma variavel string com uma copia da constante.
//   // Se for empilhar uma variavel entao criar uma string e copiar o valor da outra variavel
//
//   trace ("pushsv opcode");
//
//   int address = _code.fetchInt();
//
////   _dataStack.pushByte(_dataStack.getByte(address));
//
//   // Quando uma variavel local eh empilhada o endereco empilhado nao pode ser
//   // o relativo, mas sim o absoluto (global). Caso contrario qdo a outra funcao
//   // tentar acessar o endereco ela vai usar o seu BS e nao o BS de quando o 
//   // endereco foi empilhado
//
//   if (IS_LOCAL_ADDRESS(address)) {
//      _dataStack.pushInt(_dataStack.getBS()+realAddress(address));
//   } else {
//      _dataStack.pushInt(address);
//   }
//}


void CRunBytecode::pushivOpcode()
{
   trace ("pushiv opcode");

   int address = _code.fetchInt();

   // Empilha o conteudo para que o formato dos dados globais e locais sejam o mesmo
   _dataStack.pushInt(_dataStack.getInt(address));
}

void CRunBytecode::pushrvOpcode()
{
   trace ("pushrv opcode");

   int address = _code.fetchInt();

   _dataStack.pushReal(_dataStack.getReal(address));
}

void CRunBytecode::pushdvOpcode()
{
   trace ("pushdv opcode");

   int varAddress  = _code.fetchInt();
   int sizeAddress = _code.fetchInt();

   _dataStack.pushBytes(_dataStack.getBytes(varAddress, _dataStack.getInt(sizeAddress)));
}

void CRunBytecode::pushmvOpcode()
{
   trace ("pushmv opcode");

   char *matrix    = (char*) _dataStack.getInt(_code.fetchInt());
   int elementSize = *((int*)(matrix+0));
   int elements    = *((int*)(matrix+sizeof(int)));

   int size = sizeof(int) + sizeof(int) + elements*elementSize;
   char *newMatrix = new char[size]; // TODO: memory leak
   memcpy(newMatrix, matrix, size);
   _dataStack.pushInt((int)newMatrix);
}

void CRunBytecode::pushstOpcode()
{
   trace ("pushst opcode");

   _dataStack.pushInt(CSymbol::STRING);
}

void CRunBytecode::pushitOpcode()
{
   trace ("pushit opcode");

   _dataStack.pushInt(CSymbol::INT);
}

void CRunBytecode::pushrtOpcode()
{
   trace ("pushrt opcode");

   _dataStack.pushInt(CSymbol::REAL);
}

void CRunBytecode::pushctOpcode()
{
   trace ("pushct opcode");

   _dataStack.pushInt(CSymbol::CHAR);
}

void CRunBytecode::pushbtOpcode()
{
   trace ("pushbt opcode");

   _dataStack.pushInt(CSymbol::BOOL);
}

void CRunBytecode::pushdtOpcode()
{
   trace ("pushdt opcode");

   _dataStack.pushInt(CSymbol::DATA);
}

void CRunBytecode::pushmtOpcode()
{
   invalidOpcode(__FUNCTION__);
}


void CRunBytecode::incsp_4Opcode()
{
   trace ("incsp_4 opcode");

   _dataStack.pushInt(0);
}

void CRunBytecode::incsp_8Opcode()
{
   trace ("incsp_8 opcode");

   _dataStack.pushInt(0);
   _dataStack.pushInt(0);
}

void CRunBytecode::decsp_4Opcode()
{
   trace ("decsp_4 opcode");

   _dataStack.popInt();
}

void CRunBytecode::decsp_8Opcode()
{
   trace ("decsp_8 opcode");

   _dataStack.popInt();
   _dataStack.popInt();
}

void CRunBytecode::retOpcode()
{
   trace ("ret opcode");

   _code.setIP(_executionStack.top());
   _executionStack.pop();

   _dataStack.setBS(_executionStack.top());
   _executionStack.pop();
}

void CRunBytecode::sallocOpcode()
{
   trace ("salloc opcode");

   int address = _code.fetchInt();

   _dataStack.setByte(address, CSymbol::VAR);
   char type = _dataStack.getByte(address);

   if (type != CSymbol::VAR) {
      error( "salloc apenas com variaveis !!!" );
   }

   std::string *value = new std::string();
   _dataStack.setInt(sumAddress(address,1), (int)value);
}

void CRunBytecode::sfreeOpcode()
{
   trace ("sfree opcode");

   int address = _code.fetchInt();

   char type = _dataStack.getByte(address);
   address=sumAddress(address,1);

   if (type != CSymbol::VAR) {
      error( "sfree apenas com variaveis !!!" );
   }

   std::string *value = (std::string*)_dataStack.getInt(address);
   delete value;
   _dataStack.setInt(address, 0);
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
   trace ("m1alloc opcode");

   // Header dos dados de uma matriz de uma linha:
   // int (tamanho de cada elemento armazenado)
   // int (numero de elementos)
   // bytes (area sequencial para armazenar os dados)

   int matrixAddress = _code.fetchInt();
   int elementSize   = _dataStack.getInt(_code.fetchInt());
   int elements      = _dataStack.getInt(_code.fetchInt());

   char *matrix = new char[sizeof(int)+sizeof(int)+elementSize*elements];
   *((int*)(matrix+0))           = elementSize;
   *((int*)(matrix+sizeof(int))) = elements;
   _dataStack.setInt(matrixAddress, (int)matrix);
}

void CRunBytecode::m2allocOpcode()
{
   invalidOpcode(__FUNCTION__);
}

void CRunBytecode::mfreeOpcode()
{
   trace ("mfree opcode");

   int matrixAddress = _code.fetchInt();

   char *matrix = (char*) _dataStack.getInt(matrixAddress);

   delete []matrix;
   _dataStack.setInt(matrixAddress, 0);
}

void CRunBytecode::m1setOpcode()
{
   trace ("m1set opcode");

   char *matrix     = (char*) _dataStack.getInt(_code.fetchInt());
   int offset       = _dataStack.getInt(_code.fetchInt());
   int valueAddress = _code.fetchInt();

   int elementSize = *((int*)(matrix+0));
//   int elements    = *((int*)(matrix+sizeof(int)));
   char *data      = matrix + sizeof(int) + sizeof(int);
   data += offset * elementSize;
   memcpy(data, _dataStack.getPointer(valueAddress), elementSize);
}

void CRunBytecode::m1getOpcode()
{
   trace ("m1get opcode");

   int resultAddress = _code.fetchInt();
   char *matrix      = (char*) _dataStack.getInt(_code.fetchInt());
   int offset        = _dataStack.getInt(_code.fetchInt());

   int elementSize = *((int*)(matrix+0));
//   int elements    = *((int*)(matrix+sizeof(int)));
   char *data      = matrix + sizeof(int) + sizeof(int);
   data += offset * elementSize;
   memcpy(_dataStack.getPointer(resultAddress), data, elementSize);
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

