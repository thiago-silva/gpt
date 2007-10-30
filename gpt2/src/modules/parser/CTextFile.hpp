#ifndef CTEXT_FILE_H
#define CTEXT_FILE_H

#include <string>
#include <fstream>
using namespace std;

class CTextFile
{
public:
	/*inline*/ CTextFile( const string &filename, const string &chrTab = "\t" );
	/*inline*/ ~CTextFile( );
	void writeln( const string &message );
	void writeln( );
	void write( const string &message );
	/*inline*/
	void incTab( );
	/*inline*/ void decTab( );
	CTextFile& operator << ( const string &message );
private:
	string _chrTab;
	string _tabs;
	ofstream *_file;
	bool _emitTab;
};

#endif

