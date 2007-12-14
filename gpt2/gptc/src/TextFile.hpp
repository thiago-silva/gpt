#ifndef GPT_TEXT_FILE_H
#define GPT_TEXT_FILE_H

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class TextFile
{
public:
	TextFile( const string &chrTab = "\t" );
	~TextFile( );
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

