#ifndef GASMFE_OPTIONS
#define GASMFE_OPTIONS

#include <string>
using namespace std;

class COptions {
public:
	COptions( ) {
		sentences = false;
		sourceLine = NULL;
	}
	string sourcefile;
	string destfile;
	bool sentences;
	int *sourceLine;
};

#endif

