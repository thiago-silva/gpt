#ifndef GPT_CTEXT_FILE_H
#define GPT_CTEXT_FILE_H

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class CTextFile
{
public:
	CTextFile( const string &chrTab = "\t" );
	~CTextFile( );
	void writeln( const string &message );
	void writeln( );
	void write( const string &message );
	void incTab( );
	void decTab( );
	string getText( ) const;
	bool writeToFile( const string &filename );
private:
	string _chrTab;
	string _tabs;
	bool _emitTab;
	stringstream _txt;
};

#endif

