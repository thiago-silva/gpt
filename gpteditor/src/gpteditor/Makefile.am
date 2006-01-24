# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/src/common -I$(top_srcdir)/src/debuggergpt \
	-I$(top_srcdir)/src/gpteditor -I$(top_srcdir)/src/gpteditor/background_parser -I$(top_srcdir)/src/settings \
	-I$(top_srcdir)/src/settings/widgets $(all_includes)

# these are the headers for your project
noinst_HEADERS = editortabwidget.h debuggermanager.h \
	variableslistview.h debuggercombostack.h variableslistviewitem.h messagelistview.h \
	watchlistview.h breakpointlistview.h breakpointlistviewitem.h mainwindow.h document.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

# this is where the desktop file will go
shelldesktopdir = $(kde_appsdir)/Development
shelldesktop_DATA = gpteditor.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/gpteditor
shellrc_DATA = gpteditorui.rc

mimetypetextdata_DATA = x-gptsrc.desktop
mimetypetextdatadir = $(kde_mimedir)/text


bin_PROGRAMS = gpteditor
gpteditor_LDFLAGS = $(KDE_RPATH) $(all_libraries)
gpteditor_SOURCES = editortabwidget.cpp \
	debuggermanager.cpp variableslistview.cpp debuggercombostack.cpp variableslistviewitem.cpp \
	messagelistview.cpp watchlistview.cpp breakpointlistview.cpp breakpointlistviewitem.cpp \
	mainwindow.cpp gpteditor.cpp document.cpp

gpteditor_LDADD = $(top_builddir)/src/settings/widgets/libsettingswidget.la \
	$(top_builddir)/src/settings/libsettings.la $(top_builddir)/src/debuggergpt/libdebuggergpt.la \
	$(top_builddir)/src/common/libcommon.la -lpcrecpp -lkatepartinterfaces $(LIB_KDEUI)

if BG_PARSER
SUBDIRS = background_parser
gpteditor_LDADD += $(top_builddir)/src/gpteditor/background_parser/libbackground_parser.la
endif
