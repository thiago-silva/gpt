header {
   #include <string>
   #include <sstream>
}


options {
   language="Cpp";  
}


class GptAssemblyLexer extends Lexer;


options {
   k=2;
   charVocabulary='\0'..'\377';
   exportVocab=GptAssemblyLexer;
  testLiterals = true;
//  testLiterals = false;
//  filter=T_INVALID;
//  genHashLines=false;//no #line
}


tokens {
   T_KW_PROGRAM="program";
   T_KW_ENDPROGRAM="endprogram";
   T_KW_USER="use";
   T_KW_VAR="var";
   T_KW_INT="int";
   T_KW_REAL="real";
   T_KW_CHAR="char";
   T_KW_STRING="string";
   T_KW_BOOL="bool";
   T_KW_POINTER="pointer";
   T_KW_MATRIX="matrix";
   T_KW_PROC="proc";
   T_KW_ENDPROC="endproc";
   T_KW_PARAM="param";
   T_KW_REF="ref";
   T_KW_ISUM="isum";
   T_KW_SSUM="ssum";
   T_KW_RSUM="rsum";
   T_KW_ISUB="isub";
   T_KW_SSUB="ssub";
   T_KW_RSUB="rsub";
   T_KW_IMUL="imul";
   T_KW_RMUL="rmul";
   T_KW_IDIV="idiv";
   T_KW_RDIV="rdiv";
   T_KW_IMOD="imod";
   T_KW_RMOD="rmod";
   T_KW_IGE="ige";
   T_KW_SGE="sge";
   T_KW_RGE="rge";
   T_KW_ILE="ile";
   T_KW_SLE="sle";
   T_KW_RLE="rle";
   T_KW_INE="ine";
   T_KW_SNE="sne";
   T_KW_RNE="rne";
   T_KW_IGT="igt";
   T_KW_SGT="sgt";
   T_KW_RGT="rgt";
   T_KW_ILT="ilt";
   T_KW_SLT="slt";
   T_KW_RLT="rlt";
   T_KW_IEQ="ieq";
   T_KW_SEQ="seq";
   T_KW_REQ="req";
   T_KW_OR="or";
   T_KW_AND="and";
   T_KW_XOR="xor";
   T_KW_INEG="ineg";
   T_KW_RNEG="rneg";
   T_KW_NOT="not";
   T_KW_IINC="iinc";
   T_KW_IDEC="idec";
   T_KW_I2C="i2c";
   T_KW_R2C="r2c";
   T_KW_S2C="s2c";
   T_KW_B2C="b2c";
   T_KW_I2R="i2r";
   T_KW_C2R="c2r";
   T_KW_S2R="s2r";
   T_KW_B2R="b2r";
   T_KW_I2B="i2b";
   T_KW_C2B="c2b";
   T_KW_R2B="r2b";
   T_KW_S2B="s2b";
   T_KW_I2S="i2s";
   T_KW_C2S="c2s";
   T_KW_R2S="r2s";
   T_KW_B2S="b2s";
   T_KW_P2S="p2s";
   T_KW_C2I="c2i";
   T_KW_R2I="r2i";
   T_KW_S2I="s2i";
   T_KW_B2I="b2i";
   T_KW_ISET="iset";
   T_KW_SSET="sset";
   T_KW_RSET="rset";
   T_KW_GETA="geta";
   T_KW_IGETV="igetv";
   T_KW_SGETV="sgetv";
   T_KW_RGETV="rgetv";
   T_KW_ISETV="isetv";
   T_KW_SSETV="ssetv";
   T_KW_RSETV="rsetv";
   T_KW_JMP="jmp";
   T_KW_IF="if";
   T_KW_IFNOT="ifnot";
   T_KW_POP="pop";
   T_KW_INCSP="incsp";
   T_KW_DECSP="decsp";
//   T_KW_PUSH_0="push_0";
//   T_KW_PUSH_1="push_1";
//   T_KW_PUSH_2="push_2";
//   T_KW_PUSH_3="push_3";
//   T_KW_PUSH_4="push_4";
//   T_KW_PUSH_5="push_5";
   T_KW_PUSH_INT="push_int";
   T_KW_PUSH_STRING="push_string";
   T_KW_PUSH_REAL="push_real";
   T_KW_PUSH_CHAR="push_char";
   T_KW_PUSH_BOOL="push_bool";
   T_KW_PUSH_MATRIX="push_matrix";
   T_KW_PUSH_ITYPE="push_itype";
   T_KW_PUSH_STYPE="push_stype";
   T_KW_PUSH_RTYPE="push_rtype";
   T_KW_PUSH_CTYPE="push_ctype";
   T_KW_PUSH_BTYPE="push_btype";
   T_KW_PUSH_MTYPE="push_mtype";
   T_KW_PUSH_SP="push_sreg";
   T_KW_POP_SP="pop_sreg";
   T_KW_INCSP_4="incsp_4";
   T_KW_INCSP_8="incsp_8";
   T_KW_DECSP_4="decsp_4";
   T_KW_DECSP_8="decsp_8";
   T_KW_PCALL="pcall";
   T_KW_LIBCALL="libcall";
   T_KW_RET="ret";
   T_KW_SALLOC="salloc";
   T_KW_SFREE="sfree";
   T_KW_SSETC="ssetc";
   T_KW_SGETC="sgetc";
   T_KW_M1ALLOC="m1alloc";
   T_KW_M2ALLOC="m2alloc";
   T_KW_MFREE="mfree";
   T_KW_M1SET="m1set";
   T_KW_M1GET="m1get";
   T_KW_M2SET="m2set";
   T_KW_M2GET="m2get";
   T_KW_MCOPY="mcopy";
   T_KW_MGETSIZE1="mgetsize1";
   T_KW_MGETSIZE2="mgetsize2";
   T_KW_NOP="nop";
//   T_KW_EXIT_0="exit_0";
//   T_KW_EXIT_1="exit_1";
   T_KW_HLT="hlt";
   T_KW_EXIT="exit";
   T_KW_TRUE="true";
   T_KW_FALSE="false";
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
   : '"'! ( ~( '"' | '\\' | '\n' | '\r') | ESC)* '"'!
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

//T_DOT
//options {
//  paraphrase = "'.'";
//}
//  : '.'
//  ;

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

