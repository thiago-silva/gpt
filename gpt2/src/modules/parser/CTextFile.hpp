#ifndef GPT_CTEXT_FILE_H
#define GPT_CTEXT_FILE_H

#include <string>
#include <fstream>
using namespace std;

class CTextFile
{
public:
	CTextFile( const string &filename, const string &chrTab = "\t" );
	~CTextFile( );
	void writeln( const string &message );
	void writeln( );
	void write( const string &message );
	void incTab( );
	void decTab( );
	CTextFile& operator << ( const string &message );
private:
	string _chrTab;
	string _tabs;
	ofstream *_file;
	bool _emitTab;
};

#endif

