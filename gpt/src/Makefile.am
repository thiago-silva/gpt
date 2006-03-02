bin_PROGRAMS = gpt
gpt_SOURCES  = main.cpp

INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/c_translator -I$(top_srcdir)/src/modules/interpreter \
	-I$(top_srcdir)/src/modules/parser -I$(top_builddir)/src/modules/parser \
	-I$(top_builddir)/src/modules/c_translator -I$(top_builddir)/src/modules/interpreter \
	-I$(top_builddir)/src/modules/c_translator -I$(top_builddir)/src/modules/x86 \
	-I$(top_srcdir)/src/modules/x86 $(ANTLR_INC) $(all_includes)
	
SUBDIRS = modules

if SO_WINDOWS
gpt_LDADD = $(top_builddir)/src/modules/libgportugol.la $(ANTLR_LIB) $(PCRE_LIB) -lwsock32
else
gpt_LDADD = $(top_builddir)/src/modules/libgportugol.la $(ANTLR_LIB) $(PCRE_LIB) 
endif

AM_LDFLAGS = -static
