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

echo "Realizando o teste regressivo no arquivo $1.gvm"

exist_file $1.gvm
if [ $? -ne 0 ]
then
	exit 2
fi

exist_file checked/$1.gvm
if [ $? -ne 0 ]
then
	exit 2
fi

exist_file checked/$1.stdout
if [ $? -ne 0 ]
then
	exit 2
fi

diff $1.gvm checked/$1.gvm >/dev/null 2>&1
if [ $? -ne 0 ]
then
	echo "Erro: Os arquivos $1.gvm e checked/$1.gvm sao diferentes. Nao eh possivel realizar o teste regressivo !"
	exit 2
fi

./run.sh $1
if [ $? -ne 0 ]
then
	echo "Erro: Arquivo $1.gvm nao foi executado corretamente"
	exit 2
fi

diff $1.stdout checked/$1.stdout >/dev/null 2>&1
if [ $? -ne 0 ]
then
	echo "Erro: Teste regressivo falhou. Os arquivos $1.stdout e checked/$1.stdout sao diferentes !!!"
	vimdiff $1.stdout checked/$1.stdout
	exit 1
fi

echo "Ok: Teste regressivo ok para $1.gvm"

exit 0

