#include "CBindSources.hpp"


CBindSources::CBindSources(const std::string &filename)
      : _filename(filename)
{
   _mapGptToCppType["string"] = "const char *";
   _mapGptToCppType["real"]   = "double";
}


void CBindSources::writeHeaders()
{
   hppSource.writeln( "#ifndef GPTBIND_" + _filename + "_HPP" );
   hppSource.writeln( "#define GPTBIND_" + _filename + "_HPP" );
   hppSource.writeln();

   for(std::list<std::string>::iterator header = _headerList.begin(); header != _headerList.end(); header++) {
      std::string sheader = (*header).substr(1, (*header).length()-2);
      hppSource.writeln( "#include <" + sheader + ">" );
   }
   hppSource.writeln();

   hppSource.writeln( "extern \"C\" {" );
   hppSource.incTab();

   cppSource.writeln( "#include \"" + _filename + ".hpp\"" );
   cppSource.writeln();
   cppSource.writeln( "#include \"CDataStack.hpp\"" );
   cppSource.writeln( "#include \"CSymbol.hpp\"" );
   cppSource.writeln();
}


void CBindSources::addSubroutineBind(
      const std::string &name,
      const std::string &returnType,
      std::vector<std::pair<std::string, std::string> > parameters,
      const std::string &functionBind,
      std::vector<std::string> arguments )
{
   hppSource.writeln( "void gsl_" + name + "( CDataStack &dataStack );" );

   cppSource.writeln();
   cppSource.writeln( "void gsl_" + name + "( CDataStack &dataStack )" );
   cppSource.writeln( "{" );
   cppSource.incTab();

   for(std::vector<std::pair<std::string, std::string> >::iterator param = parameters.begin(); param != parameters.end(); param++) {
      cppSource.writeln (sourceToPopParameter (param->first, param->second));
   }

   if (!returnType.empty()) {
      cppSource.writeln(_mapGptToCppType[returnType] + " result;");
      cppSource.writeln();
      cppSource.write( "result=" );
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
   if (!returnType.empty()) {
      cppSource.writeln();
      cppSource.writeln(sourceToPushResult("result", returnType));
   }
   cppSource.decTab();
   cppSource.writeln( "}" );
}


std::string CBindSources::sourceToPopParameter(const std::string &name, const std::string &type)
{
   std::string result;

   std::map<std::string, std::string> mapPop;
   mapPop["string"] = "dataStack.popString().c_str()";
   mapPop["real"]   = "dataStack.popReal()";

   result = _mapGptToCppType[type] + " c" + name + "=" + mapPop[type] + ";";

   return result;
}


std::string CBindSources::sourceToPushResult(const std::string &name, const std::string &type)
{
   std::string result;

   std::map<std::string, std::string> mapPush;
   mapPush["real"  ] = "dataStack.pushReal(" + name + ")";
   mapPush["string"] = "dataStack.pushString(" + name + ")";

   result = mapPush[type] + ";";

   return result;
}


void CBindSources::writeFooters()
{
   hppSource.decTab();
   hppSource.writeln("}");
   hppSource.writeln();
   hppSource.writeln( "#endif" );
}


void CBindSources::addLinkerLib(const std::string &lib)
{
   _linkerLibList.push_back(lib);
}


void CBindSources::addHeader(const std::string &header)
{
   _headerList.push_back(header);
}

