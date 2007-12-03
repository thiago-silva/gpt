#ifndef GPT_SYMBOL_HPP
#define GPT_SYMBOL_HPP

#include <vector>

#include "CBinString.hpp"


class CSymbol
{
public:
   enum EScope {
      GLOBAL = 'G',
      LOCAL  = 'L'
   };
   enum ECategory {
      PROC  = 'P',
      CONST = 'C',
      VAR   = 'V',
      PARAM = 'P'
   };
   enum EType {
      NO_TYPE = 'N',
      STRING  = 'S',
      INT     = 'I',
      CHAR    = 'C',
      LOGICAL = 'L',
      REAL    = 'R',
      MATRIX  = 'M',
      POINTER = 'P',
      BYTE    = 'B'
   };
   CSymbol ();
   CSymbol (const int &scope, const std::string &name, const char &type, int size,
            const char &category, const int &address);
   CSymbol (const std::string &name, const char &type, const int &address,
            const bool &hasVarArguments, const int &staticParameters,
            std::vector<CSymbol> parameters);
   virtual ~CSymbol();
   int getScope() const
   {
      return _scope;
   }
   std::string getName() const
   {
      return _name;
   }
   int getTypeSize() const;
   char getType() const
   {
      return _type;
   }
   char getCategory() const
   {
      return _category; 
   }
   int getAddress() const
   {
      return _address;
   }
   bool readFromBinary(CBinString &bin);
   bool getHasVarArguments() const
   {
      return _hasVarArguments;
   }
   int getStaticParameters() const
   {
      return _staticParameters;
   }
   std::vector<CSymbol> getParameters() const
   {
      return _parameters;
   }
private:
   int                  _scope;
   std::string          _name;
   char                 _type;
   int                  _size;
   char                 _category;
   int                  _address;
   bool                 _hasVarArguments;
   int                  _staticParameters;
   std::vector<CSymbol> _parameters;
};

#endif

