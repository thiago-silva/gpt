#ifndef GPT_CTEMPORARY_SYMBOL_H
#define GPT_CTEMPORARY_SYMBOL_H

#include <string>

using namespace std;

class CTemporarySymbol
{
public:
	CTemporarySymbol( );
	CTemporarySymbol( const string &baseName, const int &initialValue = 0 );
	~CTemporarySymbol( );
	void setBaseName( string baseName );
	void setInitialValue( int initialValue );
	string getNew( );
	void removeLast( );
	unsigned int getMaxUsed( );
	string getLast( );
private:
	string _baseName;
	int _count;
};

#endif