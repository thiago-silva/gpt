#include "CTextFile.hpp"


CTextFile::CTextFile( const string &chrTab )
	: _chrTab( chrTab ), _emitTab( true )
{
}


CTextFile::~CTextFile( )
{
}


void CTextFile::writeln( const string &message )
{
	if ( _emitTab ) {
		_txt << _tabs;
	}
	_txt << message << endl;
	_emitTab = true;
}


void CTextFile::writeln( )
{
	_txt << endl;
	_emitTab = true;
}


void CTextFile::write( const string &message )
{
	if ( _emitTab ) {
		_txt << _tabs;
	}
	_txt << message;
	_emitTab = false;
}


void CTextFile::incTab( )
{
    _tabs += _chrTab;
}


void CTextFile::decTab( )
{
    _tabs.erase( _tabs.length( ) - _chrTab.length( ), _chrTab.length( ) );
}


string CTextFile::getText( ) const
{
	return _txt.str( );
}


bool CTextFile::writeToFile( const string &filename )
{
	ofstream *_file;
	_file = new ofstream( filename.c_str( ) );
	if (_file) {
		(*_file) << _txt;
		delete _file;
		return true;
	}
	return false;
}

