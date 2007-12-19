SUBDIRS = parser common semantic symboltable gptasm_target

COMMON_IDIR = ../../../common/src

INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/parser \
	-I$(top_srcdir)/src/common -I$(top_srcdir)/src/semantic -I$(top_srcdir)/src/symboltable \
	-I$(top_srcdir)/src/gptasm_target -I./parser -I./semantic -I./gptasm_target \
  -I$(COMMON_IDIR)

bin_PROGRAMS = gptc

gptc_SOURCES = main.cpp

gptc_LDADD = $(top_builddir)/src/symboltable/libsymboltable.la \
	$(top_builddir)/src/common/libcommon.la $(top_builddir)/src/gptasm_target/libgptasm.la \
	$(top_builddir)/src/semantic/libsemantic.la $(top_builddir)/src/parser/libparser.la \
  $(COMMON_LIB) $(ANTLR_LIB)
