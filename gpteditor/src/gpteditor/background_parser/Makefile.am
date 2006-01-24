INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/gpteditor \
	-I$(top_srcdir)/src/gpteditor/background_parser $(all_includes)
METASOURCES = AUTO

KDE_CXXFLAGS = $(USE_EXCEPTIONS)
noinst_LTLIBRARIES = libbackground_parser.la
noinst_HEADERS = backgroundparser.h
libbackground_parser_la_SOURCES = backgroundparser.cpp
libbackground_parser_la_LIBADD = -lgportugol -lantlr
