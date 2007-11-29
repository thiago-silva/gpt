header {
   #include <string>
   #include "CGenBytecode.hpp"
   #include <antlr/config.hpp>
   #include <antlr/TokenStream.hpp>
   #include <antlr/TokenBuffer.hpp>
   #include "GptAssemblyParserTokenTypes.hpp"
   #include <antlr/LLkParser.hpp>
   #include "CSymbolList.hpp"
}

options {
  language="Cpp";  
}

{
}

class GptAssemblyParser extends Parser;

options {
  importVocab=GptAssemblyLexer;  // use vocab generated by lexer
//  genHashLines=false;//no #line
  k=2;
}

{  
   public:
      CGenBytecode bytecode;
      antlr::RefToken getLastToken()
      {
         return LT(0);
      }
      std::string getLastTokenText()
      {
         return LT(0)->getText();
      }
}


//####################################################################
//####################### Início da gramática ########################
//####################################################################

//##############
//## Programa ##
//##############

//--------------------------------------------------------
  program [std::string filename] returns [std::string ret]
//--------------------------------------------------------
   : "program" T_ID
     (imports)*
     (var_declaration)*
     (procedure_declaration)+
     "endprogram"
     EOF
     {
        bytecode.translateLabelsToAddress();
        ret = bytecode.getBinary();
     }
     ;

//-------
  imports
//-------
   : "use" T_STRING_VALUE
   ;

//#############################
//## Declaração de variáveis ##
//#############################

//---------------
  var_declaration
//---------------
{
  char tk_type;
}
  : "var" tk_id:T_ID tk_type=primitive_type
    { bytecode.makeVarDefinition(tk_id->getText(), tk_type); }
  ;

//--------------------------------
  primitive_type returns [char ret]
//--------------------------------
  : "int"     {ret=CSymbol::INT;    }
  | "real"    {ret=CSymbol::REAL;   }
  | "char"    {ret=CSymbol::CHAR;   }
  | "string"  {ret=CSymbol::STRING; }
  | "bool"    {ret=CSymbol::BOOL;   }
  | "pointer" {ret=CSymbol::POINTER;}
  | "matrix"  {ret=CSymbol::MATRIX; }
  ;

//#################################
//## Declaracao de procedimentos ##
//#################################

//---------------------
  procedure_declaration
//---------------------
  : "proc" 
   tk_id:T_ID
    { bytecode.initProcedure(tk_id->getText(), false, 0, std::vector<CSymbol>()); }
    (parameter_declaration)*
    { bytecode.finishParDefinition(); }
    (var_declaration)*
    code_block
    { bytecode.finishProcedure(); }
    "endproc"
  ;

//---------------------
  parameter_declaration
//---------------------
{
  int tk_type;
}
  : "param" ("ref")? tk_id:T_ID tk_type=primitive_type
    { bytecode.makeParDefinition(tk_id->getText(), tk_type); }
  ;

//#####################
//## Bloco de código ##
//#####################

//----------
  code_block
//----------
  : (statement)*
  ;

//---------
  statement
//---------
  :
  ( label_definition
  | mn_atribuicao_binaria
  | mn_atribuicao_unaria
  | mn_conversao_dados
  | mn_copia_simples
  | mn_address_and_pointer
  | mn_salto
  | mn_chamada_subrotina
  | mn_manipulacao_strings
  | mn_manipulacao_matrizes
  | mn_execucao
  )
  ;

//----------------
  label_definition
//----------------
  : tk_id:T_ID T_COLON { bytecode.registryLabel(tk_id->getText()); }
  ;

//-----
  label
//-----
   : T_ID { bytecode.unsolvedLabel(getLastTokenText()); }
   ;

//---------------------
  mn_atribuicao_binaria
//---------------------
   :  ( "isum"|"ssum"|"rsum"
      | "isub"|"ssub"|"rsub"
      | "imul"|"rmul"|"idiv"|"rdiv"
      | "imod"|"rmod"
      | "ige" |"sge" |"rge" |"ile"|"sle"|"rle"
      | "ine" |"sne" |"rne"
      | "igt" |"sgt" |"rgt"
      | "ilt" |"slt" |"rlt"
      | "ieq" |"seq" |"req"
      | "or"  |"and" |"xor"
      )
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element T_COMMA element
   ;

//--------------------
  mn_atribuicao_unaria
//--------------------
   :  ("ineg"|"rneg"|"not"|"iinc"|"idec") 
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element
   ;

//------------------
  mn_conversao_dados
//------------------
   :  ( "i2c"|"r2c"|"s2c"|"b2c"
      | "i2r"|"c2r"|"s2r"|"b2r"
      | "i2b"|"c2b"|"r2b"|"s2b"
      | "i2s"|"c2s"|"r2s"|"b2s"|"p2s"
      | "c2i"|"r2i"|"s2i"|"b2i"
      )
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element
   ;

//----------------
  mn_copia_simples
//----------------
   :  ( "iset"|"sset"|"rset" )
      {bytecode.addOpcode(getLastTokenText());}
      identifier
      T_COMMA
      element
   ;

//----------------------
  mn_address_and_pointer
//----------------------
   :  "geta"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier
   |  ("igetv"|"sgetv"|"rgetv")
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element T_COLON element
   |  ("isetv"|"ssetv"|"rsetv")
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COLON element T_COMMA element
   ;

//--------
  mn_salto
//--------
   :  "jmp"
      {bytecode.addOpcode(getLastTokenText());}
      label
   |  ("if"|"ifnot")
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA label
   ;

//--------------------
  mn_chamada_subrotina
//--------------------
   :  ("pushiv"|"pushsv"|"pushrv"|"pushmv")
      {bytecode.addOpcode(getLastTokenText());}
      element
   |  ("pushit"|"pushst"|"pushrt"|"pushct"|"pushbt"|"pushmt")
      {bytecode.addOpcode(getLastTokenText());}
//   |  "pop"
   |  ("popiv"|"popsv"|"poprv"|"popmv")
      {bytecode.addOpcode(getLastTokenText());}
      identifier
   |  ("incsp"|"decsp")
      {bytecode.addOpcode(getLastTokenText());}
      T_INT_VALUE
      { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
   |  ("push_0"|"push_1"|"push_2"|"push_3"|"push_4"|"push_5")
      {bytecode.addOpcode(getLastTokenText());}
   |  ("incsp_4"|"incsp_8"|"decsp_4"|"decsp_8")
      {bytecode.addOpcode(getLastTokenText());}
   |  "pcall"
      {bytecode.addOpcode(getLastTokenText());}
      T_ID
      { bytecode.unsolvedLabel(getLastTokenText());}
   |  "lcall"
      {bytecode.addOpcode(getLastTokenText());}
      T_ID
      { bytecode.addAddress(getLastTokenText(),CSymbol::CONST, CSymbol::STRING); }
   |  "ret"
      {bytecode.addOpcode(getLastTokenText());}
   ;

//----------------------
  mn_manipulacao_strings
//----------------------
   :  ( "salloc"|"sfree" )
      {bytecode.addOpcode(getLastTokenText());}
      identifier
   |  "ssetc"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element T_COMMA element
   |  "sgetc"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier T_COMMA element
   ;

//-----------------------
  mn_manipulacao_matrizes
//-----------------------
   :  "m1alloc"
      {bytecode.addOpcode(getLastTokenText());}
      identifier
      T_COMMA
      T_INT_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
      T_COMMA 
      T_INT_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
   |  "m2alloc"
      {bytecode.addOpcode(getLastTokenText());}
      identifier
      T_COMMA
      T_INT_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
      T_COMMA
      T_INT_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
      T_COMMA
      T_INT_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
   |  "mfree"
      {bytecode.addOpcode(getLastTokenText());}
      identifier
   |  "m1set"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element T_COMMA element
   |  "m1get"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier T_COMMA element
   |  "m2set"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA element T_COMMA element T_COMMA element
   |  "m2get"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier T_COMMA element T_COMMA element
   |  "mcopy"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier
   |  "mgetsize1"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier
   |  "mgetsize2"
      {bytecode.addOpcode(getLastTokenText());}
      identifier T_COMMA identifier
   ;

//-----------
  mn_execucao
//-----------
   :   ("nop"|"hlt"|"exit_0"|"exit_1")
      {bytecode.addOpcode(getLastTokenText());}
   |   "exit"
      {bytecode.addOpcode(getLastTokenText());}
       T_INT_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT); }
  ;

identifier
  : id:T_ID { bytecode.addAddress(id->getText(), CSymbol::VAR, CSymbol::STRING); }
  ;


//#############
//## Literal ##
//#############

//---------------------------------
  literal //returns [std::string ret]
//---------------------------------
  : ( 
        T_STRING_VALUE { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::STRING); }
      | T_INT_VALUE    { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::INT);    }
      | T_REAL_VALUE   { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::REAL);   }
      | T_CHAR_VALUE   { bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::CHAR);   }
      | "true"         { bytecode.addAddress("1",                CSymbol::CONST, CSymbol::INT);    }
      | "false"        { bytecode.addAddress("0",                CSymbol::CONST, CSymbol::INT);    }
    ) 
  ;

//op_unario
//  : (
//        e:T_MENOS
//      | a:T_MAIS
//    )?
//  ; 

//-------
  element
//-------
  : identifier //{ bytecode.addAddress(getLastTokenText(), CSymbol::CONST, CSymbol::STRING); }
  | literal
  ;

