COMMON_DIR=../../common/src/

INCLUDE_DIR=-I$(COMMON_DIR)
CC=g++
CCFLAGS=-g -Wall

GPTASM=../gptasm/

objects = main.o CGptVm.o CRunBytecode.o CBytecode.o CDataStack.o \
          $(COMMON_DIR)CSymbol.o $(COMMON_DIR)CSymbolTable.o \
          $(COMMON_DIR)CHeader.o \
          $(COMMON_DIR)CBinString.o $(COMMON_DIR)CSymbolList.o 
          
all: $(objects)
	g++ -o gptvm $(objects)

%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@


clean:
	rm -f *.o $(COMMON_DIR)*.o gptvm
