/*
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
 *   thiago.silva@kdemal.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
                                                                           */


header {
  #include "ErrorHandler.hpp"
  #include <string>
  #include <sstream>
  #include <iostream>
  #include <ctype.h>

  using namespace std;
}

options {
  language="Cpp";  
}

class PortugolLexer extends Lexer;
options {
  k=2;
  charVocabulary='\u0003'..'\u00FF'; // latin-1  
  exportVocab=Portugol;
  testLiterals = false;
  filter=T_INVALID;
  genHashLines=false;//no #line
}

//we have to create the tokens here, because some of them have accents
//and, if used inline in parser rules, antlr generates an entry like
//TK_algorítmos (wich is an invalid C ID, because of the accents)
tokens {
  T_KW_FIM_VARIAVEIS="fim-variáveis";
  T_KW_ALGORITMO="algoritmo";
  T_KW_VARIAVEIS="variáveis";  
  T_KW_INTEIRO="inteiro";
  T_KW_REAL="real";
  T_KW_CARACTERE="caractere";
  T_KW_LITERAL="literal";
  T_KW_LOGICO="lógico";
  T_KW_INICIO="início";
  T_KW_VERDADEIRO="verdadeiro";
  T_KW_FALSO="falso";
  T_KW_FIM="fim";
  T_KW_OU="ou";
  T_KW_E="e";
  T_KW_NOT="não";
  T_KW_SE="se";
  T_KW_SENAO="senão";
  T_KW_ENTAO="então";
  T_KW_FIM_SE="fim-se";
  T_KW_ENQUANTO="enquanto";
  T_KW_FACA="faça";
  T_KW_FIM_ENQUANTO="fim-enquanto";
  T_KW_PARA="para";
  T_KW_DE="de";
  T_KW_ATE="até";
  T_KW_FIM_PARA="fim-para";
  T_KW_MATRIZ="matriz";
  T_KW_INTEIROS="inteiros";
  T_KW_REAIS="reais";
  T_KW_CARACTERES="caracteres";
  T_KW_LITERAIS="literais";
  T_KW_LOGICOS="lógicos";
  T_KW_FUNCAO="função";
  T_KW_RETORNE="retorne";  
  T_KW_PASSO="passo";

  T_REAL_LIT="número real"; //nondeterminism T_INT_LIT & T_REAL_LIT

  //imaginaries for AST
  TI_UN_POS;
  TI_UN_NEG;
  TI_UN_NOT;
  TI_UN_BNOT;
  TI_PARENTHESIS;
  TI_FCALL;
  TI_FRETURN;
  TI_VAR_PRIMITIVE;
  TI_VAR_MATRIX;
  TI_NULL;
}

{
  bool hasLatim;
}
/*------------------------- Operators -----------------------*/

T_BIT_OU
options {
  paraphrase = "operador '|'";
}
  : '|'
  ;
  
T_BIT_XOU 
options {
  paraphrase = "operador '^'";
}
  : '^'
  ;
  
T_BIT_E
options {
  paraphrase = "operador '&'";
}
  : '&'
  ;
  
T_BIT_NOT
options {
  paraphrase = "operador '~'";
}
  : '~'
  ;
  
T_IGUAL
options {
  paraphrase = "operador '='";
}
  : '='
  ;
  
T_DIFERENTE
options {
  paraphrase = "operador '<>'";
}
  : "<>"
  ;
  
T_MAIOR
options {
  paraphrase = "operador '>'";
}
  : '>'
  ;
  
T_MENOR
options {
  paraphrase = "operador '<'";
}
  : '<'
  ;
  
T_MAIOR_EQ
options {
  paraphrase = "operador '>='";
}
  : ">="
  ;
  
T_MENOR_EQ
options {
  paraphrase = "operador '<='";
}
  : "<="
  ;
  
T_MAIS
options {
  paraphrase = "operador '+'";
}
  : '+'
  ;
  
T_MENOS
options {
  paraphrase = "operador '-'";
}
  : '-'
  ;
  
T_DIV
options {
  paraphrase = "operador '/'";
}
  : {LA(2)!= '/' && LA(2)!= '*'}? '/'
  ;
  
T_MULTIP
options {
  paraphrase = "operador '*'";
}
  : '*'
  ;
  
T_MOD
options {
  paraphrase = "operador '%'";
}
  : '%'
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

T_ABREC
options {
  paraphrase = "'['";
}
  : '['
  ;

T_FECHAC
options {
  paraphrase = "']'";
}
  : ']'
  ;

/*-----------------Constant literals ***********************/


T_INT_LIT
options {
  paraphrase = "número inteiro";
}
  : ('0' ('c'|'C') )=> T_OCTAL_LIT
  | ('0' ('x'|'X') )=> T_HEX_LIT
  | ('0' ('b'|'B') )=> T_BIN_LIT
  | T_INTEGER_LIT 
    (
      '.' (T_DIGIT)+
      {$setType(T_REAL_LIT);}
    )?
  ;

protected
T_INTEGER_LIT
  : (T_DIGIT)+
  ;

protected
T_OCTAL_LIT
  : '0' ('c'|'C') (T_LETTER_OR_DIGIT)+ //T_LETTER_OR_DIGIT apenas para capturar erro corretamente
      {
        bool haserror = false;
        string str = $getText;
        if((str.find("8",0) != string::npos) ||
           (str.find("9",0) != string::npos) ) {
          stringstream s;
          s << "\"" << $getText << "\" não é um valor octal válido";
          ErrorHandler::self()->add(s.str(), getLine());          
          haserror = true;
        } else {
          for(unsigned int i = 2; i < str.length(); ++i) {
            if(!isdigit(str[i])) {
              stringstream s;
              s << "\"" << str << "\" não é um valor hexadecimal válido";
              ErrorHandler::self()->add(s.str(), getLine());
              haserror = true;
              break;
            }
          }
        }
        if(!haserror) {
          //convert to base 10
          int base10;
          str = str.substr(2);//0c
          base10 = strtoul(str.c_str(), NULL, 8);

          stringstream s;
          s << base10;        
          string res = s.str();
          $setText(res);
        }
      }
  ;

protected
T_HEX_LIT
  : '0' ('x'|'X') (T_LETTER_OR_DIGIT)+ //T_LETTER_OR_DIGIT apenas para capturar erro corretamente
    {
      bool haserror = false;
      string str = $getText;
      for(unsigned int i = 2; i < str.length(); ++i) {
        if(!isxdigit(str[i])) {
          stringstream s;
          s << "\"" << str << "\" não é um valor hexadecimal válido";
          ErrorHandler::self()->add(s.str(), getLine());
          haserror = true;
          break;
        }
      }
      if(!haserror) {
        //convert to base 10
        int base10;
        base10 = strtoul(str.c_str(), NULL, 16);
  
        stringstream s;
        s << base10;
        string res = s.str();
        $setText(res);      
      }
    }
  ;

protected
T_BIN_LIT
  : '0' ('b'|'B') (T_LETTER_OR_DIGIT)+
  {
    string str = $getText;
    bool haserror = false;
    for(unsigned int i = 2; i < str.length(); ++i) {
      if((str[i] != '0') && (str[i] != '1')) {
        stringstream s;
        s << "\"" << str << "\" não é um valor binário válido";
        ErrorHandler::self()->add(s.str(), getLine());
        haserror = true;
        break;
      }
    }
    if(!haserror) {
      //convert to base 10
      int base10;
      string str = $getText.substr(2);
      base10 = strtoul(str.c_str(), NULL, 2);//0b

      stringstream s;
      s << base10;
      string res = s.str();
      $setText(res);
    }
  }
  ;

T_CARAC_LIT
options {
  paraphrase = "caractere";
}
//  : '\''! ( ~('\''|'\n'|'\r'|'\\') | ESC )? '\''!
  : '\''! ( ~( '\'' | '\\' ) | ESC )? '\''!
  ;

//"Digite um \");

T_STRING_LIT
options {
  paraphrase = "literal";
}
//  : '"'! (ESC|~('"'|'\\'|'\n'|'\r'))* '"'!
  : '"'! ( ~( '"' | '\\' | '\n' | '\r') | ESC)* '"'!
  ;

protected
ESC
  //: '\\' ('n'| 't' | 'r' | '\\' | '\'' | '"')
  : '\\' . //permite "\a" (possibilida ser avaliado posteriormente como "a")
  ;

T_ATTR
options {
  paraphrase = "':='";
}
  : ':' '='
  ;

T_SEMICOL
options {
  paraphrase = "';'";
}
  : ';'
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
  stringstream s;
  s << "AVISO: comentário iniciado na linha " << line << " não termina com \"*/\".";
  ErrorHandler::self()->add(s.str(), getLine());

  _ttype = antlr::Token::SKIP;
}

/* Now, thats reeeally tricky:
  Here is the problem:
  
  we need a way to get tokens such as "fim-variaveis".
  those tokens are listed on tokens{} section, so
  in order to match those tokens, we need a rule
  that covers all the keywords, then, we use testLiterals=true.
  
  If there is a token on tokens{} section that
  cannot be matched by any rule, it will never
  be matched (since, there will be no rule to call testLiterals
  and create the desireble token).
  
  So, the problem arrives when we add a token with 
  hiphen (such as "fim-variaveis") on tokens{} section.    
  The T_IDENTIFIER should not accept '-', so, there is no
  other general rule that can match "fim-variaveis" to call testLiterals
  and return it to the parser.
  
  The workaround for this is to generalize T_IDENTIFIER
  to support '-', but marking its state right before the '-'.
  after the matching process, we check if there is an '-' on the
  matched token. if there is, we check for the tokens on tokens{} secion.
  if is a match, we don't do anything, and let the testLiterals do its work.
  Else, it means we have something like "a-b". So we need to rewind to
  "a", right before the match of '-', synchorinize the current matched text
  (wich now is "a-b") to "a", and let it roll again (wich will dispatch
  a token T_IDENTIFIER with text "a", and start again from '-' point).
*/
protected
T_ID_AUX
  : (T_LETTER | '_') (T_LETTER_OR_DIGIT)*
  ;

protected
T_LETTER_OR_DIGIT
  : T_LETTER | T_DIGIT | '_'
  ;

T_IDENTIFICADOR
options {
  testLiterals = true;
   //nota: identificador pode ser:
   // -nome do algoritmo (ver declaração de algoritmo)
   // -variável
   // -nome de função
//   paraphrase = "variável/função";
}
  { int m=-1,len; hasLatim=false;}
  
  : T_ID_AUX
      {
        len=$getText.length();
        if(LA(1)=='-') {
          m=mark();          
        }
      }
      
    ('-' (T_LETTER_OR_DIGIT)*)?
  {
    
    if(m != -1) {
      if(testLiteralsTable(_ttype) == T_IDENTIFICADOR) {
        rewind(m);
        std::string s = $getText;
        $setText(s.substr(0,len));
      }
    }

    //check for latim non-keywords
    if(hasLatim && (testLiteralsTable(_ttype) == T_IDENTIFICADOR)) {
      stringstream s;
      s << "Identificador \"" << $getText << "\" não pode ter caracteres especiais.";
      ErrorHandler::self()->add(s.str(), getLine());
    }
  }
  ;


protected
T_DIGIT
  : '0'..'9'
  ;

protected
T_LETTER
  : 'a'..'z'|'A'..'Z' | {hasLatim=true;}'\u00C0' .. '\u00FF' /* latim-1 */
  ;


protected
T_INVALID
  : . 
    {
      stringstream s;
      if(($getText != "\"") && ($getText != "'")) {
        s << "Caractere inválido: \"" << $getText << "\"";
      } else {
        s << "Faltando fechar aspas";
      }
      ErrorHandler::self()->add(s.str(), getLine());
    }
  ;