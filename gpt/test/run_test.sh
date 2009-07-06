#! /bin/bash

echo "
Testando a compilação direta"
../src/gpt -o tester tester.gpt
./tester
echo "echo \$?=$?"

echo "
Testando a interpretação"
../src/gpt -i tester.gpt
echo "echo \$?=$?"

echo "
Testando a tradução para linguagem C"
../src/gpt -t tester.c tester.gpt
gcc -o tester.c.exe tester.c
./tester.c.exe
echo "echo \$?=$?"

echo "
Testando a tradução para linguagem Assembly"
../src/gpt -s tester.asm tester.gpt
nasm -O1 -fbin -o tester.asm.exe tester.asm
chmod +x tester.asm.exe
./tester.asm.exe
echo "echo \$?=$?"
