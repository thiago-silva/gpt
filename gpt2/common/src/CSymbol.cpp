#include <iostream>


#include "CSymbol.hpp"
#include "Tools.hpp"


CSymbol::CSymbol()
{
}


CSymbol::CSymbol (const int &scope, const std::string &name, const char &type, const char &category, const int &address)
   : _scope(scope)
   , _name(name)
   , _type(type)
   , _category(category)
   , _address(address)
{
}


CSymbol::CSymbol (const std::string &name, const char &type, 
                  const int &address, const bool &hasVarArguments, 
                  const int &staticParameters, 
                  std::vector<CSymbol> parameters)
   : _scope(GLOBAL)
   , _name(name)
   , _type(type)
   , _category(PROC)
   , _address(address)
   , _hasVarArguments(hasVarArguments)
   , _staticParameters(staticParameters)
   , _parameters(parameters)
{
}


CSymbol::~CSymbol()
{
}


int CSymbol::getTypeSize() const
{
   return ::getTypeSize(_type);
}


bool CSymbol::readFromBinary(CBinString &bin)
{
   _scope = GLOBAL;
   bin.readInt(_address);
   bin.readByte(_category);

   if (_category == CSymbol::VAR || _category == CSymbol::CONST) {
      bin.readByte(_type);
      bin.readString(_name);
//      std::cout << "var ou const, type=" << _type << " name=" << _name << std::endl;
   } else { // PROC
      bool _hasVariableArguments;// TODO
      char _staticParameters;// TODO
      bin.readByte(_type);
      bin.readString(_name);
      bin.readBool(_hasVariableArguments);
      bin.readByte(_staticParameters);
//      std::cout << "proc, type=" << _type << " name=" << _name << std::endl;
   }
   return true;
}

