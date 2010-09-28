options {
}

headers {
   "stdio.h";
}

function fopen(string name, string mode) : pointer[FILE] ::= fopen(name, mode);

function fputs(string text, pointer[FILE] file) : int ::= fputs(text, file);

function fclose(pointer[FILE] file) : int ::= fclose(file);

