echo "Realizando o teste regressivo em todos os arquivos .gvm do diretorio"

rm *.stdout 1>/dev/null 2>&1

for i in `ls *.gvm -1 | sed s/"\.gvm"//g`
do
	./check.sh $i 
	echo ""
#	if [ $? -ne 0 ]
#	then
#		echo "Falha no teste regressivo do arquivo $i";
#	fi
done
