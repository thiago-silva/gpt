bin_PROGRAMS = gpt
gpt_SOURCES = main.cpp GPT.cpp

INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/parser -I$(top_builddir)/src/modules/parser
	
SUBDIRS = modules

if SO_WINDOWS
gpt_LDADD = $(top_builddir)/src/modules/libgportugol.la $(ANTLR_LIB) $(PCRE_LIB) -lwsock32
else
gpt_LDADD = $(top_builddir)/src/modules/libgportugol.la $(ANTLR_LIB) $(PCRE_LIB)
endif

AM_LDFLAGS = -static
noinst_HEADERS = GPT.hpp
