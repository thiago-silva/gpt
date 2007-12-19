INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src

noinst_LTLIBRARIES = libgptc.la

bin_PROGRAMS = gptc

gptc_SOURCES = main.cpp

gptc_LDADD = libgptc.la $(ANTLR_LIB)


EXTRA_DIST = lexer.g parser.g semantic.g gptasm.g


BUILT_SOURCES = PortugolLexer.hpp \
                PortugolLexer.cpp \
                PortugolParser.hpp \
                PortugolParser.cpp \
                SemanticWalker.hpp \
                SemanticWalker.cpp \
								GptAsmWalker.hpp \
								GptAsmWalker.cpp \
                TokenNames.hpp \
                TokenLabels.hpp


libgptc_la_SOURCES =  BaseSemanticWalker.hpp \
											BaseSemanticWalker.cpp \
											BaseGptAsmWalker.hpp \
											BaseGptAsmWalker.cpp \
											CompilerError.hpp \
											CompilerError.cpp \
											MismatchedUnicodeCharException.hpp \
											MismatchedUnicodeCharException.cpp \
											PortugolAST.hpp \
											PortugolAST.cpp \
											Symbol.hpp \
											Symbol.cpp \
											SymbolTable.hpp \
											SymbolTable.cpp \
											SymbolTableExceptions.hpp \
											SymbolTableExceptions.cpp \
											Types.hpp \
											Types.cpp \
											UnicodeCharBuffer.hpp \
											UnicodeCharScanner.hpp \
											GptAsmExpression.hpp GptAsmExpression.cpp \
											Arguments.hpp \
											Arguments.cpp \
											AsmProgram.hpp AsmProgram.cpp \
										  Context.hpp Context.cpp \
										  Options.hpp \
											Subroutine.hpp Subroutine.cpp \
											TextFile.hpp TextFile.cpp \
										  Tools.hpp Tools.cpp \
											$(BUILT_SOURCES)


CLEANFILES = PortugolLexer.cpp \
             PortugolLexer.hpp \
             PortugolTokenTypes.hpp \
             PortugolTokenTypes.txt \
             PortugolParser.cpp \
						 PortugolParser.hpp  \
             PortugolParserTokenTypes.hpp \
             PortugolParserTokenTypes.txt \
             SemanticWalker.hpp \
             SemanticWalker.cpp \
             SemanticWalkerTokenTypes.hpp \
             SemanticWalkerTokenTypes.txt \
             GptAsmWalkerTokenTypes.hpp \
             GptAsmWalker.cpp \
						 GptAsmWalker.hpp  \
             GptAsmWalkerTokenTypes.txt \
             TokenNames.hpp \
             TokenLabels.hpp

PortugolLexer.cpp PortugolLexer.hpp: $(srcdir)/lexer.g Makefile
	$(ANTLR_BIN) $(srcdir)/lexer.g

PortugolParser.hpp PortugolParser.cpp: $(srcdir)/parser.g Makefile
	$(ANTLR_BIN) $(srcdir)/parser.g

SemanticWalker.hpp SemanticWalker.cpp: $(srcdir)/semantic.g Makefile
	$(ANTLR_BIN) $(srcdir)/semantic.g

GptAsmWalker.hpp GptAsmWalker.cpp: $(srcdir)/gptasm.g Makefile
	$(ANTLR_BIN) $(srcdir)/gptasm.g

TokenNames.hpp: Makefile
	@echo "#ifndef TOKENNAMES_H" > $@
	@echo "#define TOKENNAMES_H"  >> $@
	@echo "static char* g_tokenNames[] = " >> $@
	@echo '{"", "EOF", "", "",'   >> $@
	grep = PortugolTokenTypes.txt >> $@
	sed -i -e 's/\([A-Z][^=( ]*\)[=(].*/"\1",/' $@
	sed -i -e '$$s/,/};/' $@
	@echo -n "static int g_tokenNamesSize = " >> $@
	@echo `grep -o , TokenNames.hpp | wc -l` + 1 | bc >> $@
	@echo ";" >> $@
	@echo "#endif" >> $@

TokenLabels.hpp: Makefile
	@echo "#ifndef TOKENLABELS_H" > $@
	@echo "#define TOKENLABELS_H"  >> $@
	@echo "static char* g_tokenLabels[] = " >> $@
	@echo '{"", "EOF", "", "",'   >> $@
	grep '"' PortugolTokenTypes.txt >> $@
	sed -i -e 's/^T.*\("[^"]*"\).*/\1,/' $@
	sed -i -e '$$s/,/};/' $@
	@echo "static int g_tokenLabelsSize = " >> $@
	@echo  `grep -o , TokenLabels.hpp | wc -l` + 1 | bc >> $@
	@echo ";" >> $@
	@echo "#endif" >> $@


