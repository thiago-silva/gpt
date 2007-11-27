#include "CSymbolTable.hpp"


CSymbolTable::CSymbolTable()
{
}


CSymbolTable::~CSymbolTable()
{
}


CSymbol* CSymbolTable::addProcedure (const std::string &name, const int &type, const int &address, const bool &hasVarArguments, const int &staticParameters, std::vector<CSymbol> parameters)
{
   CSymbol *symbol = new CSymbol( name, type, address, hasVarArguments, staticParameters, parameters);

   _symbols.push_back(symbol);

   // Endereço 0123 (code), procedure, sem retorno, nome "p1",
   // sem argumentos variáveis, 2 parâmetros fixos, p1 (int) e p2 (string)
   // 0024: 0123 P N 2 "p1" N 2 I 4 2 "p1" S 30 2 "p2"
   writeInt( address );
   writeByte( CSymbol::PROC ); // procedure
   writeByte( type ); // tipo do retorno // TODO: em asm proc tem retorno ???
   writeString( name ); // nome da procedure
   writeBool( hasVarArguments );
   writeByte( staticParameters );
   // TODO: falta argumentos

   return symbol;
}


CSymbol* CSymbolTable::addParameter (const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = new CSymbol(CSymbol::LOCAL, name, type, CSymbol::PARAM, address | 0x80000000);

   _symbols.push_back(symbol);

   //_data += symbol->getBinary();

   return symbol;
}


CSymbol* CSymbolTable::addConstant (const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = new CSymbol(CSymbol::GLOBAL, name, type, CSymbol::CONST, address);

   _symbols.push_back(symbol);

   // Endereço 0000 (data), constante, string, 8 bytes ???, nome "c1"
   // 0000: 0000 C S 8??? 2 "c1"
   writeInt( address );
   writeByte( CSymbol::CONST ); // categoria: constante
   writeByte( type );           // tipo
   writeString( name );         // nome da constante

   return symbol;
}


CSymbol* CSymbolTable::addVariable (const int &scope, const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = NULL;

   if (scope == CSymbol::GLOBAL) {
      symbol = new CSymbol(scope, name, type, CSymbol::VAR, address);
   } else {
      symbol = new CSymbol(scope, name, type, CSymbol::VAR, address | 0x80000000 );
   }

   _symbols.push_back(symbol);

   if (scope == CSymbol::GLOBAL) {
      // Endereço 0000 (data), variavel, string, 8 bytes ???, nome "v1"
      // 0000: 0000 V S 8??? 2 "v1"
      writeInt( address );
      writeByte( CSymbol::VAR ); // categoria: variavel
      writeByte( type );         // tipo
      writeString( name );       // nome da variavel
   }

   return symbol;
}


CSymbol* CSymbolTable::add(CSymbol *symbol)
{
//   std::cout << "Adicionando simbolo [" << symbol->getName() << "] type [" << symbol->getType() << "] address [" << symbol->getAddress() << "] a SymbolTable" << std::endl;

   return CSymbolList::add( symbol );
}


bool CSymbolTable::readFromBinary(CBinString &bin)
{
   int size;
   bin.readInt(size);
   for (int count=0; count < size; count++) {
      CSymbol *symbol = new CSymbol();
      symbol->readFromBinary(bin);
      add(symbol);
   }
   return true;
}


void CSymbolTable::clearLocalSymbols()
{
   for(size_t pos = 0; pos < _symbols.size(); pos++) {
      if (_symbols[pos]->getScope() == CSymbol::LOCAL) {
         _symbols.erase(_symbols.begin() + pos);
         pos--;
      }
   }
}

