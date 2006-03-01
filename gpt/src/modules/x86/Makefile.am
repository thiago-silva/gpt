INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/x86 $(ANTLR_INC) $(all_includes)
METASOURCES = AUTO

gensources = X86Walker.cpp X86Walker.hpp

noinst_LTLIBRARIES = libx86.la
libx86_la_SOURCES = $(gensources) X86.cpp
noinst_HEADERS = X86.hpp

x86_g = $(srcdir)/x86.g

$(gensources): x86

x86: $(x86_g)
	$(ANTLR_BIN) $(x86_g)

distclean: clean
	rm -rf .deps

clean:
	rm -f *.cpp *.hpp *.a *.o *.txt

