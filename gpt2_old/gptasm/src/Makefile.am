COMMON_IDIR = ../../../common/src

INCLUDES = -I$(top_srcdir)/.  -I$(COMMON_IDIR)

bin_PROGRAMS = gptasm

gptasm_LDADD = $(COMMON_LIB) $(ANTLR_LIB)


BUILT_SOURCES = GptAssemblyLexer.hpp \
                GptAssemblyLexer.cpp \
                GptAssemblyParser.hpp \
                GptAssemblyParser.cpp 

gptasm_SOURCES = CData.cpp  CData.hpp  CGenBytecode.cpp  CGenBytecode.hpp  CGptAsm.cpp  \
                 CGptAsm.hpp  main.cpp $(BUILT_SOURCES)

EXTRA_DIST = lexer.g parser.g

CLEANFILES = $(BUILT_SOURCES) GptAssemblyLexerTokenTypes.* GptAssemblyParserTokenTypes.*




GptAssemblyLexer.cpp GptAssemblyLexer.hpp: $(srcdir)/lexer.g
	$(ANTLR_BIN) $(srcdir)/lexer.g

GptAssemblyParser.hpp GptAssemblyParser.cpp: $(srcdir)/parser.g
	$(ANTLR_BIN) $(srcdir)/parser.g
