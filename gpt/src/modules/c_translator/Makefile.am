INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/c_translator $(ANTLR_INC) $(all_includes)

noinst_LTLIBRARIES = libctranslator.la

BUILT_SOURCES = Portugol2CWalker.cpp

nodist_libctranslator_la_SOURCES = $(BUILT_SOURCES)

pt2c_g = pt2c.g
EXTRA_DIST = $(pt2c_g)

CLEANFILES = Portugol2CWalker.cpp Portugol2CWalker.hpp Portugol2CWalkerTokenTypes.hpp \
             Portugol2CWalkerTokenTypes.txt PortugolTokenTypes.txt

Portugol2CWalker.cpp: $(srcdir)/$(pt2c_g)
	$(ANTLR_BIN) $(srcdir)/$(pt2c_g)
