#include "CGenBytecode.hpp"

#include <sstream>
#include <iostream>


#include "Common.hpp"
#include "Tools.hpp"


CGenBytecode::CGenBytecode()
   : _currentSP(0)
//   , _parametersSize(0)
{
   _opcodes[ "isum"        ] = OP_ISUM;
   _opcodes[ "ssum"        ] = OP_SSUM;
   _opcodes[ "rsum"        ] = OP_RSUM;
   _opcodes[ "isub"        ] = OP_ISUB;
   _opcodes[ "ssub"        ] = OP_SSUB;
   _opcodes[ "rsub"        ] = OP_RSUB;
   _opcodes[ "imul"        ] = OP_IMUL;
   _opcodes[ "rmul"        ] = OP_RMUL;
   _opcodes[ "idiv"        ] = OP_IDIV;
   _opcodes[ "rdiv"        ] = OP_RDIV;
   _opcodes[ "imod"        ] = OP_IMOD;
   _opcodes[ "rmod"        ] = OP_RMOD;
   _opcodes[ "ige"         ] = OP_IGE;
   _opcodes[ "sge"         ] = OP_SGE;
   _opcodes[ "rge"         ] = OP_RGE;
   _opcodes[ "ile"         ] = OP_ILE;
   _opcodes[ "sle"         ] = OP_SLE;
   _opcodes[ "rle"         ] = OP_RLE;
   _opcodes[ "ine"         ] = OP_INE;
   _opcodes[ "sne"         ] = OP_SNE;
   _opcodes[ "rne"         ] = OP_RNE;
   _opcodes[ "igt"         ] = OP_IGT;
   _opcodes[ "sgt"         ] = OP_SGT;
   _opcodes[ "rgt"         ] = OP_RGT;
   _opcodes[ "ilt"         ] = OP_ILT;
   _opcodes[ "slt"         ] = OP_SLT;
   _opcodes[ "rlt"         ] = OP_RLT;
   _opcodes[ "ieq"         ] = OP_IEQ;
   _opcodes[ "seq"         ] = OP_SEQ;
   _opcodes[ "req"         ] = OP_REQ;
   _opcodes[ "or"          ] = OP_OR;
   _opcodes[ "and"         ] = OP_AND;
   _opcodes[ "xor"         ] = OP_XOR;
   _opcodes[ "ineg"        ] = OP_INEG;
   _opcodes[ "rneg"        ] = OP_RNEG;
   _opcodes[ "not"         ] = OP_NOT;
   _opcodes[ "iinc"        ] = OP_IINC;
   _opcodes[ "idec"        ] = OP_IDEC;
   _opcodes[ "i2c"         ] = OP_I2C;
   _opcodes[ "r2c"         ] = OP_R2C;
   _opcodes[ "s2c"         ] = OP_S2C;
   _opcodes[ "b2c"         ] = OP_B2C;
   _opcodes[ "i2r"         ] = OP_I2R;
   _opcodes[ "c2r"         ] = OP_C2R;
   _opcodes[ "s2r"         ] = OP_S2R;
   _opcodes[ "b2r"         ] = OP_B2R;
   _opcodes[ "i2b"         ] = OP_I2B;
   _opcodes[ "c2b"         ] = OP_C2B;
   _opcodes[ "r2b"         ] = OP_R2B;
   _opcodes[ "s2b"         ] = OP_S2B;
   _opcodes[ "i2s"         ] = OP_I2S;
   _opcodes[ "c2s"         ] = OP_C2S;
   _opcodes[ "r2s"         ] = OP_R2S;
   _opcodes[ "b2s"         ] = OP_B2S;
   _opcodes[ "p2s"         ] = OP_P2S;
   _opcodes[ "c2i"         ] = OP_C2I;
   _opcodes[ "r2i"         ] = OP_R2I;
   _opcodes[ "s2i"         ] = OP_S2I;
   _opcodes[ "b2i"         ] = OP_B2I;
   _opcodes[ "iset"        ] = OP_ISET;
   _opcodes[ "sset"        ] = OP_SSET;
   _opcodes[ "rset"        ] = OP_RSET;
   _opcodes[ "geta"        ] = OP_GETA;
   _opcodes[ "igetv"       ] = OP_IGETV;
   _opcodes[ "sgetv"       ] = OP_SGETV;
   _opcodes[ "rgetv"       ] = OP_RGETV;
   _opcodes[ "isetv"       ] = OP_ISETV;
   _opcodes[ "ssetv"       ] = OP_SSETV;
   _opcodes[ "rsetv"       ] = OP_RSETV;
   _opcodes[ "jmp"         ] = OP_JMP;
   _opcodes[ "if"          ] = OP_IF;
   _opcodes[ "ifnot"       ] = OP_IFNOT;
//   _opcodes[ "push"        ] = OP_PUSH;
   _opcodes[ "popiv"         ] = OP_POPIV;
   _opcodes[ "poprv"         ] = OP_POPRV;
   _opcodes[ "popmv"         ] = OP_POPMV;
   _opcodes[ "incsp"       ] = OP_INCSP;
   _opcodes[ "decsp"       ] = OP_DECSP;
   _opcodes[ "push0"       ] = OP_PUSH0;
   _opcodes[ "push1"       ] = OP_PUSH1;
   _opcodes[ "push2"       ] = OP_PUSH2;
   _opcodes[ "push3"       ] = OP_PUSH3;
   _opcodes[ "push4"       ] = OP_PUSH4;
   _opcodes[ "push5"       ] = OP_PUSH5;
   _opcodes[ "pushiv"      ] = OP_PUSHIV;
   _opcodes[ "pushsv"      ] = OP_PUSHSV;
   _opcodes[ "pushrv"      ] = OP_PUSHRV;
   _opcodes[ "pushmv"      ] = OP_PUSHMV;
   _opcodes[ "pushit"      ] = OP_PUSHIT;
   _opcodes[ "pushst"      ] = OP_PUSHST;
   _opcodes[ "pushrt"      ] = OP_PUSHRT;
   _opcodes[ "pushct"      ] = OP_PUSHCT;
   _opcodes[ "pushbt"      ] = OP_PUSHBT;
   _opcodes[ "pushmt"      ] = OP_PUSHMT;
   _opcodes[ "pushsreg"    ] = OP_PUSHSREG;
   _opcodes[ "popsreg"     ] = OP_POPSREG;
   _opcodes[ "incsp4"      ] = OP_INCSP4;
   _opcodes[ "incsp8"      ] = OP_INCSP8;
   _opcodes[ "decsp4"      ] = OP_DECSP4;
   _opcodes[ "decsp8"      ] = OP_DECSP8;
   _opcodes[ "pcall"       ] = OP_PCALL;
   _opcodes[ "ret"         ] = OP_RET;
   _opcodes[ "libcall"     ] = OP_LIBCALL;
   _opcodes[ "salloc"      ] = OP_SALLOC;
   _opcodes[ "sfree"       ] = OP_SFREE;
   _opcodes[ "ssetc"       ] = OP_SSETC;
   _opcodes[ "sgetc"       ] = OP_SGETC;
   _opcodes[ "m1alloc"     ] = OP_M1ALLOC;
   _opcodes[ "m2alloc"     ] = OP_M2ALLOC;
   _opcodes[ "mfree"       ] = OP_MFREE;
   _opcodes[ "m1set"       ] = OP_M1SET;
   _opcodes[ "m1get"       ] = OP_M1GET;
   _opcodes[ "m2set"       ] = OP_M2SET;
   _opcodes[ "m2get"       ] = OP_M2GET;
   _opcodes[ "mcopy"       ] = OP_MCOPY;
   _opcodes[ "mgetsize1"   ] = OP_MGETSIZE1;
   _opcodes[ "mgetsize2"   ] = OP_MGETSIZE2;
   _opcodes[ "nop"         ] = OP_NOP;
   _opcodes[ "exit0"       ] = OP_EXIT0;
   _opcodes[ "exit1"       ] = OP_EXIT1;
   _opcodes[ "hlt"         ] = OP_HLT;
   _opcodes[ "exit"        ] = OP_EXIT;
}


CGenBytecode::~CGenBytecode()
{
}


void CGenBytecode::initProcedure(const std::string &procedureName, const bool &hasVarArguments, const int &staticParameters, std::vector<CSymbol> parameters)
{
// TODO: e qdo procedure vai retornar valor no assembly ???
// Talvez temos q fazer como em C e gerar procedure_name + retorno + parametros (int_procedure_int_real)
   _symbolTable.addProcedure (procedureName, CSymbol::NO_TYPE, _code.size(), hasVarArguments, staticParameters, parameters);
   _currentProcedure = procedureName;
   _currentSP = 0;
    registryLabel(procedureName);
   _parameters.clear();
//   _parametersSize = 0;
}


void CGenBytecode::finishProcedure()
{
   _currentProcedure.clear();
   _currentSP = 0;
//   _parametersSize = 0;
   _symbolTable.clearLocalSymbols();
   // TODO: delete na procedure ???
}


void CGenBytecode::makeVarDefinition(const std::string &lexeme, const int &type)
{
   if (_currentProcedure.empty()) {
      // variavel global
      _symbolTable.addVariable (CSymbol::GLOBAL, lexeme, type, _data.getDataSize());
      std::cout << "var global " << lexeme << " address " << _data.getDataSize() << std::endl;
      _data.addVariable (CSymbol::GLOBAL, lexeme, type, _data.getDataSize());
   } else {
      // variavel local
      CSymbol *symbol = _symbolTable.addVariable (CSymbol::LOCAL, lexeme, type, _currentSP);
      std::cout << "var local " << lexeme << " address " << _currentSP << std::endl;
      _currentSP += symbol->getTypeSize();
      // gera incsp XXX
      _code.writeByte(OP_INCSP);
      addAddress(itoa(symbol->getTypeSize()), CSymbol::CONST, CSymbol::INT);
   }
}


void CGenBytecode::makeParDefinition(const std::string &lexeme, const int &type)
{
   _parameters.push_back(std::pair<std::string,int>(lexeme, type));
//   _parametersSize += getTypeSize(type);
//   CSymbol *symbol = _symbolTable.addParameter(lexeme, type, _currentSP);
//   std::cout << "par " << lexeme << " address " << _currentSP << std::endl;
//   _currentSP += symbol->getTypeSize();
}


void CGenBytecode::finishParDefinition()
{
   _currentSP = 0;
   for (std::list<std::pair<std::string,int> >::reverse_iterator par = _parameters.rbegin();
         par != _parameters.rend(); par++) {
      _currentSP -= getTypeSize(par->second);
      CSymbol *symbol = _symbolTable.addParameter(par->first, par->second, abs(_currentSP));
//      std::cout << "par " << par->first << " address " << _currentSP << std::endl;
      std::cout << "param=" << symbol->getName() << " address=" << symbol->getAddress() << std::endl;
      std::cout << "\t" << realAddressString(symbol->getAddress()) << std::endl;
   }

   _currentSP = 0;

//   if (_parametersSize != 0) {
//      std::cout << "_parametersSize != 0 !!!" << std::endl;
//      abort();
//   }
}


//void CGenBytecode::makeParDefinition(const std::string &lexeme, const int &type)
//{
//   CSymbol *symbol = _symbolTable.addParameter(lexeme, type, _currentSP);
//   std::cout << "par " << lexeme << " address " << _currentSP << std::endl;
//   _currentSP += symbol->getTypeSize();
//}


void CGenBytecode::registryLabel(const std::string &labelName)
{
   // TODO
   if (_solvedLabels.find(labelName) != _solvedLabels.end()) {
      std::cout << "Label ja definido: " << labelName << std::endl;
      abort();
   }
   _solvedLabels[labelName] = _code.size();
   std::cout << "Registry label: " << labelName << " address: " << _code.size() << std::endl;
}


void CGenBytecode::addOpcode(const std::string &mn)
{
//   std::cout << "Mn=" << mn << " opcode: " << (int)_opcodes[mn] << std::endl;
   _code.writeByte(_opcodes[mn]);
}


void CGenBytecode::addAddress(const std::string &id, const int &category, const int &type)
{
   int address = _symbolTable.getAddress(id);
   if (address == -1) {
      address = _data.addConstant (id, type, _data.getDataSize())->getAddress();
   }
   _code.writeInt(address);
}


CBinString CGenBytecode::getBinary()
{
   CBinString ret;

   ret += _header.getBinary();

   ret.writeInt(_symbolTable.getSymbolsCount());
   ret += _symbolTable;

   ret.writeInt(_data.getDataSize());
   ret += _data;

   ret.writeInt(_code.size());
   ret += _code;

   return ret;
}

void CGenBytecode::unsolvedLabel(const std::string &label)
{
   _unsolvedLabels.push_back(std::pair<std::string, int>(label, _code.size()));
   std::cout << "Unsolved label: " << label << " address: " << _code.size() << std::endl;
   _code.writeInt(0);
}


void CGenBytecode::translateLabelsToAddress()
{
   // Varre os labels referenciados
   for (std::list<std::pair<std::string,int> >::const_iterator unsolvedLabel = _unsolvedLabels.begin();
         unsolvedLabel != _unsolvedLabels.end(); unsolvedLabel++) {
      std::map<std::string,int>::const_iterator solvedLabel;
      solvedLabel = _solvedLabels.find(unsolvedLabel->first);
      if (solvedLabel == _solvedLabels.end()) {
         std::cout << "Label nao encontrado: " << unsolvedLabel->second << std::endl;
         abort();
      }
      _code.setInt(unsolvedLabel->second, solvedLabel->second);
//      std::cout << "code[" << unsolvedLabel->second << "]=" << solvedLabel->second << std::endl;
   }
}

