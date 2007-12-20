#include "CBindSources.hpp"


CBindSources::CBindSources(const std::string &filename)
      : _filename(filename)
{
   _mapGptToCppType["string" ] = "const char *";
   _mapGptToCppType["real"   ] = "double";
   _mapGptToCppType["int"    ] = "int";
   _mapGptToCppType["char"   ] = "char";
   _mapGptToCppType["bool"   ] = "boolean";
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

   hppSource.writeln( "#include \"CDataStack.hpp\"" );
   hppSource.writeln();

   hppSource.writeln( "extern \"C\" {" );
   hppSource.incTab();

   cppSource.writeln( "#include \"" + _filename + ".hpp\"" );
   cppSource.writeln();
   cppSource.writeln( "#include \"CSymbol.hpp\"" );
   cppSource.writeln();

   makefileSource.writeln("COMMON_DIR=../../../common/src/");
   makefileSource.writeln("INCLUDE_DIR=-I$(COMMON_DIR)");// -I../../../gptvm/src/");
   makefileSource.writeln("CC=g++");
   makefileSource.writeln("CCFLAGS=-g -Wall");
}


void CBindSources::addSubroutineBind(
      const std::string &name,
      const std::pair<std::string, std::string> &returnType,
      std::vector<std::pair<std::string, std::pair<std::string, std::string> > > parameters,
      const std::string &functionBind,
      std::vector<std::string> arguments )
{
   hppSource.writeln( "void gsl_" + name + "( CDataStack &dataStack );" );

   cppSource.writeln();
   cppSource.writeln( "void gsl_" + name + "( CDataStack &dataStack )" );
   cppSource.writeln( "{" );
   cppSource.incTab();

   for(std::vector<std::pair<std::string, std::pair<std::string,std::string> > >::iterator param = parameters.begin(); param != parameters.end(); param++) {
      cppSource.writeln (sourceToPopParameter (param->first, param->second));
   }

   if (!returnType.first.empty()) {
      cppSource.writeln(getMapGptToCppType(returnType) + " result;");
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
   if (!returnType.first.empty()) {
      cppSource.writeln();
      cppSource.writeln(sourceToPushResult("result", returnType));
   }
   cppSource.decTab();
   cppSource.writeln( "}" );
}


std::string CBindSources::sourceToPopParameter(const std::string &name, const std::pair<std::string,std::string> &type)
{
   std::string result;

   std::map<std::string, std::string> mapPop;
   mapPop["string"] = "dataStack.popString().c_str()";
   mapPop["real"  ] = "dataStack.popReal()";
   mapPop["int"   ] = "dataStack.popInt()";
   mapPop["char"  ] = "dataStack.popInt()";
   mapPop["bool"  ] = "dataStack.popInt()";

   result = getMapGptToCppType(type) + " c" + name + "=";

   if (type.first == "pointer") {
      result += "(" + type.second + ")" + mapPop["int"];
   } else {
      result += mapPop[type.first];
   }
   result += ";";

   return result;
}


std::string CBindSources::sourceToPushResult(const std::string &name, const std::pair<std::string,std::string> &type)
{
   std::string result;

   std::map<std::string, std::string> mapPush;
   mapPush["string"] = "dataStack.pushString(" + name + ")";
   mapPush["real"  ] = "dataStack.pushReal(" + name + ")";
   mapPush["int"   ] = "dataStack.pushInt(" + name + ")";
   mapPush["char"  ] = "dataStack.pushInt(" + name + ")";
   mapPush["bool"  ] = "dataStack.pushBool(" + name + ")";
   mapPush["pointer"] = "dataStack.pushInt((int)" + name + ")";

   result = mapPush[type.first] + ";";

   return result;
}


void CBindSources::writeFooters()
{
   hppSource.decTab();
   hppSource.writeln("}");
   hppSource.writeln();
   hppSource.writeln( "#endif" );

   makefileSource.write("LIBS=");

   for(std::list<std::string>::iterator lib = _linkerLibList.begin(); lib != _linkerLibList.end(); lib++) {
      std::string slib = (*lib).substr(1, (*lib).length()-2);
      makefileSource.write("-l" + slib + " ");
   }
   makefileSource.writeln();
   makefileSource.writeln();

   makefileSource.writeln("objects = " + _filename + ".o $(COMMON_DIR)/CDataStack.o $(COMMON_DIR)/CBinString.o $(COMMON_DIR)/Tools.o");
   makefileSource.writeln();

   makefileSource.writeln("all: $(objects)");
   makefileSource.writeln("\tg++ -shared $(objects) $(LIBS) -o lib" + _filename + ".so");
   makefileSource.writeln();

   makefileSource.writeln("%.o: %.cpp %.hpp");
   makefileSource.writeln("\t$(CC) -shared $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@");
   makefileSource.writeln();

   makefileSource.writeln("%.o: %.cpp");
   makefileSource.writeln("\t$(CC) -shared $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@");
   makefileSource.writeln();

   makefileSource.writeln("clean:");
   makefileSource.writeln("\trm -f *.o lib" + _filename + ".so");
   makefileSource.writeln();
}


void CBindSources::addLinkerLib(const std::string &lib)
{
   _linkerLibList.push_back(lib);
}


void CBindSources::addHeader(const std::string &header)
{
   _headerList.push_back(header);
}


std::string CBindSources::getMapGptToCppType(const std::pair<std::string,std::string> &type)
{
   if (type.first == "pointer") {
      return type.second;
   } else {
      return _mapGptToCppType[type.first];
   }
}

