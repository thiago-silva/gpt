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


//CSymbol* CData::add(const std::string &name, const int &category, const int &type, const int &address)
//{
//   return CSymbolList::add( name, category, type, address, 0 ); // TODO: nao eh zero...
//}


CSymbol* CData::addVariable (const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = new CSymbol (name, type, CSymbol::VAR, address);

   _symbols.push_back(symbol);

   //_data += symbol->getBinary();
   for (int i=0; i < symbol->getTypeSize(); i++) {
      // TODO: horrivel :-)
      _data.writeByte (' ');
   }

   return symbol;
}


CSymbol* CData::addConstant (const std::string &name, const int &type, const int &address)
{
   CSymbol *symbol = new CSymbol (name, type, CSymbol::CONST, address);

   _symbols.push_back(symbol);

   //_data += symbol->getBinary();
   if (type == CSymbol::STRING) {
      _data.writeString(name, false);
   } else if (type == CSymbol::INT) {
      _data.writeInt(atoi(name.c_str()));
   }

   return symbol;
}


bool CData::readFromBinary(CBinString &bin)
{
   bin.readString(_data);
   return true;
}


std::string CData::getString(const int &address)
{
   int pos = 0;

   pos = _data.find('\0', address);

   if (pos == -1) {
      abort();
   }

   return _data.substr(address, pos - address);
}


int CData::getInt(const int &address)
{
   int *ret = (int*)&(_data[address]);
   return *ret;
}


void CData::setInt(const int &address, const int &value)
{
   int *targetAddress = (int*)&(_data[address]);

   *targetAddress = value;
}

