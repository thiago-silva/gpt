COMMON_IDIR = ../../../../common/src
INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src -I$(top_srcdir)/src/common \
           -I$(COMMON_IDIR)

EXTRA_DIST = lexer.g parser.g

noinst_LTLIBRARIES = libparser.la

BUILT_SOURCES = PortugolLexer.hpp \
                PortugolLexer.cpp \
                PortugolParser.hpp \
                PortugolParser.cpp \
                TokenNames.hpp \
                TokenLabels.hpp

libparser_la_SOURCES = $(BUILT_SOURCES)


CLEANFILES = PortugolLexer.cpp \
             PortugolLexer.hpp \
             PortugolTokenTypes.hpp \
             PortugolTokenTypes.txt \
             PortugolParser.cpp \
						 PortugolParser.hpp  \
             PortugolParserTokenTypes.hpp \
             PortugolParserTokenTypes.txt \
             TokenNames.hpp \
             TokenLabels.hpp

PortugolLexer.cpp PortugolLexer.hpp: $(srcdir)/lexer.g
	$(ANTLR_BIN) $(srcdir)/lexer.g
	cp PortugolTokenTypes.txt ../semantic
	cp PortugolTokenTypes.txt ../gptasm_target

PortugolParser.hpp PortugolParser.cpp: $(srcdir)/parser.g
	$(ANTLR_BIN) $(srcdir)/parser.g

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