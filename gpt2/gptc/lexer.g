/*
 *   Copyright (C) 2003-2006 by Thiago Silva                               *
 *   tsilva@sourcecraft.info                                               *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             */


header {
  #include <string>
  #include <sstream>
  #include <iostream>
  #include <ctype.h>
  #include "UnicodeCharBuffer.hpp"
  #include "UnicodeCharScanner.hpp"
}

options {
  language="Cpp";
}

class PortugolLexer extends Lexer("UnicodeCharScanner");

options {
  k=2;
  genHashLines   = false;
  exportVocab    = Portugol;
  filter         = T_INVALIDO;
  testLiterals   = false;
}

tokens {
  T_ALGORITMO="algoritmo";
  T_USE="use";
  T_VARIAVEL="variável";
  T_VARIAVEIS="variáveis";
  T_FIM_VARIAVEIS="fim-variáveis";
  T_INTEIRO="inteiro";
  T_REAL="real";
  T_CARACTERE="caractere";
  T_LITERAL="literal";
  T_LOGICO="lógico";
  T_CORINGA="coringa";
  T_MATRIZ="matriz";
  T_DO="do";
  T_TIPO="tipo";
  T_CONSTANTE="constante";
  T_CONSTANTES="constantes";
  T_FIM_CONSTANTES="fim-constantes";
  T_ESTRUTURA="estrutura";
  T_FIM_ESTRUTURA="fim-estrutura";
  T_FUNCAO="função";
  T_PROCEDIMENTO="procedimento";
  T_REF="ref";
  T_INICIO="início";
  T_FIM="fim";
  T_SAIR="sair";
  T_RETORNE="retorne";
  T_SE="se";
  T_ENTAO="então";
  T_SENAO="senão";
  T_FIM_SE="fim-se";
  T_ENQUANTO="enquanto";
  T_FACA="faça";
  T_FIM_ENQUANTO="fim-enquanto";
  T_PARA="para";
  T_DE="de";
  T_ATE="até";
  T_FIM_PARA="fim-para";
  T_PASSO="passo";
  T_REPITA="repita";
  T_CASO="caso";
  T_SEJA="seja";
  T_FIM_SENAO="fim-senão";
  T_FIM_CASO="fim-caso";
  T_FIM_FACA="fim-faça";
  T_OU="ou";
  T_E="e";
  T_NAO="não";
  T_VERDADEIRO="verdadeiro";
  T_FALSO="falso";
  T_NULO="nulo";
  T_ASM="asm";
  T_FIMASM="fim-asm";

  T_REAL_LITERAL="número real"; //a regra T_INTEIRO_LITERAL lida com reais


  //imaginários para a AST

  T_PARAM;
  T_TIPO_RETORNO;

  T_VALOR;
  T_VAL_MATRIZ;    //valor da matriz    (literal [])
  T_VAL_ESTRUTURA; //valor da estrutura (literal {})

  T_DIMENSOES;

  T_UN_POSITIVO;
  T_UN_NEGATIVO;

  T_CALL;
}


{
public:
  PortugolLexer(std::istream& in, bool case_s)
   : UnicodeCharScanner(new UnicodeCharBuffer(in), case_s)
    {
      initLiterals();
    }

private:

  std::string toBase10(const std::string& oct, int from) {
    std::stringstream stream;
    unsigned long long base10;
    base10 = strtoull(oct.c_str(), NULL, from);
    stream << base10;
    return stream.str();
  }

  bool isOctal(const std::string& num) {
    std::stringstream s;
    if ((num.find("8",0)  != std::string::npos) ||
        (num.find("9",0)  != std::string::npos)) {
      return false;
    } else {
      for (unsigned int i = 2; i < num.length(); ++i) {
        if (!isdigit(num[i])) {
          return false;
        }
      }
    }
    return true;
  }

  bool isHex(const std::string& num) {
    for (unsigned int i = 2; i < num.length(); ++i) {
      if (!isxdigit(num[i])) {
        return false;
      }
    }
    return true;
  }

  bool isBin(const std::string& num) {
    for(unsigned int i = 2; i < num.length(); ++i) {
      if ((num[i] != '0') && (num[i] != '1')) {
        return false;
      }
    }
    return true;
  }
}


//####################################
//##             Regras             ##
//####################################



T_ESPACO :
  (   ' '
    | '\t'
    | '\n' { newline(); }
    | '\r'
  )
  { $setType(antlr::Token::SKIP); }
  ;

COMENTARIO_CPP
  : "//" (~('\n'))* ('\n')?
    {
      newline();
      $setType(antlr::Token::SKIP);
    }
  ;

COMENTARIO_C
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
  //aviso! não é erro!
  s << "comentário iniciado na linha " << line << " não termina com \"*/\".";
  std::cerr << s.str() << ", na linha " << getLine() << std::endl;
  $setType(antlr::Token::SKIP);
}

T_PONTO_VIRGULA
options {
  paraphrase = "';'";
}
  : ';'
  ;

T_VIRGULA
options {
  paraphrase = "','";
}
  : ','
  ;

T_2_PONTOS
options {
  paraphrase = "':'";
}

  : ':'
  ;

T_ABRE_CHAVE
options {
  paraphrase = "'['";
}
  : '['
  ;

T_FECHA_CHAVE
options {
  paraphrase = "']'";
}
  : ']'
  ;

T_ABRE_COLCHETE
options {
  paraphrase = "'{'";
}
  : '{'
  ;

T_FECHA_COLCHETE
options {
  paraphrase = "'}'";
}
  : '}'
  ;

T_ABRE_PAREN
options {
  paraphrase = "'('";
}
  : '('
  ;
T_FECHA_PAREN
options {
  paraphrase = "')'";
}
  : ')'
  ;

T_ATRIBUICAO
options {
  paraphrase = "':='";
}
  : ":="
  ;

T_BIT_OU
options {
  paraphrase = "operador '|'";
}
  : '|'
  ;

T_BIT_OUX
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

T_BIT_NAO
options {
  paraphrase = "operador '~'";
}
  : '~'
  ;

T_BIT_SHIFT_LEFT
options {
  paraphrase = "operador '<<'";
}
  : "<<"
  ;

T_BIT_SHIFT_RIGHT
options {
  paraphrase = "operador '>>'";
}

  : ">>"
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

T_MAIOR_EQ
options {
  paraphrase = "operador '>='";
}
  : ">="
  ;

T_MENOR
options {
  paraphrase = "operador '<'";
}
  : '<'
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
  : '/'
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

/* Literais */


T_INTEIRO_LITERAL
options {
  paraphrase = "número inteiro";
}
  : ('0' ('c'|'C') )=> T_OCTAL_LIT
  | ('0' ('x'|'X') )=> T_HEX_LIT
  | ('0' ('b'|'B') )=> T_BIN_LIT
  | (T_DIGITO)+
    (
      '.' (T_DIGITO)+
      {$setType(T_REAL_LITERAL);}
    )?
  ;

T_TEXTO_LITERAL
options {
  paraphrase = "literal";
}
  : '"'! ( ~( '"' | '\\' | '\n' | '\r') | T_ESCAPE )* '"'!
  ;


T_CARACTERE_LITERAL
options {
  paraphrase = "caractere";
}
  : '\''! ( ~( '\'' | '\\' ) | T_ESCAPE )? '\''!
  ;



/*
  Ok, um pouco de malabarismo...

  Eis o problema:

  Nós precisamos que uma string como "fim-variáveis" seja
  identificada pelo scanner. Para isso, as keywords em tokens{} devem
  ser cobertos pelas regras T_*. No caso, T_IDENTIFICADOR
  costuma ser a regra que identifica keywords.

  Porém, "fim-variáveis" não é um identificador válido
  (ambig. com "fim - variáveis").

  Então, T_IDENTIFICADOR aqui representa a regra mais geral possível,
  para cobrir as keywords em tokens{}. Mas, para o caso problemático
  de T_IDENTIFICADOres com "-", verificamos se o lexema atual
  é uma keyword ou um identificador seguido de T_MENOS, utilizando
  "testLiteralsTable" manualmente.

  Se testLiteralsTable indicar que o lexema atual com hifen for uma keyword,
  é a própria, obviamente.

  Se não, trata-se de um ID seguido de T_MENOS. Então, fazemos o rollback
  até o ponto do hifen.
*/

T_IDENTIFICADOR
options {
  testLiterals = true;
}
  { int m = -1,len; }

  : T_ID_AUX
      {
        len = $getText.length();
        if (LA(1) == '-') {
          m = mark();
        }
      }

    ('-' (T_LETRA_OU_DIGITO)*)?
  {
    if ((m != -1) && (testLiteralsTable(_ttype) == T_IDENTIFICADOR)) {
      rewind(m);
      std::string s = $getText;
      $setText(s.substr(0,len));
    }
  }
  ;


T_RETICENCIAS
options {
  paraphrase = "reticências (...)";
}
  : "..."
  ;

T_PONTO
options {
  paraphrase = "ponto (.)";
}
  : '.'
  ;

//####################################
//##            Protecteds          ##
//####################################


protected
T_ESCAPE
  : '\\' .  /* ex. \a */
  ;

protected
T_DIGITO
  : '0'..'9'
  ;

protected
T_LETRA
  : 'a'..'z'|'A'..'Z' | '\u00C0' .. '\u00FF' /* latim-1 */
  ;


protected
T_LETRA_OU_DIGITO
  : T_LETRA | T_DIGITO | '_'
  ;


//mais detalhes em T_IDENTIFICADOR
protected
T_ID_AUX
  : (T_LETRA | '_') (T_LETRA_OU_DIGITO)*
  ;

protected
T_OCTAL_LIT
  : '0' ('c'|'C') (T_LETRA_OU_DIGITO)+ //T_LETRA_OU_DIGITO:
                                        //apenas para exibir
                                        //erro precisamente
  {
    std::string str = $getText;
    if (isOctal(str)) {
      $setText(toBase10(str.substr(2), 8));
    } else {
      std::stringstream s;
      s << "\"" << str << "\" não é um número octal válido";
      std::cerr << s.str() << ", na linha " << getLine() << std::endl;
    }
  }
  ;

protected
T_HEX_LIT
  : '0' ('x'|'X') (T_LETRA_OU_DIGITO)+ //T_LETRA_OU_DIGITO:
                                        //apenas para exibir
                                        //erro precisamente
  {
    std::string str = $getText;
    if (isHex(str)) {
      $setText(toBase10(str, 16));
    } else {
      std::stringstream s;
      s << "\"" << str << "\" não é um número hexadecimal válido";
      std::cerr << s.str() << ", na linha " << getLine() << std::endl;
    }
  }
  ;

protected
T_BIN_LIT
  : '0' ('b'|'B') (T_LETRA_OU_DIGITO)+ //T_LETRA_OU_DIGITO:
                                        //apenas para exibir
                                        //erro precisamente
  {
    std::string str = $getText;
    if (isBin(str)) {
      $setText(toBase10(str.substr(2), 2));
    } else {
      std::stringstream s;
      s << "\"" << str << "\" não é um número binário válido";
      std::cerr << s.str() << ", na linha " << getLine() << std::endl;
    }
  }
  ;

protected
T_INVALIDO
  : .
    {
      //caracteres de espaço inexistentes na tabela ascii
      //([160] == [-96] == ' ' == [32] == 160-128)
      if ($getText.c_str()[0] == (int)0xffffffa0) {
        $setType(antlr::Token::SKIP);
      } else {
        std::stringstream s;
        if (($getText != "\"") && ($getText != "'")) {
          s << "Caractere inválido: \"" << $getText << "\".\n";
          s << "Verifique se a codificação do texto está em UTF-8";
        } else {
          s << "Faltando fechar aspas";
        }
        std::cerr << s.str() << ", na linha " << getLine() << std::endl;
      }
    }
  ;
