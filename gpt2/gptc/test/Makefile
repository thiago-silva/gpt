output = ./obj

objects = $(output)/LangTestLexer.o $(output)/LangTestParser.o \
					$(output)/tester.o

other = ../obj/PortugolLexer.o ../obj/PortugolParser.o \
				../obj/MismatchedUnicodeCharException.o

cpp = g++ -I$(output) -I. -I../ -I../obj

$(output)/tester: $(output) $(objects)
	$(cpp) -o $(output)/tester $(objects) $(other) /usr/local/lib/libantlr.a

$(output):
	mkdir -p $(output)

$(output)/LangTestLexer.o: $(output)/LangTestLexer.hpp $(output)/LangTestLexer.cpp
	$(cpp) -c $(output)/LangTestLexer.cpp -o $(output)/LangTestLexer.o

$(output)/LangTestParser.o: $(output)/LangTestParser.hpp $(output)/LangTestParser.cpp
	$(cpp) -c $(output)/LangTestParser.cpp -o $(output)/LangTestParser.o

$(output)/tester.o: tester.cpp
	$(cpp) -c tester.cpp -o $(output)/tester.o

$(output)/LangTestLexer.hpp $(output)/LangTestLexer.cpp \
$(output)/LangTestParser.hpp $(output)/LangTestParser.cpp: compiler_test.g
	antlr -o $(output) compiler_test.g


# $(output)/table.hpp:
# 	grep = $(output)/LangTestLexerTokenTypes.txt > $(output)/table.hpp
# 	sed -i -e 's/([^=]*)//' $(output)/table.hpp
# 	sed -i -e 's/"[^"]*"//' $(output)/table.hpp
# 	sed -i -e 's/==/=/' $(output)/table.hpp
# 	sed -i -e 's/\([^=]*\)=\(.*\)/table[\2]="\1";/' $(output)/table.hpp


clean:
	rm -rf $(output)


