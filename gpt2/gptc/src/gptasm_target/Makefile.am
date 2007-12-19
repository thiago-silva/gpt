INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I../parser \
           -I$(top_srcdir)/src/symboltable -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libgptasm.la

EXTRA_DIST = gptasm.g

BUILT_SOURCES = GptAsmWalker.hpp GptAsmWalker.cpp


libgptasm_la_SOURCES = Arguments.cpp Arguments.hpp AsmProgram.cpp AsmProgram.hpp\
	BaseGptAsmWalker.cpp BaseGptAsmWalker.hpp Context.cpp Context.hpp\
	GptAsmExpression.cpp GptAsmExpression.hpp Options.hpp Subroutine.cpp\
	Subroutine.hpp TextFile.cpp TextFile.hpp Tools.cpp Tools.hpp $(BUILT_SOURCES)

CLEANFILES = GptAsmWalkerTokenTypes.hpp \
             GptAsmWalker.cpp \
						 GptAsmWalker.hpp  \
             GptAsmWalkerTokenTypes.txt

GptAsmWalker.hpp GptAsmWalker.cpp: $(srcdir)/gptasm.g Makefile
	$(ANTLR_BIN) $(srcdir)/gptasm.g
