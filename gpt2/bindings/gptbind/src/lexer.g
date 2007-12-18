header {
   #include <string>
   #include <sstream>
}


options {
   language="Cpp";  
}


class GptBindLexer extends Lexer;


options {
   k=2;
   charVocabulary='\0'..'\377';
   exportVocab=GptBindLexer;
  testLiterals = true;
//  testLiterals = false;
//  filter=T_INVALID;
//  genHashLines=false;//no #line
}


tokens {
   T_KW_OPTIONS="options";
   T_KW_HEADERS="headers";
   T_KW_PROCEDURE="procedure";
   T_KW_FUNCTION="function";
   T_KW_INT="int";
   T_KW_REAL="real";
   T_KW_CHAR="char";
   T_KW_STRING="string";
   T_KW_BOOL="bool";
   T_KW_POINTER="pointer";
   T_KW_MATRIX="matrix";
   T_KW_DATA="data";
   T_KW_LIBNAME="libname";
   T_KW_FUNCTIONPREFIX="functionprefix";
   T_KW_LINKERLIB="linkerlib";
   T_REAL_VALUE="número real"; //nondeterminism T_INT_LIT & T_REAL_LIT
}


{
public:  

private:
}


/******************* Constant literals ***********************/

T_INT_VALUE
options {
   paraphrase = "int number";
}
   : ('0' ('c'|'C') )=> T_OCTAL_VALUE
   | ('0' ('x'|'X') )=> T_HEX_VALUE
   | ('0' ('b'|'B') )=> T_BIN_VALUE
   | T_INTEGER_VALUE
      (
         '.' (T_DIGIT)+
         {$setType(T_REAL_VALUE);}
      )?
   ;

protected
T_INTEGER_VALUE
   : (T_DIGIT)+
   ;

protected
T_OCTAL_VALUE
   : '0' ('c'|'C')
      {
         //convert to base 10
         std::string str = $getText;
         int base10;
         str = str.substr(2);//0c
         base10 = strtoul(str.c_str(), NULL, 8);

         std::stringstream s;
         s << base10;        
         std::string res = s.str();
         $setText(res);
      }
   ;

protected
T_HEX_VALUE
   : '0' ('x'|'X')
      {
         //convert to base 10
         std::string str = $getText;
         int base10;
         base10 = strtoul(str.c_str(), NULL, 16);
  
         std::stringstream s;
         s << base10;
         std::string res = s.str();
         $setText(res);      
      }
   ;

protected
T_BIN_VALUE
   : '0' ('b'|'B')
      {
         //convert to base 10
         int base10;
         std::string str = $getText.substr(2);
         base10 = strtoul(str.c_str(), NULL, 2);//0b

         std::stringstream s;
         s << base10;
         std::string res = s.str();
         $setText(res);
      }
   ;

T_CHAR_VALUE
options {
   paraphrase = "char";
}
   : '\'' ( ~( '\'' | '\\' ) | ESC )? '\''
   ;

T_STRING_VALUE
options {
   paraphrase = "string";
}
   : '"' ( ~( '"' | '\\' | '\n' | '\r') | ESC)* '"'
   ;

protected
ESC
   : '\\' . //permite "\a" (possibilida ser avaliado posteriormente como "a")
   ;


T_COLON
options {
   paraphrase = "':'";
}
   : ':'
   ;
  
T_COMMA
options {
   paraphrase = "','";
}
   : ','
   ;

T_MAPINTO
options {
   paraphrase = "::=";
}
   : "::="
   ;

T_ABREC
options {
  paraphrase = "'{'";
}
  : '{'
  ;

T_FECHAC
options {
  paraphrase = "'}'";
}
  : '}'
  ;

T_ABREP
options {
  paraphrase = "'('";
}
  : '('
  ;

T_FECHAP
options {
  paraphrase = "')'";
}
  : ')'
  ;


T_EQUAL
options {
  paraphrase = "'='";
}
  : '='
  ;


T_SEMICOLON
options {
  paraphrase = "';'";
}
  : ';'
  ;


T_WS_ : (' '
   | '\t'
   | '\n' { newline(); }
   | '\r')
      { $setType(antlr::Token::SKIP); }
   ;

SL_COMMENT
   : "//" (~('\n'))* ('\n')?
      { 
         newline();
         $setType(antlr::Token::SKIP);
      }
   ;

ML_COMMENT
{int line = getLine();}
   : "/*" 
      ( 
         options { generateAmbigWarnings=false; } :  
         '\n'                     {newline();}
         | ('\r' '\n')=> '\r' '\n'  {newline();}
         | '\r'                     {newline();}
         |~('*'|'\n'|'\r')
         | ('*' ~'/' )=> '*' 
      )* 
      "*/"
      {$setType(antlr::Token::SKIP);}
   ;

exception
catch[antlr::RecognitionException] {  
   std::stringstream s;
   s << "AVISO: comentário iniciado na linha " << line << " não termina com \"*/\".";

   _ttype = antlr::Token::SKIP;
}

T_ID
   : (T_LETTER | '_') (T_LETTER | T_DIGIT | '_')*
   ;

protected
T_DIGIT
   : '0'..'9'
   ;

protected
T_LETTER
   : 'a'..'z'|'A'..'Z' | '\u00C0'..'\u00FF' /* latim-1 */
   ;


protected
T_INVALID
   : . 
      {
         printf("INVALID %d == '%x' -> '%c'\n", $getText.c_str()[0], $getText.c_str()[0], $getText.c_str()[0]);
      }
   ;

