INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/test -I$(top_srcdir)/src -I../src

bin_PROGRAMS = tester
tester_LDADD = $(top_builddir)/src/libgptc.la $(ANTLR_LIB)

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
             tokenNames.hpp


LangTestLexer.cpp LangTestLexer.hpp LangTestParser.hpp LangTestParser.cpp: \
     Makefile $(srcdir)/compiler_test.g
	$(ANTLR_BIN) $(srcdir)/compiler_test.g

testTokenNames.hpp: Makefile
	echo "std::map<int, std::string > testTokenNames;"  > $@
	grep = LangTestLexerTokenTypes.txt >> $@
	sed -i -e 's/([^=]*)//' $@
	sed -i -e 's/"[^"]*"//' $@
	sed -i -e 's/==/=/' $@
	sed -i -e 's/()//' $@
	sed -i -e 's/\([^=]*\)=\(.*\)/testTokenNames[\2]="\1";/' $@
