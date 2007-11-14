output = ./obj

objects = $(output)/PortugolLexer.o $(output)/PortugolParser.o \
					$(output)/BaseSemanticWalker.o $(output)/SemanticWalker.o \
					$(output)/teste.o $(output)/MismatchedUnicodeCharException.o


cpp = g++ -I$(output) -I.

$(output)/teste: $(objects)
	$(cpp) -o $(output)/teste $(objects) /usr/local/lib/libantlr.a

$(output):
	mkdir -p $(output)

$(output)/PortugolLexer.o: $(output)/PortugolLexer.hpp \
		$(output)/PortugolLexer.cpp
	$(cpp) -c $(output)/PortugolLexer.cpp -o $(output)/PortugolLexer.o

$(output)/PortugolParser.o: $(output)/PortugolParser.hpp \
		$(output)/PortugolParser.cpp
	$(cpp) -c $(output)/PortugolParser.cpp -o $(output)/PortugolParser.o

$(output)/SemanticWalker.o: $(output)/SemanticWalker.hpp \
		BaseSemanticWalker.hpp
	$(cpp) -c $(output)/SemanticWalker.cpp -o $(output)/SemanticWalker.o

$(output)/BaseSemanticWalker.o: BaseSemanticWalker.hpp BaseSemanticWalker.cpp
	$(cpp) -c BaseSemanticWalker.cpp -o $(output)/BaseSemanticWalker.o

$(output)/MismatchedUnicodeCharException.o: \
	MismatchedUnicodeCharException.cpp MismatchedUnicodeCharException.hpp
	$(cpp) -c MismatchedUnicodeCharException.cpp \
	-o $(output)/MismatchedUnicodeCharException.o

$(output)/teste.o: teste.cpp $(output)/tokenNames.hpp
	$(cpp) -c teste.cpp -o $(output)/teste.o

$(output)/PortugolLexer.hpp $(output)/PortugolLexer.cpp: lexer.g
	antlr -o $(output) lexer.g

$(output)/PortugolParser.hpp $(output)/PortugolParser.cpp: parser.g
	antlr -o $(output) parser.g

$(output)/SemanticWalker.hpp $(output)/SemanticWalker.cpp: semantic.g
	antlr -o $(output) semantic.g

$(output)/tokenNames.hpp:
	echo "std::map<int, std::string > tokenNames;"  > $(output)/tokenNames.hpp
	grep = $(output)/PortugolTokenTypes.txt >> $(output)/tokenNames.hpp
	sed -i -e 's/([^=]*)//' $(output)/tokenNames.hpp
	sed -i -e 's/"[^"]*"//' $(output)/tokenNames.hpp
	sed -i -e 's/==/=/' $(output)/tokenNames.hpp
	sed -i -e 's/()//' $(output)/tokenNames.hpp
	sed -i -e 's/\([^=]*\)=\(.*\)/tokenNames[\2]="\1";/' \
		$(output)/tokenNames.hpp


clean:
	rm -rf $(output)

