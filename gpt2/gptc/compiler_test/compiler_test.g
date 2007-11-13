header {

  #include "PortugolParser.hpp"
  #include "PortugolLexer.hpp"

  #include "TestAST.hpp"

  #include <antlr/TokenBuffer.hpp>
  #include <antlr/CommonAST.hpp>

  #include <iostream>
  #include <map>
  #include <string>

  #include "UnicodeCharBuffer.hpp"
  #include "UnicodeCharScanner.hpp"

  using namespace std;
  using namespace antlr;
}


options {
  language="Cpp";
}

class LangTestLexer extends Lexer("UnicodeCharScanner");
options {
  k=2;
  genHashLines   = false;
  testLiterals   = false;
  noConstructors = true;
}

{
public:
  LangTestLexer(std::istream& in)
   : UnicodeCharScanner(new UnicodeCharBuffer(in), true)
    {
      initLiterals();
    }
}

T_VIRGULA
  : ','
  ;

T_DOIS_PONTOS
  : ':'
  ;

T_ABRE_PAREN
  : '('
  ;

T_FECHA_PAREN
  : ')'
  ;

T_ABRE_CHAVE
  : '['
  ;

T_FECHA_CHAVE
  : ']'
  ;

T_TOKEN_NAME
options {
  testLiterals = true;
}
  : ('a'..'z' | 'A'..'Z' | '0'..'9' | '_'
    | '\u00C0' .. '\u00FF' /* latim-1 */)+
  ;

T_TEXTO_LITERAL
  : '"'! ( ~( '"' | '\\' | '\n' | '\r') | T_ESCAPE )* '"'!
  ;

T_CODIGO
  : '{'!

    (
        options { generateAmbigWarnings=false; } :
        '\n'                     {newline();}
      | ~('}')
    )*

    '}'!
  ;


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

protected
T_ESCAPE
  : '\\' .
  ;

protected
T_ETC
  : . {
        $setType(antlr::Token::SKIP);
        cerr << "Letra inválida: " <<  $getText << endl;
      }
  ;


//***************************************************************


class LangTestParser extends Parser;
options {
  k=2;
  genHashLines   = false;
  buildAST       = true;
  ASTLabelType   = "RefTestAST";
}

// {
// private:
//   int searchToken(string text) {
//     #include "tokenNames.hpp"
//     std::map<int, std::string >::iterator it;
//     for (it = tokenNames.begin(); it != tokenNames.end(); it++) {
//       if (it->second == text) {
//         //return astFactory->create(it->first, "&test_empty");
//         return it->first;
//       }
//     }
//     cerr << "searchToken::leave2\n";
// //     return astFactory->create(0, "&test_invalid");
//     return it->first;
//   }
// }

teste_desc
  : bloco EOF
  ;

bloco
  : "teste" codigo:T_CODIGO

    {
      stringstream fi(codigo->getText());
      PortugolLexer lexer(fi, true);
      PortugolParser parser(lexer,false);

      ASTFactory ast_factory(antlr::CommonAST::TYPE_NAME,
                                      &antlr::CommonAST::factory);
      parser.initializeASTFactory(ast_factory);
      parser.setASTFactory(&ast_factory);

      parser.programa();

      if (parser.hasErorrs()) {
        cerr << "Parser has errors!!\n";
      }

      RefAST root = parser.getAST();

      RefCommonAST cast = antlr::RefCommonAST(root);

/*      if (cast) {
        std::cerr << cast->toStringList() << std::endl << std::endl;
      } else {
        cerr << "no tree!\n";
      }*/
    }

    (erros)?

    (ast[root])?

    "fim"

    (bloco)?
  ;

erros!
  : "erros" T_DOIS_PONTOS T_ABRE_CHAVE (T_TOKEN_NAME)* T_FECHA_CHAVE
  ;


ast[RefAST gptRoot]
  : tline:"ast"! T_DOIS_PONTOS! lista
    {
      int line = tline->getLine();
      TestAST* tast = 0;
      tast = RefTestAST(currentAST.root).get();

      if (!tast->equalsList(gptRoot)) {
        cerr << "-Erro na AST da linha [" << line << "]\n";
        cerr << "Esperando: \n-----\n" << tast->toStringList() << endl;
        cerr << "-----\nEncontrado: \n-----\n" << gptRoot->toStringList() << endl;
        cerr << "-----\n";
      }
    }
  ;



lista

  : T_ABRE_PAREN!
    T_TOKEN_NAME^  (cdr)*
    T_FECHA_PAREN!
  ;

cdr
  : T_TOKEN_NAME
  | lista
  ;

/*
lista!
{cerr << "lista::enter\n";}
  : T_ABRE_PAREN t:T_TOKEN_NAME
    {cerr << "lista::matched token\n";}
    (c:cdr)? T_FECHA_PAREN
    {
      cerr << "lista::buildingAST\n";
      #lista = #(searchToken(t->getText()),c);
      {cerr << "lista::leave\n";}
    }
  ;

cdr
{cerr << "cdr::enter\n";}
  :  lista         (cdr)?
  |! t:T_TOKEN_NAME
    (
      c:cdr
      {
        cerr << "cdr::buildingAST\n";
        #cdr = #(searchToken(t->getText()),c);
        cerr << "cdr::leaving\n";
      }

      |
        {
          cerr << "cdr::building2\n";
          #cdr = (#t);
          cerr << "cdr::leaving2\n";
        }
    )
  ;*/
