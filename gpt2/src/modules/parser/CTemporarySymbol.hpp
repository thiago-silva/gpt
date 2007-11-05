#ifndef GPT_CTEMPORARY_SYMBOL_H
#define GPT_CTEMPORARY_SYMBOL_H

#include <string>

using namespace std;

class CTemporarySymbol
{
public:
	CTemporarySymbol( );
	CTemporarySymbol( const string &baseName, const int &nextValue = 0 );
	~CTemporarySymbol( );
	void setBaseName( string baseName );
	void setNextValue( int initialValue );
	string getNew( );
	void removeLast( );
	unsigned int getMaxUsed( );
	string getLast( );
private:
	string _baseName;
	int _count;
	string _last;
};

#endif
