INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/settings $(all_includes)
METASOURCES = AUTO

# Application settings data


noinst_LTLIBRARIES = libsettings.la

libsettings_la_LDFLAGS = $(all_libraries)

SUBDIRS = widgets
libsettings_la_SOURCES = gpteditorsettings.cpp 

noinst_HEADERS = gpteditorsettings.h 

libsettings_la_LIBADD =\
	$(top_builddir)/src/settings/widgets/libsettingswidget.la
