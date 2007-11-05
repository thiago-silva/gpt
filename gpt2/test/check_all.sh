echo "Realizando o teste regressivo em todos os arquivos .gpt do diretorio"

rm *.gasm 1>/dev/null 2>&1
#rm *.gvm 1>/dev/null 2>&1
#rm *.stdout 1>/dev/null 2>&1

for i in `ls *.gpt -1 | sed s/"\.gpt"//g`
do
	./check.sh $i 
	echo ""
#	if [ $? -ne 0 ]
#	then
#		echo "Falha no teste regressivo do arquivo $i";
#	fi
done
