<<<<<<< .working
<<<<<<< .working
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
=======
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
>>>>>>> .merge-right.r249
=======
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
>>>>>>> .merge-right.r249
