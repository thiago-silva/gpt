rm $1.gasm 1>/dev/null 2>&1
#rm $1.gvm 1>/dev/null 2>&1
#rm $1.stdout 1>/dev/null 2>&1

../../src/gpt $1.gpt
if [ $? -ne 0 ]
then
	echo "Erro: nao foi possivel compilar $1.gpt"
	exit 1
fi
#echo ""

#./gptasm $1
#if [ $? -ne 0 ]
#then
#	echo "Erro: nao foi possivel assemblar $1.gasm"
#	exit 1
#fi
#echo ""

#./gvm $1 > $1.stdout
#if [ $? -ne 0 ]
#then
#	echo "Erro: nao foi possivel executar $1.gvm"
#	exit 1
#fi
#echo ""

#cat $1.stdout

#cat $1.gasm

