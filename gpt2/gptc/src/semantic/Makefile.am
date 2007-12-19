INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/common \
           -I$(top_srcdir)/src/symboltable -I../parser -I../semantic

noinst_LTLIBRARIES = libsemantic.la

EXTRA_DIST = semantic.g

BUILT_SOURCES = SemanticWalker.hpp SemanticWalker.cpp

libsemantic_la_SOURCES = BaseSemanticWalker.hpp \
	  										 BaseSemanticWalker.cpp \
 											   $(BUILT_SOURCES)


CLEANFILES = SemanticWalker.hpp \
             SemanticWalker.cpp \
             SemanticWalkerTokenTypes.hpp \
             SemanticWalkerTokenTypes.txt \
             PortugolTokenTypes.txt

SemanticWalker.hpp SemanticWalker.cpp: $(srcdir)/semantic.g
	$(ANTLR_BIN) $(srcdir)/semantic.g
