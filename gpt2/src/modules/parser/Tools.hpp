#ifndef GPT_TOOLS_H
#define GPT_TOOLS_H

#include <iomanip>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <sstream>
using namespace std;


int hexToInt(const char *);
string pad(string, unsigned int);
string lpad(string, unsigned int);
string intToHex(unsigned int, int = -1);
string strZero(string, unsigned int);
string strZero(int, unsigned int);
string dataToHex(char *, unsigned int);
string dataToHex( char *, unsigned int, unsigned int);
int binToInt(const char *);
int octToInt(const char *);
string itoa(int);
string itobool(int);
string ftos(double);
string strtoupper(string);
string strToUpperWithEscapeControls(string);
string alltrim(string);
string wo_ctrl_chrs(string str, char = ' ');
bool streamtoken(istringstream&, string&, string&);
// string& getStringWithTab(string, string);
const string typeToText(const int &type);
const string typeInAsm( const int &type );

#endif
