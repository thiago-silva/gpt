#/bin/sh
#sed  's/\\341/Ã¡/g; s/\\351/Ã©/g; s/\\355/Ã­/g; s/\\363/Ã³/g; s/\\372/Ãº/g; s/\\343/Ã£/g; s/\\347/Ã§/g' PortugolParser.cpp
sed  -i 's/"fim-var.*veis/"fim-variáveis/;
      s/"vari.*veis/"variáveis/;
      s/"l.*gico/"lógico/;
      s/"in.*cio/"início/;
      s/"n.*o\\"/"não\\"/;
      s/"sen.*o\\"/"senão\\"/;
      s/"ent.*o\\"/"então\\"/;
      s/"fa.*a\\"/"faça\\"/;
      s/"at.*\\"/"até\\"/;
      s/"l.*gicos/"lógicos/;
      s/"fun.*o\\"/"função\\"/;
      s/"n.*mero/"número/;
' PortugolParser.cpp


# 	"\"vari\303\241veis\"",
# 	"\"l\303\263gico\"",
# 	"\"in\303\255cio\"",
# 	"\"n\303\243o\"",
# 	"\"sen\303\243o\"",
# 	"\"ent\303\243o\"",
# 	"\"fa\303\247a\"",
# 	"\"at\303\251\"",
# 	"\"l\303\263gicos\"",
# 	"\"fun\303\247\303\243o\"",
# 	"\"n\303\272mero real\"",
# 	"n\303\272mero inteiro",
# 
