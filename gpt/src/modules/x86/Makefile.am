INCLUDES = -I$(top_srcdir)/. -I$(top_srcdir)/src/modules \
	-I$(top_srcdir)/src/modules/x86 $(ANTLR_INC) $(all_includes)
METASOURCES = AUTO

noinst_LTLIBRARIES = libx86.la

BUILT_SOURCES = X86Walker.cpp

libx86_la_SOURCES = X86.cpp
nodist_libx86_la_SOURCES = $(BUILT_SOURCES)
noinst_HEADERS = X86.hpp asm_elf.h asm_lib.h asm_prologue.h asm_win32.h

x86_g = x86.g
EXTRA_DIST = $(x86_g)

CLEANFILES = X86Walker.hpp X86Walker.cpp X86WalkerTokenTypes.hpp X86WalkerTokenTypes.txt \
             PortugolTokenTypes.txt

X86Walker.cpp: $(srcdir)/$(x86_g)
	$(ANTLR_BIN) $(srcdir)/$(x86_g)

