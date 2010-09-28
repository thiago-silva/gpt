COMMON_DIR=../../../common/src/

INCLUDE_DIR=-I/usr/include/antlr/ -I$(COMMON_DIR)
CC=g++
CCFLAGS=-g -Wall

objects = GptBindLexer.o GptBindParser.o main.o CGptBind.o CBindSources.o \
          $(COMMON_DIR)/CTextFile.o

all: $(objects)
	g++ -o gptbind $(objects) /usr/local/lib/libantlr.a

GptBindLexer.hpp GptBindLexer.cpp: lexer.g
	antlr lexer.g

GptBindParser.hpp GptBindParser.cpp: parser.g
	antlr parser.g


%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $(INCLUDE_DIR) $< -o $@


clean:
	rm -f *.o $(COMMON_DIR)*.o GptBindLexer.cpp GptBindLexer.hpp GptBindLexerTokenTypes.hpp \
	GptBindLexerTokenTypes.txt GptBindParser.cpp GptBindParser.hpp gptbind \
	GptBindParserTokenTypes.hpp GptBindParserTokenTypes.txt 
