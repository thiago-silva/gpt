#ifndef OPTIONS
#define OPTIONS

#include <string>
using namespace std;

class Options {
public:
	Options( ) {
		sentences = false;
		sourceLine = NULL;
	}
	string filename;
//	string sourcefile;
//	string destfile;
	bool sentences;
	int *sourceLine;
};

#endif

