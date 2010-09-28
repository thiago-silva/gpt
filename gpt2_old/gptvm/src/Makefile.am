COMMON_IDIR = ../../../common/src

INCLUDES = -I$(top_srcdir)/. -I$(COMMON_IDIR)

bin_PROGRAMS = gptvm

gptvm_SOURCES = CBytecode.cpp  CGptVm.cpp  CRunBytecode.cpp  main.cpp \
						    CBytecode.hpp  CGptVm.hpp  CRunBytecode.hpp  

gptvm_LDADD = $(COMMON_LIB) -ldl
