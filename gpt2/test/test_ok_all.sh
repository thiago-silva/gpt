echo "Executando test ok para todos os arquivos .gpt do diretorio"

for i in `ls *.gpt -1 | sed s/"\.gpt"//g`
do
	./test_ok.sh $i 
	echo ""
done
