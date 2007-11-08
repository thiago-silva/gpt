/*
 * $Id: Tools.cpp,v 1.1.1.1 2005/08/15 15:19:52 asgarzao Exp $
 * MyLibrary version 0.1.0
 * Copyright (C) 2002 Alex Sandro Garzão <alexgarzao@bol.com.br>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "PortugolParserTokenTypes.hpp"

#include "Tools.hpp"


int hexToInt(const char* _value) {
    int result = 0;
    char *position;
    char conversionTable[] = "0123456789ABCDEF";

    while (*_value) {
        position = strchr(conversionTable, *_value);
        if (position == NULL)
            return 0;

        result <<= 4;
        result = result | (position - conversionTable);
        _value++;
    }

    return result;
}

string pad(string _text, unsigned int _size) {
    if (_size > _text.length())
        _text.append(_size - _text.length(), ' ');

    return _text;
}

string lpad(string _text, unsigned int _size) {
    if (_size > _text.length())
        _text = string(_size - _text.length(), ' ') + _text;

    return _text;
}

string intToHex(unsigned int _value, int _size) {
    string codHex = "0123456789ABCDEF";
    string result;
    int nible;

    while (_value > 0) {
        nible = _value & 0xF;
        _value >>= 4;
        result = codHex[nible] + result;
    }

    if ( _size == -1 )
        _size = 1;

    return strZero(result, _size);
    // if (_size == -1)
    //     return( result );
    // else
    //     return( strZero( result, _size ) );
    //
}

string strZero(string _value, unsigned int _size) {
    while (_value.length() < _size) {
        _value = '0' + _value;
    }

    return _value;
}

string strZero(int _value, unsigned int _size) {
    string result = itoa(_value);

    while (result.length() < _size) {
        result = '0' + result;
    }

    return result;
}

string dataToHex(char _code[], unsigned int _length) {
    string result;
    unsigned int count;

    for (count = 0; count < _length; count++)
        result += intToHex(_code[count], 2);

    return result;
}

string dataToHex(char _code[], unsigned int _start, unsigned int _length) {
    string result;
    unsigned int count;

    for (count = _start; count - _start < _length; count++)
        result += intToHex(_code[count], 2);

    return result;
}

int binToInt(const char* _binValue) {
    int intValue = 0;
    int length = strlen(_binValue) - 1;
    int countBit;

    for (countBit = length; countBit >= 0; countBit--) {
        if (_binValue[countBit] == '1')
            intValue += int(pow(2.0, length - countBit));
    }

    return intValue;
}

string itoa(int _number) {
    stringstream s;

    s << _number;
    return s.str();
}

string ftos(double _value) {
    stringstream s;

    s <<  setiosflags(ios::fixed) << _value;
    
    return s.str();
}

string itobool(int _number) {
    if (_number == 0)
        return "false";
    else
        return "true";
}

string strtoupper(string str) {
    unsigned int count;

    for (count = 0; count < str.size(); count++)
        str[count] = toupper(str[count]);

    return str;
}

string strToUpperWithEscapeControls(string str) {
    unsigned int count;

    for (count = 0; count < str.size(); count++) {
        if (str[count] == '\\')
            count += 2;
        else
            str[count] = toupper(str[count]);
    }

    return str;
}

string alltrim(string str) {
    // crap version...  :-)
    // precisamos usar `find', `rfind'! --felipek
    while (str[0] == ' ')
        str.erase(0, 1);

    while (str[str.length() - 1] == ' ')
        str.erase(str.length() - 1, 1);

    return str;
}

string wo_ctrl_chrs(string str, char chr_to_put) {
    unsigned int chr_number;

    for (chr_number = 0; chr_number < str.size(); chr_number++) {
        if (str[chr_number] < 32)
            str[chr_number] = chr_to_put;
    }

    return str;
}

/* CRAP!  --felipek
string& getStringWithTab(string _data, string _tab) {
	cout << "begin getStringWithTab" << endl;
	cout << "tab=\"" << _tab << "\"" << endl;
	cout << "begin datae" << endl;
	cout << _data << endl;
	cout << "end datae" << endl;
	int pos = -1;
 
	// test too another new line sequences
	while( ( pos = _data.find( "\n", pos + 1 ) ) != -1 ) {
		_data.insert( pos + 1, _tab );
	}
 
	cout << "begin datas" << endl;
	cout << _data << endl;
	cout << "end datas" << endl;
	return( _data );
}
*/

bool streamtoken(istringstream& stream, string& key, string& val) {
     /* FIXME: leak, --felipek */
     char buffer[8192];
     unsigned int delim;
     string entry;
//     bool ret;

     /* FIXME: ret type, -- felipek */
     stream.getline(buffer, 8192 - 1);
     entry = string(buffer);
     delim = entry.find(' ', 0);

     if (delim != entry.npos) {
         key = entry.substr(0, delim);
         val = entry.substr(delim + 1);
     }

     return !stream.eof();
}


const string typeToText(const int &type)
{
   if (type == PortugolParserTokenTypes::T_INT_LIT) {
      return "int";
   } else if (type == PortugolParserTokenTypes::T_CARAC_LIT) {
      return "char";
   } else if (type == PortugolParserTokenTypes::T_STRING_LIT) {
      return "string";
   } else if (type == PortugolParserTokenTypes::T_REAL_LIT) {
      return "real";
   } else if (type == PortugolParserTokenTypes::T_IDENTIFICADOR) {
      return "id";
   }

   return "ERRO !!!";
}

const string typeInAsm( const int &type )
{
   if (type == PortugolParserTokenTypes::T_KW_INTEIRO) {
      return "int";
   } else if (type == PortugolParserTokenTypes::T_KW_LITERAL) {
      return "string";
   } else if (type == PortugolParserTokenTypes::T_KW_CARACTERE) {
      return "char";
   } else if (type == PortugolParserTokenTypes::T_KW_LOGICO) {
      return "bool";
   } else if (type == PortugolParserTokenTypes::T_KW_REAL) {
      return "real";
   } else if (type == PortugolParserTokenTypes::T_KW_CORINGA) {
      return "pointer ???";
   } else if (type == PortugolParserTokenTypes::T_KW_MATRIZ) {
      return "matrix";
   }

   return "ERRO !!!";
}


const int typeToLiteral( const int &type )
{
   switch (type) {
      case PortugolParserTokenTypes::T_KW_INTEIRO:
         return PortugolParserTokenTypes::T_INT_LIT;
      case PortugolParserTokenTypes::T_KW_LITERAL:
         return PortugolParserTokenTypes::T_STRING_LIT;
      case PortugolParserTokenTypes::T_KW_CARACTERE:
         return PortugolParserTokenTypes::T_CARAC_LIT;
      case PortugolParserTokenTypes::T_KW_LOGICO:
         return PortugolParserTokenTypes::T_INT_LIT;
//      case PortugolParserTokenTypes::T_KW_CORINGA:
//         return "pointer ???";
//      case PortugolParserTokenTypes::T_KW_MATRIZ:
//          return PortugolParserTokenTypes::T_MATRIZ;
      default:
          return 0;
          // trow exception ???
   }
}

