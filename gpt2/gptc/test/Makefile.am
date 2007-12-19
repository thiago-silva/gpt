INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/test -I$(top_srcdir)/src \
           -I../src/parser -I$(top_srcdir)/src/parser -I$(top_srcdir)/src/common

bin_PROGRAMS = tester
tester_LDADD = $(top_builddir)/src/parser/libparser.la \
               $(top_builddir)/src/common/libcommon.la \
               $(top_builddir)/src/symboltable/libsymboltable.la $(ANTLR_LIB)

EXTRA_DIST = compiler_test.g


BUILT_SOURCES = LangTestLexer.hpp \
                LangTestLexer.cpp \
                LangTestParser.hpp \
                LangTestParser.cpp \
                testTokenNames.hpp


tester_SOURCES = tester.cpp \
                 TestAST.hpp \
                 TestAST.cpp \
                 $(BUILT_SOURCES)


CLEANFILES = LangTestLexer.cpp \
             LangTestLexer.hpp \
             LangTestTokenTypes.hpp \
             LangTestTokenTypes.txt \
             LangTestParser.cpp \
						 LangTestParser.hpp  \
             LangTestParserTokenTypes.hpp \
             LangTestParserTokenTypes.txt \
             LangTestLexerTokenTypes.hpp \
             LangTestLexerTokenTypes.txt \
             testTokenNames.hpp


LangTestLexer.cpp LangTestLexer.hpp LangTestParser.hpp LangTestParser.cpp: \
     $(srcdir)/compiler_test.g
	$(ANTLR_BIN) $(srcdir)/compiler_test.g

testTokenNames.hpp: Makefile
	echo "std::map<int, std::string > testTokenNames;"  > $@
	grep = LangTestLexerTokenTypes.txt >> $@
	sed -i -e 's/([^=]*)//' $@
	sed -i -e 's/"[^"]*"//' $@
	sed -i -e 's/==/=/' $@
	sed -i -e 's/()//' $@
	sed -i -e 's/\([^=]*\)=\(.*\)/testTokenNames[\2]="\1";/' $@
