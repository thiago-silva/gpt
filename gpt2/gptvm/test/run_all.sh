echo "Executando todos os arquivos .gvm do diretorio"

rm *.stdout

for i in `ls *.gvm -1 | sed s/"\.gvm"//g`
do
	./run.sh $i 
	echo ""
done
