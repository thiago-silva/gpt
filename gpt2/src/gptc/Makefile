objects = PortugolLexer.o PortugolParser.o \
			    MismatchedUnicodeCharException.o teste.o

teste: $(objects)
	g++ -o teste $(objects) /usr/local/lib/libantlr.a

PortugolLexer.o: PortugolLexer.hpp PortugolLexer.cpp
	g++ -c PortugolLexer.cpp

PortugolParser.o: PortugolParser.hpp PortugolParser.cpp
	g++ -c PortugolParser.cpp

MismatchedUnicodeCharException.o:
	g++ -c MismatchedUnicodeCharException.cpp

teste.o: teste.cpp table.hpp
	g++ -c teste.cpp

PortugolLexer.hpp PortugolLexer.cpp: lexer.g
	antlr lexer.g

PortugolParser.hpp PortugolParser.cpp: parser.g
	antlr parser.g

table.hpp:
	grep = PortugolTokenTypes.txt > table.hpp
	sed -i -e 's/([^=]*)//' table.hpp
	sed -i -e 's/"[^"]*"//' table.hpp
	sed -i -e 's/==/=/' table.hpp
	sed -i -e 's/\([^=]*\)=\(.*\)/table[\2]="\1";/' table.hpp


clean:
	rm -f *.o PortugolLexer.cpp PortugolLexer.hpp PortugolTokenTypes.hpp \
	PortugolTokenTypes.txt PortugolParser.cpp PortugolParser.hpp teste \
	PortugolParserTokenTypes.hpp PortugolParserTokenTypes.txt table.hpp
