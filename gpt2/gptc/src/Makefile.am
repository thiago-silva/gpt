INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src

bin_PROGRAMS = gptc
gptc_LDADD = $(ANTLR_LIB)

EXTRA_DIST = lexer.g parser.g semantic.g


BUILT_SOURCES = PortugolLexer.hpp \
                PortugolLexer.cpp \
                PortugolParser.hpp \
                PortugolParser.cpp \
                SemanticWalker.hpp \
                SemanticWalker.cpp \
                TokenNames.hpp


gptc_SOURCES = BaseSemanticWalker.hpp \
               BaseSemanticWalker.cpp \
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
               teste.cpp \
               Types.hpp \
               Types.cpp \
               UnicodeCharBuffer.hpp \
               UnicodeCharScanner.hpp \
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
             TokenNames.hpp

PortugolLexer.cpp PortugolLexer.hpp: $(srcdir)/lexer.g Makefile
	$(ANTLR_BIN) $(srcdir)/lexer.g

PortugolParser.hpp PortugolParser.cpp: $(srcdir)/parser.g Makefile \
                                       PortugolLexer.cpp  PortugolLexer.hpp
	$(ANTLR_BIN) $(srcdir)/parser.g

SemanticWalker.hpp SemanticWalker.cpp: $(srcdir)/semantic.g Makefile \
                                       PortugolLexer.cpp  PortugolLexer.hpp
	$(ANTLR_BIN) $(srcdir)/semantic.g


TokenNames.hpp: Makefile
	@echo "#ifndef TOKENNAMES_H" > $@
	@echo "#define TOKENNAMES_H"  >> $@
	@echo "static char* g_tokenNames[] = " >> $@
	@echo '{"", "EOF", "", "",'   >> $@
	grep = PortugolTokenTypes.txt >> $@
	sed -i -e 's/\([A-Z][^=( ]*\)[=(].*/"\1",/' $@
	sed -i -e '$$s/,/};/' $@
	@echo "#endif" >> $@
