INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src \
	-I$(top_srcdir)/src/translations $(all_includes)
METASOURCES = AUTO
noinst_LIBRARIES = libparser.a

antlrlib = /usr/local/java/lib/antlr-2.7.5.jar

lexer_g      = $(top_srcdir)/gramaticas/lexer.g
parser_g     = $(top_srcdir)/gramaticas/parser.g
semantic_g   = $(top_srcdir)/gramaticas/semantic.g
pt2c_g       = $(top_srcdir)/gramaticas/pt2c.g

gensources = PortugolLexer.cpp   PortugolParser.hpp PortugolLexer.hpp \
					   PortugolParserTokenTypes.hpp  PortugolParser.cpp \
						 SemanticCheck.hpp SemanticCheck.cpp \
						 Portugol2C.cpp Portugol2C.hpp 

libparser_a_SOURCES = $(gensources) BasePortugolParser.cpp SymbolTable.cpp \
		PortugolAST.cpp SemanticEval.cpp Symbol.cpp

noinst_HEADERS = BasePortugolParser.hpp SymbolTable.hpp PortugolAST.hpp \
		SemanticEval.hpp Symbol.hpp

$(gensources): lexer parser semantic pt2c

lexer: $(lexer_g)
	antlr $(lexer_g)

parser: $(parser_g)
	antlr $(parser_g)

semantic: $(semantic_g)
	antlr $(semantic_g)

pt2c: $(pt2c_g)
	antlr $(pt2c_g)

distclean: clean
	rm -rf .deps

clean:
	rm -f *.cpp *.hpp *.a *.o *.txt

