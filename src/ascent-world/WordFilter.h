/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef _WORDFILTER_H
#define _WORDFILTER_H

struct WordFilterMatch
{
	char * szMatch;
	char * szIgnoreMatch;
	void* pCompiledExpression;
	void* pCompiledIgnoreExpression;
	void* pCompiledExpressionOptions;
	void* pCompiledIgnoreExpressionOptions;
	int iType;
};

class WordFilter
{
	WordFilterMatch ** m_filters;
	size_t m_filterCount;

	bool CompileExpression(const char * szExpression, void** pOutput, void** pExtraOutput);

public:
	WordFilter() : m_filters(NULL),m_filterCount(0) {}
	~WordFilter();

	void Load(const char * szTableName);
	bool Parse(string& sMessage, bool bAllowReplace = true);
};

extern WordFilter * g_characterNameFilter;
extern WordFilter * g_chatFilter;

#endif		// _WORDFILTER_H
