bin_PROGRAMS = gpt
gpt_SOURCES  = gpt.cpp 

INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/compiler \
	-I$(top_srcdir)/src/compiler/c_translator -I$(top_srcdir)/src/compiler/interpreter \
	-I$(top_srcdir)/src/compiler/parser $(all_includes) -I$(top_builddir)/src/compiler/parser \
	-I$(top_builddir)/src/compiler/c_translator -I$(top_builddir)/src/compiler/interpreter

SUBDIRS = compiler
gpt_LDADD = $(top_builddir)/src/compiler/libgportugol.la $(ANTLR_LIB) -lpcrecpp
AM_LDFLAGS = -static
