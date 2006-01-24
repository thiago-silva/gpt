INCLUDES = -I$(top_srcdir)/src/common -I$(top_srcdir)/src/gpteditor \
	-I$(top_srcdir)/src/settings $(all_includes)
METASOURCES = AUTO

noinst_LTLIBRARIES = libdebuggergpt.la
noinst_HEADERS = debuggergpt.h gptnet.h gptvariable.h gptvariableparser.h
libdebuggergpt_la_SOURCES = debuggergpt.cpp gptnet.cpp gptvariable.cpp \
	gptvariableparser.cpp
libdebuggergpt_la_LIBADD = \
	$(top_builddir)/src/settings/widgets/libsettingswidget.la $(top_builddir)/src/common/libcommon.la \
	$(top_builddir)/src/settings/libsettings.la 


