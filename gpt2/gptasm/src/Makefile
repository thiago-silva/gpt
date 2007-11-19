COMMON_DIR=../../common/src/

INCLUDE_DIR=-I/usr/include/antlr/ -I$(COMMON_DIR)
CC=g++
CCFLAGS=-g -Wall

objects = GptAssemblyLexer.o GptAssemblyParser.o main.o CGptAsm.o CGenBytecode.o \
	$(COMMON_DIR)CSymbol.o $(COMMON_DIR)CSymbolTable.o $(COMMON_DIR)CCode.o $(COMMON_DIR)CHeader.o \
        $(COMMON_DIR)CData.o $(COMMON_DIR)CBinString.o $(COMMON_DIR)CSymbolList.o

all: $(objects)
	g++ -o gptasm $(objects) /usr/local/lib/libantlr.a

GptAssemblyLexer.hpp GptAssemblyLexer.cpp: lexer.g
	antlr lexer.g

GptAssemblyParser.hpp GptAssemblyParser.cpp: parser.g
	antlr parser.g


%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@


clean:
	rm -f *.o $(COMMON_DIR)*.o GptAssemblyLexer.cpp GptAssemblyLexer.hpp GptAssemblyLexerTokenTypes.hpp \
	GptAssemblyLexerTokenTypes.txt GptAssemblyParser.cpp GptAssemblyParser.hpp gptasm \
	GptAssemblyParserTokenTypes.hpp GptAssemblyParserTokenTypes.txt 
