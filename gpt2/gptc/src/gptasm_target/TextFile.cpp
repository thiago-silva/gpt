#include "TextFile.hpp"


TextFile::TextFile( const string &chrTab )
	: _chrTab( chrTab ), _emitTab( true )
{
}


TextFile::~TextFile( )
{
}


void TextFile::writeln( const string &message )
{
	if ( _emitTab ) {
		_txt << _tabs;
	}
	_txt << message << endl;
	_emitTab = true;
}


void TextFile::writeln( )
{
	_txt << endl;
	_emitTab = true;
}


void TextFile::write( const string &message )
{
	if ( _emitTab ) {
		_txt << _tabs;
	}
	_txt << message;
	_emitTab = false;
}


void TextFile::incTab( )
{
    _tabs += _chrTab;
}


void TextFile::decTab( )
{
    _tabs.erase( _tabs.length( ) - _chrTab.length( ), _chrTab.length( ) );
}


string TextFile::getText( ) const
{
	return _txt.str( );
}


bool TextFile::writeToFile( const string &filename )
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

