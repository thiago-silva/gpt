output = ./obj

objects = $(output)/LangTestLexer.o $(output)/LangTestParser.o \
          $(output)/TestAST.o $(output)/tester.o

other = ../obj/PortugolLexer.o ../obj/PortugolParser.o \
				../obj/MismatchedUnicodeCharException.o

cpp = g++ -I$(output) -I. -I../ -I../obj

$(output)/tester: $(output) $(objects)
	$(cpp) -o $(output)/tester $(objects) $(other) /usr/local/lib/libantlr.a

$(output):
	mkdir -p $(output)

$(output)/LangTestLexer.o: $(output)/LangTestLexer.hpp \
		$(output)/LangTestLexer.cpp
	$(cpp) -c $(output)/LangTestLexer.cpp -o $(output)/LangTestLexer.o

$(output)/LangTestParser.o: $(output)/LangTestParser.hpp \
		$(output)/LangTestParser.cpp
	$(cpp) -c $(output)/LangTestParser.cpp -o $(output)/LangTestParser.o

$(output)/tester.o: tester.cpp $(output)/testTokenNames.hpp
	$(cpp) -c tester.cpp -o $(output)/tester.o

$(output)/TestAST.o: TestAST.cpp TestAST.hpp
	$(cpp) -c TestAST.cpp -o $(output)/TestAST.o

$(output)/LangTestLexer.hpp $(output)/LangTestLexer.cpp \
$(output)/LangTestParser.hpp $(output)/LangTestParser.cpp: compiler_test.g
	antlr -o $(output) compiler_test.g


$(output)/testTokenNames.hpp: $(output)/LangTestLexerTokenTypes.txt
	echo "std::map<int, std::string > testTokenNames;"  \
		> $(output)/testTokenNames.hpp
	grep '=' $(output)/LangTestLexerTokenTypes.txt \
		>> $(output)/testTokenNames.hpp
	sed -i -e 's/([^=]*)//' $(output)/testTokenNames.hpp
	sed -i -e 's/"[^"]*"//' $(output)/testTokenNames.hpp
	sed -i -e 's/==/=/' $(output)/testTokenNames.hpp
	sed -i -e 's/()//' $(output)/testTokenNames.hpp
	sed -i -e 's/\([^=]*\)=\(.*\)/testTokenNames[\2]="\1";/' \
		$(output)/testTokenNames.hpp


clean:
	rm -rf $(output)


