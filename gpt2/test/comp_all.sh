echo "Compilando todos os arquivos .gpt do diretorio"

rm *.gasm
#rm *.gvm
#rm *.stdout

for i in `ls *.gpt -1 | sed s/"\.gpt"//g`
do
	./comp.sh $i 
	echo ""
done
