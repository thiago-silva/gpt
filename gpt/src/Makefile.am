bin_PROGRAMS = gpt
gpt_SOURCES = gpt.cpp ErrorHandler.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/src/parser -Iparser \
	$(all_includes)

gpt_LDADD = $(top_builddir)/src/parser/libparser.a -lantlr
 
SUBDIRS = parser
noinst_HEADERS = ErrorHandler.hpp
