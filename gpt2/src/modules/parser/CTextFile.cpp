#include "CTextFile.hpp"


CTextFile::CTextFile( const string &filename, const string &chrTab )
	: _chrTab( chrTab ), _emitTab( true )
{
	_file = new ofstream( filename.c_str( ) );
}


CTextFile::~CTextFile( )
{
	delete _file;
}


void CTextFile::writeln( const string &message )
{
	if ( _emitTab ) {
		(*_file) << _tabs;
	}
	(*_file) << message << endl;
	_emitTab = true;
}


void CTextFile::writeln( )
{
	(*_file) << endl;
	_emitTab = true;
}


void CTextFile::write( const string &message )
{
	if ( _emitTab ) {
		(*_file) << _tabs;
	}
	(*_file) << message;
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


CTextFile& CTextFile::operator << ( const string &message )
{
    write(message);
	
	return *this;
}


/*
ostream& operator << (ostream& os, const ConstantPool& constantPool) {
    os << "Constant pool size: " << constantPool.size( ) << endl;

    for ( ConstantPool::const_iterator constant = constantPool.begin( );
          constant != constantPool.end( ); constant++ ) {
        // number
        os << "\t" << strZero(constant - constantPool.begin( ), 3) << ":"
        << *constant;
    }

    return ( os );
}
*/

