#ifndef GPT_CBIND_SOURCES_HPP
#define GPT_CBIND_SOURCES_HPP

#include "CTextFile.hpp"

#include <string>
#include <vector>

class CBindSources
{
public:
   CBindSources(const std::string &filename, const std::string &prefix);
   void writeHeaders();
   void addProcedureBind(
         const std::string &name, 
         std::vector<std::pair<std::string, std::string> > parameters,
         const std::string &functionBind,
         std::vector<std::string> arguments
   );
   void writeFooters();
   std::string getHppSource()
   {
      return hppSource.getText();
   }
   std::string getCppSource()
   {
      return cppSource.getText();
   }
   std::string getMakefileSource()
   {
      return makefileSource.getText();
   }
   std::string sourceToGetParameter(const std::string &name, const std::string &type);
private:
   std::string _filename;
   std::string _prefix;
   CTextFile hppSource;
   CTextFile cppSource;
   CTextFile makefileSource;
};

#endif

