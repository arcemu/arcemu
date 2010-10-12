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
 
#ifndef _DEV_STACKTRACE_H
#define _DEV_STACKTRACE_H

class MapFile;

/** Stack tracing utility. */
class StackTrace
{
public:
	/**
	 * Prints formatted call stack to the user buffer.
	 * Always terminates the user buffer with 0.
	 *
	 * @param map Array of pointers to map files.
	 * @param maps Number of map files.
	 * @param initLevel Number of functions to skip before starting the tracing.
	 * @param maxDepth Maximum number of levels in the stack trace.
	 * @param buffer [out] Output buffer for the formatted stack trace.
	 * @param bufferSize Size of the output buffer.
	 * @return Needed buffer size.
	 */
	static int	printStackTrace( MapFile** map, int maps,
					int initLevel, int maxDepth,
					char* buffer, int bufferSize );
};

#endif // _DEV_STACKTRACE_H
