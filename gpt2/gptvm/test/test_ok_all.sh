echo "Executando test ok para todos os arquivos .gvm do diretorio"

for i in `ls *.gvm -1 | sed s/"\.gvm"//g`
do
	./test_ok.sh $i 
	echo ""
done
