INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/interpreter -I$(top_srcdir)/src/modules/parser \
	$(ANTLR_INC) $(PCRE_INC) $(all_includes)
	
noinst_LTLIBRARIES = libinterpreter.la

BUILT_SOURCES = InterpreterWalker.cpp

nodist_libinterpreter_la_SOURCES = $(BUILT_SOURCES)

libinterpreter_la_SOURCES = InterpreterEval.cpp

if BUILD_DEBUGGER
libinterpreter_la_SOURCES += InterpreterDBG.cpp 
endif

noinst_HEADERS = InterpreterDBG.hpp InterpreterEval.hpp

interpreter_g = interpreter.g
EXTRA_DIST = $(interpreter_g)

CLEANFILES = InterpreterWalker.cpp InterpreterWalker.hpp InterpreterWalkerTokenTypes.hpp \
             InterpreterWalkerTokenTypes.txt PortugolTokenTypes.txt

InterpreterWalker.cpp: $(srcdir)/$(interpreter_g)
	$(ANTLR_BIN) $(srcdir)/$(interpreter_g)

