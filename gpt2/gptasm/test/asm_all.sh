echo "Assemblando todos os arquivos .gasm do diretorio"

rm *.gvm > /dev/null 2>&1
#rm *.stdout

for i in `ls *.gasm -1 | sed s/"\.gasm"//g`
do
	./asm.sh $i 
	echo ""
done
