INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src/parser -Iparser 	$(all_includes)
METASOURCES = AUTO
SUBDIRS = parser c_translator interpreter 

if INSTALL_DEVEL
lib_LTLIBRARIES = libgportugol.la
pkginclude_HEADERS = ErrorHandler.hpp PortugolAST.hpp Symbol.hpp SymbolTable.hpp
else
noinst_LTLIBRARIES = libgportugol.la
noinst_HEADERS = ErrorHandler.hpp PortugolAST.hpp Symbol.hpp SymbolTable.hpp
endif


libgportugol_la_SOURCES = ErrorHandler.cpp PortugolAST.cpp Symbol.cpp SymbolTable.cpp

libgportugol_la_LIBADD = $(top_builddir)/src/compiler/parser/libparser.la\
$(top_builddir)/src/compiler/interpreter/libinterpreter.la\
$(top_builddir)/src/compiler/c_translator/libc_translator.la
