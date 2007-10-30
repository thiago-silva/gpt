INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src/parser -Iparser $(ANTLR_INC) $(all_includes)

SUBDIRS = parser

if INSTALL_DEVEL
lib_LTLIBRARIES = libgportugol.la
pkginclude_HEADERS = GPTDisplay.hpp Symbol.hpp SymbolTable.hpp
else
noinst_LTLIBRARIES = libgportugol.la
noinst_HEADERS = GPTDisplay.hpp Symbol.hpp SymbolTable.hpp
endif


libgportugol_la_SOURCES = GPTDisplay.cpp Symbol.cpp SymbolTable.cpp

libgportugol_la_LIBADD = $(top_builddir)/src/modules/parser/libparser.la

#                         $(top_builddir)/src/modules/interpreter/libinterpreter.la \
#                         $(top_builddir)/src/modules/c_translator/libctranslator.la \
#			 $(top_builddir)/src/modules/x86/libx86.la
                         
