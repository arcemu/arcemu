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
 
#ifndef _DEV_TEXTFILE_H
#define _DEV_TEXTFILE_H

/** 
 * ASCII-7 text file parser. Doesnt throw exceptions.
 */
class TextFile
{
public:
	/** Error code. */
	enum ErrorType
	{
		/** No error. */
		ERROR_NONE,
		/** File open failed. */
		ERROR_OPEN,
		/** File reading failed. */
		ERROR_READ,
		/** Syntax error. */
		ERROR_PARSE
	};

	/** Opens a file. */
	explicit TextFile( const char* filename );

	///
	~TextFile();

	/** 
	 * Reads a single character. 
	 * @return true if read ok.
	 */
	bool		readChar( char* ch );

	/** 
	 * Peeks a single character. 
	 * @return true if peek ok.
	 */
	bool		peekChar( char* ch );

	/** 
	 * Reads whitespace delimited string.
	 * If the string doesnt fit to the buffer then
	 * the rest of the string is skipped. Buffer
	 * is always 0-terminated.
	 * @param buf [out] Pointer to string buffer.
	 * @param size String buffer size. Must be larger than 0.
	 * @return false if end-of-file reached before any characters was read.
	 */
	bool		readString( char* buf, int size );

	/** Skips the rest of the line. */
	void		skipLine();

	/** Reads hex integer. Skips preceding whitespace. */
	long		readHex();

	/** 
	 * Skips whitespace characters. 
	 * @return false if end-of-file reached.
	 */
	bool		skipWhitespace();

	/** Returns true if end-of-file have been reached. */
	bool		eof() const;

	/** Returns error code or 0 (ERROR_NONE) if no error. */
	ErrorType	error() const;

	/** Returns line number of last successful read character. */
	int			line() const;

private:
	class TextFileImpl;
	TextFileImpl* m_this;

	TextFile( const TextFile& );
	TextFile& operator=( const TextFile& );
};

#endif // _DEV_TEXTFILE_H
