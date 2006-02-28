INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/interpreter -I$(top_srcdir)/src/modules/parser \
	$(ANTLR_INC) $(PCRE_INC) $(all_includes)
	
METASOURCES = AUTO

noinst_LTLIBRARIES = libinterpreter.la

interpreter_g = $(srcdir)/interpreter.g

gensources = InterpreterWalker.hpp InterpreterWalker.cpp

libinterpreter_la_SOURCES = $(gensources) InterpreterDBG.cpp InterpreterEval.cpp
noinst_HEADERS = InterpreterDBG.hpp InterpreterEval.hpp

$(gensources): interpreter

interpreter : $(interpreter_g)
	$(ANTLR_BIN) $(interpreter_g)

distclean: clean
	rm -rf .deps

clean:
	rm -f *.cpp *.hpp *.a *.o *.txt

