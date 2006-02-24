INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/compiler \
	-I$(top_srcdir)/src/compiler/c_translator $(all_includes)
METASOURCES = AUTO

gensources = Portugol2CWalker.cpp Portugol2CWalker.hpp

noinst_LTLIBRARIES = libc_translator.la
libc_translator_la_SOURCES = $(gensources)

pt2c_g = $(srcdir)/pt2c.g

$(gensources): pt2c

pt2c: $(pt2c_g)
	$(ANTLR_BIN) $(pt2c_g)

distclean: clean
	rm -rf .deps

clean:
	rm -f *.cpp *.hpp *.a *.o *.txt
