INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/parser \
	-I$(top_srcdir)/src/compiler -I$(top_srcdir)/src/compiler/parser $(all_includes)

METASOURCES = AUTO
noinst_LTLIBRARIES = libparser.la

lexer_g       = $(srcdir)/lexer.g
parser_g      = $(srcdir)/parser.g
semantic_g    = $(srcdir)/semantic.g

gensources = PortugolLexer.cpp   PortugolParser.hpp PortugolLexer.hpp \
					   PortugolParserTokenTypes.hpp  PortugolParser.cpp \
						 SemanticWalker.hpp SemanticWalker.cpp

libparser_la_SOURCES = $(gensources) BasePortugolParser.cpp SemanticEval.cpp

if INSTALL_DEVEL
pkginclude_HEADERS = PortugolParserTokenTypes.hpp BasePortugolParser.hpp \
										 PortugolLexer.hpp PortugolParser.hpp PortugolTokenTypes.hpp \
										 SemanticEval.hpp SemanticWalker.hpp SemanticWalkerTokenTypes.hpp
else
noinst_HEADERS = BasePortugolParser.hpp SemanticEval.hpp
endif


$(gensources): lexer parser semantic

lexer: $(lexer_g)
	$(ANTLR_BIN) $(lexer_g)
	cp PortugolTokenTypes.txt ../c_translator
	cp PortugolTokenTypes.txt ../interpreter


parser: $(parser_g)
	$(ANTLR_BIN) $(parser_g)

semantic: $(semantic_g)
	$(ANTLR_BIN) $(semantic_g)

distclean: clean
	rm -rf .deps

clean:
	rm -f *.cpp *.hpp *.a *.o *.txt


