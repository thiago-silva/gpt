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

echo "Realizando o teste regressivo no arquivo $1.gpt"

exist_file $1.gpt
if [ $? -ne 0 ]
then
	exit 2
fi

exist_file checked/$1.gpt
if [ $? -ne 0 ]
then
	exit 2
fi

exist_file checked/$1.gasm
if [ $? -ne 0 ]
then
	exit 2
fi

#exist_file checked/$1.stdout
#if [ $? -ne 0 ]
#then
#	exit 2
#fi

diff $1.gpt checked/$1.gpt >/dev/null 2>&1
if [ $? -ne 0 ]
then
	echo "Erro: Os arquivos $1.gpt e checked/$1.gpt sao diferentes. Nao eh possivel realizar o teste regressivo !"
	exit 2
fi

./comp.sh $1
if [ $? -ne 0 ]
then
	echo "Erro: Arquivo $1.gpt nao foi compilado corretamente"
	exit 2
fi

diff $1.gasm checked/$1.gasm >/dev/null 2>&1
if [ $? -ne 0 ]
then
	echo "Erro: Teste regressivo falhou. Os arquivos $1.gasm e checked/$1.gasm sao diferentes !!!"
	vimdiff $1.gasm checked/$1.gasm
	exit 1
fi

#diff $1.gvm checked/$1.gvm >/dev/null 2>&1
#if [ $? -ne 0 ]
#then
#	echo "Erro: Teste regressivo falhou. Os arquivos $1.gvm e checked/$1.gvm sao diferentes !!!"
#	gdumpasm -D$1
#	gdumpasm -Dchecked/$1
#	vimdiff $1.dump checked/$1.dump
#	exit 1
#fi

#diff $1.stdout checked/$1.stdout >/dev/null 2>&1
#if [ $? -ne 0 ]
#then
#	echo "Erro: Teste regressivo falhou. Os arquivos $1.stdout e checked/$1.stdout sao diferentes !!!"
#	vimdiff $1.stdout checked/$1.stdout
#	exit 1
#fi

echo "Ok: Teste regressivo ok para $1.gpt"

exit 0

