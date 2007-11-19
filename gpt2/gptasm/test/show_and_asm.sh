echo "ARQUIVO $1.gasm"
cat $1.gasm

./asm.sh $1
if [ $? -ne 0 ]
then
	exit 1
fi

echo ""

echo "ARQUIVO $1.gvm"
cat $1.gvm

#echo "ARQUIVO $1.stdout"
#cat $1.stdout

