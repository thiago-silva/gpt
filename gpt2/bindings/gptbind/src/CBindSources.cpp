#include "CBindSources.hpp"


#include <map>


CBindSources::CBindSources(const std::string &filename, const std::string &prefix)
      : _filename(filename)
      , _prefix(prefix)
{
}


void CBindSources::writeHeaders()
{
   hppSource.writeln( "#ifndef GPTBIND_" + _filename + "_HPP" );
   hppSource.writeln( "#define GPTBIND_" + _filename + "_HPP" );
   hppSource.writeln();
   hppSource.writeln( "extern \"C\" {" );
   hppSource.incTab();

   cppSource.writeln( "#include \"" + _filename + ".hpp\"" );
   cppSource.writeln();
   cppSource.writeln( "#include \"CDataStack.hpp\"" );
   cppSource.writeln( "#include \"CSymbol.hpp\"" );
   cppSource.writeln();
}


void CBindSources::addProcedureBind(
      const std::string &name, 
      std::vector<std::pair<std::string, std::string> > parameters,
      const std::string &functionBind,
      std::vector<std::string> arguments )
{
   hppSource.writeln( "void " + _prefix + name + "( CDataStack &dataStack );" );

   cppSource.writeln();
   cppSource.writeln( "void " + _prefix + name + "( CDataStack &dataStack )" );
   cppSource.writeln( "{" );
   cppSource.incTab();
   for(std::vector<std::pair<std::string, std::string> >::iterator param = parameters.begin(); param != parameters.end(); param++) {
      cppSource.writeln (sourceToGetParameter (param->first, param->second));
   }
   cppSource.write(functionBind + "(");
   for(std::vector<std::string>::iterator arg = arguments.begin(); arg != arguments.end(); arg++) {
      if (arg != arguments.begin()) {
         cppSource.write(", ");
      }
      if ((*arg)[0] != '"') {
         cppSource.write("c" + *arg);
      } else {
         cppSource.write(*arg);
      }
   }
   cppSource.writeln(");");
   cppSource.decTab();
   cppSource.writeln( "}" );
}


std::string CBindSources::sourceToGetParameter(const std::string &name, const std::string &type)
{
   std::string result;

   std::map<std::string, std::string> mapType;
   mapType["string"] = "const char *";

   std::map<std::string, std::string> mapPop;
   mapPop["string"] = "dataStack.popString().c_str()";

   result = mapType[type] + " c" + name + "=" + mapPop[type] + ";";

   return result;
}


void CBindSources::writeFooters()
{
   hppSource.decTab();
   hppSource.writeln("}");
   hppSource.writeln();
   hppSource.writeln( "#endif" );
}

