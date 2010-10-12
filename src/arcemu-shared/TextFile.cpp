/*
 * Copyright (c) 2001 Jani Kajala
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Jani Kajala makes no representations about the suitability 
 * of this software for any purpose. It is provided "as is" 
 * without express or implied warranty.
 */
 
#include "TextFile.h"
#include <stdio.h>
#include <ctype.h>

class TextFile::TextFileImpl
{
public:
	TextFile::ErrorType		err;
	int						line;

	explicit TextFileImpl( const char* filename )
	{
		err				= TextFile::ERROR_NONE;
		line			= 1;
		m_peeked		= false;
		m_peekedChar	= 0;
		m_file			= fopen( filename, "rt" );

		if ( !m_file )
			err = TextFile::ERROR_OPEN;
	}

	~TextFileImpl()
	{
		if ( m_file )
		{
			fclose( m_file );
			m_file = 0;
		}
	}

	bool eof() const
	{
		if ( err )
			return true;
		return 0 != feof(m_file);
	}

	bool peekChar( char* ch )
	{
		if ( err )
			return false;

		if ( !m_peeked )
		{
			int c = getc( m_file );
			if ( EOF != c )
			{
				m_peeked = true;
				m_peekedChar = (char)c;
			}
			else
			{
				if ( ferror(m_file) )
					err = TextFile::ERROR_READ;
			}
		}

		if ( m_peeked )
			*ch = m_peekedChar;
		return m_peeked;
	}

	bool readChar( char* ch )
	{
		if ( err )
			return false;

		bool more = peekChar( ch );
		m_peeked = false;
		if ( more && *ch == '\n' )
			++line;
		return more;
	}

	bool skipWhitespace()
	{
		if ( err )
			return false;

		char ch;
		while ( peekChar(&ch) )
		{
			if ( !isspace(ch) )
				break;
			readChar( &ch );
		}
		return !eof();
	}

	bool readString( char* buf, int size )
	{
		if ( err )
			return false;

		skipWhitespace();

		int count = 0;
		char ch;
		while ( peekChar(&ch) )
		{
			if ( isspace(ch) )
				break;
			if ( count+1 < size )
				buf[count++] = ch;
			readChar( &ch );
		}
		if ( size > 0 )
			buf[count] = 0;
		return count > 0;
	}

	void skipLine()
	{
		if ( err )
			return;

		char ch;
		while ( readChar(&ch) )
		{
			if ( ch == '\n' )
				break;
		}
	}

	long readHex()
	{
		if ( err )
			return 0;

		skipWhitespace();

		// hex must start with alphanumeric character
		char ch;
		if ( !peekChar(&ch) || !isalnum(ch) )
		{
			err = TextFile::ERROR_PARSE;
			return 0;
		}

		long x = 0;
		while ( peekChar(&ch) )
		{
			switch ( ch )
			{
			case '0':	x <<= 4; x += 0; break;
			case '1':	x <<= 4; x += 1; break;
			case '2':	x <<= 4; x += 2; break;
			case '3':	x <<= 4; x += 3; break;
			case '4':	x <<= 4; x += 4; break;
			case '5':	x <<= 4; x += 5; break;
			case '6':	x <<= 4; x += 6; break;
			case '7':	x <<= 4; x += 7; break;
			case '8':	x <<= 4; x += 8; break;
			case '9':	x <<= 4; x += 9; break;
			case 'a':
			case 'A':	x <<= 4; x += 0xA; break;
			case 'b':
			case 'B':	x <<= 4; x += 0xB; break;
			case 'c':
			case 'C':	x <<= 4; x += 0xC; break;
			case 'd':
			case 'D':	x <<= 4; x += 0xD; break;
			case 'e':
			case 'E':	x <<= 4; x += 0xE; break;
			case 'f':
			case 'F':	x <<= 4; x += 0xF; break;
			default:	return x;
			}
			readChar( &ch );
		}
		return x;
	}

private:
	bool	m_peeked;
	char	m_peekedChar;
	FILE*	m_file;

	TextFileImpl( const TextFileImpl& );
	TextFileImpl& operator=( const TextFileImpl& );
};

//-----------------------------------------------------------------------------

TextFile::TextFile( const char* filename )
{
	m_this = new TextFileImpl( filename );
}

TextFile::~TextFile()
{
	delete m_this;
}

bool TextFile::readString( char* buf, int size )
{
	return m_this->readString( buf, size );
}

void TextFile::skipLine()
{
	m_this->skipLine();
}

long TextFile::readHex()
{
	return m_this->readHex();
}

bool TextFile::skipWhitespace()
{
	return m_this->skipWhitespace();
}

TextFile::ErrorType TextFile::error() const
{
	return m_this->err;
}

bool TextFile::readChar( char* ch )
{
	return m_this->readChar( ch );
}

bool TextFile::peekChar( char* ch )
{
	return m_this->peekChar( ch );
}

bool TextFile::eof() const
{
	return m_this->eof();
}

int TextFile::line() const
{
	return m_this->line;
}
