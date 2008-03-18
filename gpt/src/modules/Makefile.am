INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src/parser -Iparser $(ANTLR_INC) $(all_includes)

SUBDIRS = parser c_translator interpreter x86

headers = GPTDisplay.hpp PortugolAST.hpp Symbol.hpp SymbolTable.hpp
sources = GPTDisplay.cpp PortugolAST.cpp Symbol.cpp SymbolTable.cpp

if INSTALL_DEVEL
lib_LTLIBRARIES = libgportugol.la
pkginclude_HEADERS = $(headers)
else
noinst_LTLIBRARIES = libgportugol.la
noinst_HEADERS = $(headers)
endif


libgportugol_la_SOURCES = $(sources)

libgportugol_la_LIBADD = $(top_builddir)/src/modules/parser/libparser.la \
                         $(top_builddir)/src/modules/interpreter/libinterpreter.la \
                         $(top_builddir)/src/modules/c_translator/libctranslator.la \
												 $(top_builddir)/src/modules/x86/libx86.la                         
