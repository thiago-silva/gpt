#include "CData.hpp"


CData::CData()
{
}


CData::~CData()
{
}


CSymbol* CData::add(CSymbol *symbol)
{
//   std::cout << "Adicionando [" << symbol->getName() << "] type [" << symbol->getType() << "] address [" << symbol->getAddress() << "] a Data" << std::endl;

   return CSymbolList::add( symbol );
}


CSymbol* CData::addVariable (const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = new CSymbol (name, type, CSymbol::VAR, address);

   _symbols.push_back(symbol);

   //_data += symbol->getBinary();
   for (int i=0; i < symbol->getTypeSize(); i++) {
      // TODO: horrivel :-)
      writeByte (' ');
   }

   return symbol;
}


CSymbol* CData::addConstant (const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = new CSymbol (name, type, CSymbol::CONST, address);

   _symbols.push_back(symbol);

   //_data += symbol->getBinary();
   if (type == CSymbol::STRING) {
      writeString(name, false);
   } else if (type == CSymbol::INT) {
      writeInt(atoi(name.c_str()));
   }

   return symbol;
}



