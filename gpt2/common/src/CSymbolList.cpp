#include "CSymbolList.hpp"


CSymbolList::CSymbolList()
{
}


CSymbolList::~CSymbolList()
{
}


CSymbol* CSymbolList::add(CSymbol* symbol)
{
   _symbols.push_back(symbol);

//   Este metodo esta sendo utilizado durante a leitura do binario. Mas durante a leitura nao eh
//   mais necessario popular "_data". Com isso symbol->getBinary pode ser descartado junto com
//   varios outros metodos/propriedades
//   (*this) += symbol->getBinary();
   return symbol;
}


CSymbol* CSymbolList::getSymbol(const std::string &name)
{
   std::vector<CSymbol*>::iterator it;

   it = find(name);

   return *it;
}


int CSymbolList::getReference( const std::string &name )
{
   std::vector<CSymbol*>::iterator it;

   it = find(name);

   if (it == _symbols.end()) {
      return -1;
   }

   return (it - _symbols.begin());
}


int CSymbolList::getAddress( const std::string &name )
{
   std::vector<CSymbol*>::iterator it;

   it = find(name);

   if (it == _symbols.end()) {
      return -1;
   }

   return (*it)->getAddress();
}


std::vector<CSymbol*>::iterator CSymbolList::find(const std::string &name)
{
   for(std::vector<CSymbol*>::iterator it = _symbols.begin(); it != _symbols.end(); it++) {
      if ((*it)->getName() == name) {
         return it;
      }
   }

   return _symbols.end();
}


bool CSymbolList::readFromBinary(CBinString &bin)
{
   int size = 0;
   bin.readInt(size);
   for (int count=0; count < size; count++) {
      CSymbol *symbol = new CSymbol();
      symbol->readFromBinary(bin);
      add(symbol);
   }
   return true;
}
