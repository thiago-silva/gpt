function exist_file( )
{
	if [ -z "$1" ]                    # Checks if parameter #1 is zero length.
	then
		echo "-Parameter #1 is zero length."
		return 1
	fi

	if [ "$2" ] # verifica se o parametro existe
	then
		echo "-Parameter #2 is \"$2\".-"
	fi
					  
	if [[ ! -a $1 ]]
	then
		echo "Erro: Arquivo $1 nao existe !!!"
		return 1
	fi

	return 0
}


exist_file $1.gasm
if [ $? -ne 0 ]
then
	exit 2
fi

exist_file $1.gvm
if [ $? -ne 0 ]
then
	exit 2
fi

#exist_file $1.stdout
#if [ $? -ne 0 ]
#then
#	exit 2
#fi

echo "Copiando o arquivo $1.gasm p/ o diretorio checked"
cp $1.gasm checked/

echo "Copiando o arquivo $1.gvm p/ o diretorio checked"
cp $1.gvm  checked/

#echo "Copiando o arquivo $1.stdout p/ o diretorio checked"
#cp $1.stdout  checked/

