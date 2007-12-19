COMMON_IDIR = ../../../common/src
COMMON_LDIR = ../../common/src

INCLUDES = -I$(top_srcdir)/.  -I$(COMMON_IDIR)

bin_PROGRAMS = gpt

gpt_SOURCES = CBytecode.cpp  CDataStack.cpp  CGptVm.cpp  CRunBytecode.cpp  main.cpp \
						  CBytecode.hpp  CDataStack.hpp  CGptVm.hpp  CRunBytecode.hpp  

gpt_LDADD = $(COMMON_LDIR)/libgptcommon.la -ldl