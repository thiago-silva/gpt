#ifndef GPT_CBIND_SOURCES_HPP
#define GPT_CBIND_SOURCES_HPP

#include "CTextFile.hpp"

#include <string>
#include <vector>
#include <map>
#include <list>

class CBindSources
{
public:
   CBindSources(const std::string &filename);
   void writeHeaders();
   void addSubroutineBind(
         const std::string &name, 
         const std::pair<std::string,std::string> &returnType,
         std::vector<std::pair<std::string, std::pair<std::string, std::string> > > parameters,
         const std::string &functionBind,
         std::vector<std::string> arguments
   );
   void writeFooters();
   void addLinkerLib(const std::string &lib);
   void addHeader(const std::string &header);
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
private:
   std::string _filename;
   CTextFile hppSource;
   CTextFile cppSource;
   CTextFile makefileSource;
   std::map<std::string, std::string> _mapGptToCppType;
   std::list<std::string> _linkerLibList;
   std::list<std::string> _headerList;

   std::string sourceToPopParameter(const std::string &name, const std::pair<std::string,std::string> &type);
   std::string sourceToPushResult(const std::string &name, const std::pair<std::string,std::string> &type);
   std::string getMapGptToCppType(const std::pair<std::string,std::string> &type);
};

#endif

