/*

teste
  caso {
    algoritmo teste
  }

  ast: ([T_ALGORITMO,"algoritmo"] [T_IDENTIFICADOR,"teste"])
  erros: [ES001] //ponto virgula
fim

Codigos de erros:

EXXXX - Erro XXXX
ESXXX - Erro Sintatico
EMXXX - Erro Semantico


*/

header {

  #include "PortugolParser.hpp"
  #include "PortugolLexer.hpp"

  #include <antlr/TokenBuffer.hpp>
  #include <antlr/CommonAST.hpp>

  #include <iostream>

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
  buildAST       = true;
  ASTLabelType   = "TestAST";
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
  genHashLines = false;
}

{
private:
  string searchToken(int token) {
    #include "tokenNames.hpp"
    if (token >= 4) {
      return tokenNames[token];
    } else {
      return "EOF";
    }
  }
}

teste_desc
  : bloco EOF
  ;

bloco
  : "teste" codigo:T_CODIGO

    {
      stringstream fi(codigo->getText());
      PortugolLexer lexer(fi, true);
      PortugolParser parser(lexer,false);

      antlr::ASTFactory ast_factory(antlr::CommonAST::TYPE_NAME,
                                      &antlr::CommonAST::factory);
      parser.initializeASTFactory(ast_factory);
      parser.setASTFactory(&ast_factory);

      try {
        parser.programa();
      } catch ( ... ) {
        cerr << "Houveram erros...\n";
      }

      RefAST root = parser.getAST();

      RefCommonAST cast = antlr::RefCommonAST(root);

      if (cast) {
        std::cerr << cast->toStringList() << std::endl << std::endl;
      } else {
        cerr << "no tree!\n";
      }
    }

    (erros)? (ast[root])? "fim" (bloco)?
  ;

erros
  : "erros" T_DOIS_PONTOS T_ABRE_CHAVE (T_TOKEN_NAME)* T_FECHA_CHAVE
  ;

ast[RefAST node]
  : "ast" T_DOIS_PONTOS lista[node]
  ;

/*
TODO:
  Se "ast: ()" omitir nós, este parser não saberá.

  -Criar a AST do "ast ()" e, ao final, comparar com
    ast->equalsList(other)
*/

lista[RefAST node]
{
  RefAST child;
  if (node != nullAST) child = node->getFirstChild();
}

  : T_ABRE_PAREN atomo[node] (cdr[child])? T_FECHA_PAREN
  ;

cdr[RefAST node]
{
  RefAST sib;
  if (node != nullAST) sib = node->getNextSibling();
}
  : (lista[node] | atomo[node])
    (irmao[sib] {if (sib != nullAST) sib = sib->getNextSibling();})*
  ;

irmao[RefAST node]
  : lista[node]
  | atomo[node]
  ;

atomo[RefAST node]
  : tk:T_TOKEN_NAME
      {
        if (node == nullAST) {
          cerr << "erro(ast) " << tk->getLine()
                << ": esperando " << tk->getText() << " encontrado *null*\n";
        } else if(tk->getText() != searchToken(node->getType())) {
          cerr << "erro(ast) " << tk->getLine()
               << ": esperando " << tk->getText()
               << ", encontrado '" << searchToken(node->getType()) << "'\n";
        }
      }
  ;

