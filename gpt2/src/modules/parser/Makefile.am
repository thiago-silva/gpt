INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/parser \
	-I$(top_srcdir)/src/modules -I$(top_srcdir)/src/modules/parser \
	$(ANTLR_INC) $(all_includes)

noinst_LTLIBRARIES = libparser.la

BUILT_SOURCES = PortugolLexer.cpp  PortugolParser.cpp

nodist_libparser_la_SOURCES = $(BUILT_SOURCES)
libparser_la_SOURCES = BasePortugolParser.cpp MismatchedUnicodeCharException.cpp GASMFE_CSubroutine.cpp \
                       GASMFE_CArguments.cpp  GASMFE_CMakeTempVars.cpp CTemporarySymbol.cpp   \
                       GASMFE_CContext.cpp Tools.cpp CTextFile.cpp GASMFE_CGptAssemblyFile.cpp          \
                       GASMFE_CAsmProgram.cpp GASMFE_CMakeLabels.cpp


if INSTALL_DEVEL
nodist_pkginclude_HEADERS = PortugolParserTokenTypes.hpp PortugolLexer.hpp \
                            PortugolParser.hpp PortugolTokenTypes.hpp


pkginclude_HEADERS = BasePortugolParser.hpp MismatchedUnicodeCharException.hpp \
                     UnicodeCharBuffer.hpp UnicodeCharScanner.hpp
else
noinst_HEADERS = BasePortugolParser.hpp MismatchedUnicodeCharException.hpp \
                     UnicodeCharBuffer.hpp UnicodeCharScanner.hpp
endif

lexer_g       = lexer.g
parser_g      = parser.g
#semantic_g    = semantic.g

#EXTRA_DIST = $(lexer_g) $(parser_g) $(semantic_g) accent.sh
EXTRA_DIST = $(lexer_g) $(parser_g) accent.sh

CLEANFILES = PortugolLexer.cpp PortugolLexer.hpp PortugolParser.cpp PortugolParser.hpp \
             PortugolParserTokenTypes.hpp PortugolParserTokenTypes.txt \
             PortugolTokenTypes.hpp PortugolTokenTypes.txt

PortugolLexer.cpp: $(srcdir)/$(lexer_g)
	$(ANTLR_BIN) $(srcdir)/$(lexer_g)

PortugolParser.cpp: $(srcdir)/$(parser_g) PortugolLexer.cpp
	$(ANTLR_BIN) $(srcdir)/$(parser_g)
	./$(srcdir)/accent.sh

#SemanticWalker.cpp: $(srcdir)/$(semantic_g) PortugolLexer.cpp
#	$(ANTLR_BIN) $(srcdir)/$(semantic_g)
