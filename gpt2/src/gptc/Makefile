output = ./obj

objects = $(output)/PortugolLexer.o $(output)/PortugolParser.o \
					$(output)/SemanticWalker.o $(output)/teste.o \
					$(output)/MismatchedUnicodeCharException.o


cpp = g++ -I$(output) -I.

teste: bin $(objects)
	$(cpp) -o $(output)/teste $(objects) /usr/local/lib/libantlr.a

bin:
	mkdir -p $(output)

$(output)/PortugolLexer.o: $(output)/PortugolLexer.hpp $(output)/PortugolLexer.cpp
	$(cpp) -c $(output)/PortugolLexer.cpp -o $(output)/PortugolLexer.o

$(output)/PortugolParser.o: $(output)/PortugolParser.hpp $(output)/PortugolParser.cpp
	$(cpp) -c $(output)/PortugolParser.cpp -o $(output)/PortugolParser.o

$(output)/SemanticWalker.o: $(output)/SemanticWalker.hpp $(output)/SemanticWalker.cpp
	$(cpp) -c $(output)/SemanticWalker.cpp -o $(output)/SemanticWalker.o

$(output)/MismatchedUnicodeCharException.o:
	$(cpp) -c MismatchedUnicodeCharException.cpp \
	-o $(output)/MismatchedUnicodeCharException.o

$(output)/teste.o: teste.cpp $(output)/table.hpp
	$(cpp) -c teste.cpp -o $(output)/teste.o

$(output)/PortugolLexer.hpp $(output)/PortugolLexer.cpp: lexer.g
	antlr -o $(output) lexer.g

$(output)/PortugolParser.hpp $(output)/PortugolParser.cpp: parser.g
	antlr -o $(output) parser.g

$(output)/SemanticWalker.hpp $(output)/SemanticWalker.cpp: semantic.g
	antlr -o $(output) semantic.g

$(output)/table.hpp:
	grep = $(output)/PortugolTokenTypes.txt > $(output)/table.hpp
	sed -i -e 's/([^=]*)//' $(output)/table.hpp
	sed -i -e 's/"[^"]*"//' $(output)/table.hpp
	sed -i -e 's/==/=/' $(output)/table.hpp
	sed -i -e 's/\([^=]*\)=\(.*\)/table[\2]="\1";/' $(output)/table.hpp


clean:
	rm -rf $(output)

cleanold:
	rm -f *.o PortugolLexer.cpp PortugolLexer.hpp PortugolTokenTypes.hpp \
	PortugolTokenTypes.txt PortugolParser.cpp PortugolParser.hpp teste \
	PortugolParserTokenTypes.hpp PortugolParserTokenTypes.txt table.hpp \
	SemanticWalkerTokenTypes.txt SemanticWalkerTokenTypes.hpp \
	SemanticWalker.hpp SemanticWalker.cpp
