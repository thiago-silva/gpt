echo "Executando test ok para todos os arquivos .gasm do diretorio"

for i in `ls *.gasm -1 | sed s/"\.gasm"//g`
do
	./test_ok.sh $i 
	echo ""
done
