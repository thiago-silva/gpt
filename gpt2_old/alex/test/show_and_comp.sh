echo "ARQUIVO $1.gpt"
cat $1.gpt

./comp.sh $1
if [ $? -ne 0 ]
then
	exit 1
fi

echo ""

echo "ARQUIVO $1.gasm"
cat $1.gasm

#echo "ARQUIVO $1.stdout"
#cat $1.stdout

