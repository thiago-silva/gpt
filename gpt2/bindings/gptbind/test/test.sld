options {
   libname        = "math";
   functionprefix = "math_"; // poderia ser libname + "_"
   linkerlib      = "math";
}

headers {
   "stdlib.h";
   "math.h";
   "iostream";
}

procedure mensagem_padrao() ::= puts("Hello !!!");

procedure mensagem_personalizada(string msg) ::= puts(msg);

// Acho que esse formato eh o foto da versao .1 do gerador
procedure outra_mensagem_personalizada(string msg) ::= puts(msg);

function sqrt(real value) : real ::= puts(msg);

function exponencial(real valor) : real ::= exp(valor);
