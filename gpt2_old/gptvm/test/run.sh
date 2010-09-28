rm $1.stdout 1>/dev/null 2>&1

../../../_build/gptvm/src/gptvm $1 > $1.stdout
if [ $? -ne 0 ]
then
	echo "Erro: nao foi possivel executar $1.gvm"
	exit 1
fi
echo ""

cat $1.stdout

