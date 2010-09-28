echo "Realizando o teste regressivo em todos os arquivos .gasm do diretorio"

rm *.gvm 1>/dev/null 2>&1
rm *.stdout 1>/dev/null 2>&1

for i in `ls *.gasm -1 | sed s/"\.gasm"//g`
do
	./check.sh $i 
	echo ""
#	if [ $? -ne 0 ]
#	then
#		echo "Falha no teste regressivo do arquivo $i";
#	fi
done
