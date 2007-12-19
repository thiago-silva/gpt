options {
   linkerlib = "math";
}

headers {
   "stdlib.h";
   "math.h";
   "iostream";
}

procedure mensagem_padrao() ::= puts("Hello !!!");

procedure mensagem_personalizada(string msg) ::= puts(msg);

function sqrt(real value) : real ::= sqrt(value);

function exponencial(real valor) : real ::= exp(valor);
