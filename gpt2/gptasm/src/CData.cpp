#include "CData.hpp"
#include "Tools.hpp"


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


CSymbol* CData::addVariable (const int &scope, const std::string &name, const int &type, int size, const int &address)
{
   if (size == 0) {
      size = getTypeSize(type);
   }

   CSymbol *symbol = new CSymbol (scope, name, type, size, CSymbol::VAR, address);

   _symbols.push_back(symbol);

   if (type == CSymbol::STRING) {
      // Para strings constantes e variaveis eh necessario indicar a categoria
      writeByte(CSymbol::VAR);
   }

   //_data += symbol->getBinary();
   for (int i=0; i < symbol->getTypeSize(); i++) {
      // TODO: horrivel :-)
      writeByte ('\0');
   }

   return symbol;
}


CSymbol* CData::addConstant (const std::string &name, const int &type, int size, const int &address)
{
   if (size == 0) {
      size = getTypeSize(type);
   }

   CSymbol *symbol = new CSymbol (CSymbol::GLOBAL, name, type, size, CSymbol::CONST, address);

   _symbols.push_back(symbol);

   //_data += symbol->getBinary();
   if (type == CSymbol::STRING) {
      // Para strings constantes e variaveis eh necessario indicar a categoria
      std::cout << "String constante [" << name << "] indicado o tipo..." << std::endl;
      writeByte(CSymbol::CONST);
      writeString(name, false);
   } else if (type == CSymbol::INT) {
      writeInt(atoi(name.c_str()));
   } else if (type == CSymbol::REAL) {
      writeReal(atof(name.c_str()));
   } else if (type == CSymbol::CHAR) {
      writeInt(toUTF8Char(name));
   }

   return symbol;
}

