output = ./obj

objects = $(output)/PortugolLexer.o $(output)/PortugolParser.o \
					$(output)/BaseSemanticWalker.o $(output)/SemanticWalker.o \
					$(output)/teste.o $(output)/MismatchedUnicodeCharException.o


cpp = g++ -I$(output) -I.

all: $(output) $(objects) $(output)/teste

$(output)/teste:
	$(cpp) -o $(output)/teste $(objects) /usr/local/lib/libantlr.a

$(output):
	mkdir -p $(output)

$(output)/PortugolLexer.o: $(output)/PortugolLexer.hpp
	$(cpp) -c $(output)/PortugolLexer.cpp -o $(output)/PortugolLexer.o

$(output)/PortugolParser.o: $(output)/PortugolParser.hpp
	$(cpp) -c $(output)/PortugolParser.cpp -o $(output)/PortugolParser.o

$(output)/SemanticWalker.o: $(output)/BaseSemanticWalker.o \
		$(output)/SemanticWalker.hpp
	$(cpp) -c $(output)/SemanticWalker.cpp -o $(output)/SemanticWalker.o

$(output)/BaseSemanticWalker.o: BaseSemanticWalker.hpp BaseSemanticWalker.cpp
	$(cpp) -c BaseSemanticWalker.cpp -o $(output)/BaseSemanticWalker.o

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

