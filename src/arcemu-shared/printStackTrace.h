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

#ifndef _PRINTSTACKTRACE_H
#define _PRINTSTACKTRACE_H

/**
 * Prints formatted call stack to the user defined buffer,
 * always terminating the buffer with 0.
 * Uses stack frame to find out the caller function address and
 * the map file to find out the function name.
 */
extern SERVER_DECL void printStackTrace(char* buffer, int bufferSize);
extern SERVER_DECL void printStackTrace();
extern SERVER_DECL void arcAssertFailed(const char* fname, int line, const char* expr);

#endif // _PRINTSTACKTRACE_H
