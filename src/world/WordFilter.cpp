/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"
extern "C" {
#include <pcre.h>
};

#define REPLACE_FILTER 1
#define SEARCH_FILTER 0

WordFilter* g_characterNameFilter;
WordFilter* g_chatFilter;

WordFilter::~WordFilter()
{
	size_t i;
	WordFilterMatch* p;

	if(!m_filters)
		return;

	for(i = 0; i < m_filterCount; ++i)
	{
		p = m_filters[i];
		if(p == NULL)
			continue;

		if(p->szMatch)
		{
			free(p->szMatch);
			pcre_free(p->pCompiledExpression);
			if(p->pCompiledExpressionOptions)
				pcre_free(p->pCompiledExpressionOptions);
		}

		if(p->szIgnoreMatch)
		{
			pcre_free(p->pCompiledIgnoreExpression);
			if(p->pCompiledIgnoreExpressionOptions)
				pcre_free(p->pCompiledIgnoreExpressionOptions);

			free(p->szIgnoreMatch);
		}

		delete p;
	}

	delete [] m_filters;
}

bool WordFilter::CompileExpression(const char* szExpression, void** pOutput, void** pExtraOutput)
{
	pcre* re;
	pcre_extra* ee = 0;
	const char* error;
	const char* error2 = 0;
	int erroffset;

	re = pcre_compile(szExpression, 0, &error, &erroffset, NULL);
	if(re != NULL)
		ee = pcre_study(re, 0, &error2);

	if(re == NULL || error2 != NULL)
	{
		if(re == NULL)
			Log.Error("WordFilter", "An error occurred while compiling the expression: \'%s\': %s", szExpression, error ? error : "unknown error");
		else
			Log.Error("WordFilter", "An error occurred while compiling extra data for the expression: \'%s\': %s", szExpression, error2);

		if(re)
			pcre_free(re);

		return false;
	}

	*pOutput = (void*)re;
	*pExtraOutput = (void*)ee;
	return true;
}

void WordFilter::Load(const char* szTableName)
{
	WordFilterMatch* pMatch;
	size_t i;
	list<WordFilterMatch*> lItems;
	list<WordFilterMatch*>::iterator itr;
	QueryResult* pResult = WorldDatabase.Query("SELECT * FROM %s", szTableName);
	if(pResult == NULL)
		return;

	do
	{
		pMatch = new WordFilterMatch;
		pMatch->szMatch = (strlen(pResult->Fetch()[0].GetString()) > 1) ? strdup(pResult->Fetch()[0].GetString()) : NULL;
		if(pMatch->szMatch == NULL)
		{
			delete pMatch;
			continue;
		}

		pMatch->szIgnoreMatch = (strlen(pResult->Fetch()[1].GetString()) > 1) ? strdup(pResult->Fetch()[1].GetString()) : NULL;

		// compile the expression
		if(!CompileExpression(pMatch->szMatch, &pMatch->pCompiledExpression, &pMatch->pCompiledExpressionOptions))
		{
			free(pMatch->szMatch);
			if(pMatch->szIgnoreMatch)
				free(pMatch->szIgnoreMatch);
			delete pMatch;
			continue;
		}

		if(pMatch->szIgnoreMatch != NULL)
		{
			if(!CompileExpression(pMatch->szIgnoreMatch, &pMatch->pCompiledIgnoreExpression, &pMatch->pCompiledIgnoreExpressionOptions))
			{
				free(pMatch->szMatch);
				if(pMatch->szIgnoreMatch)
					free(pMatch->szIgnoreMatch);
				delete pMatch;
				continue;
			}
		}
		else
		{
			pMatch->pCompiledIgnoreExpression = NULL;
			pMatch->pCompiledIgnoreExpressionOptions = NULL;
		}

		pMatch->iType = pResult->Fetch()[2].GetUInt32();
		lItems.push_back(pMatch);
	}
	while(pResult->NextRow());
	delete pResult;

	if(lItems.size() == 0)
		return;

	m_filters = new WordFilterMatch*[lItems.size()];
	i = 0;
	for(itr = lItems.begin(); itr != lItems.end(); ++itr)
		m_filters[i++] = (*itr);

	m_filterCount = i;
}

bool WordFilter::Parse(string & sMessage, bool bAllowReplace /* = true */)
{
#define N 10
#define NC (N*3)
	size_t i;
	int ovec[N * 3];
	int n;
	WordFilterMatch* pFilter;
	const char* szInput = sMessage.c_str();
	size_t iLen = sMessage.length();
	//char

	for(i = 0; i < m_filterCount; ++i)
	{
		pFilter = m_filters[i];
		if((n = pcre_exec((const pcre*)pFilter->pCompiledExpression,
		                  (const pcre_extra*)pFilter->pCompiledExpressionOptions, szInput, (int)iLen, 0, 0, ovec, NC)) < 0)
		{
			if(n == PCRE_ERROR_NOMATCH)
				continue;

			Log.Error("WordFilter", "::Parse -> pcre_exec returned %d.", n);
			return false;
		}
		else
		{
			// one or more matches found
			if(pFilter->szIgnoreMatch)
			{
				// ignore case
				if((n = pcre_exec((const pcre*)pFilter->pCompiledIgnoreExpression,
				                  (const pcre_extra*)pFilter->pCompiledIgnoreExpressionOptions, szInput, (int)iLen, 0, 0, ovec, NC)) < 0)
				{
					// our string didn't match any of the excludes, so it passes
					if(n == PCRE_ERROR_NOMATCH)
						return true;

					Log.Error("WordFilter", "::Parse -> pcre_exec returned %d.", n);
					return false;
				}
				else
				{
					// our string passed this filter.
					continue;
				}
			}
			else
				return true;
		}
	}

	return false;
}

/*
	Perhaps this could use a little documentation:

	it works its way through the string looking for | symbols,
	if it finds one then it checks the following character and deals with it.

	it really should be a case of ORDER DENY,ALLOW, but atm it is ALLOW,DENY

	The following should be considered 'unfriendly' sequences. Ones that occur
	normally in chat, but only in specific circumstances, these should all be dealt with here:
	|c[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F] - Colour tag, should be followed by a |H *always*
	|r - end of colour tag, should always follow a |h
	|H - hyperlink open tag. Should always be followed by two |h. Should be followed by one of the following (not yet enforced)
		item
		spell
		achievement
		trade
		quest

	The following are the 'bad' sequences. These should NEVER be transmitted:
	|T and |t - open/close icon tags. Not handled here - if detected the entire message is dropped and the player gets a warning sysmsg
	\n - newline char. Not handled, afaik currently stripped out by the client

	The following are not particularly 'unfriendly' sequences, and aren't handled:
	|1 and |4 - plural sequence
	|3 - used to crash the client, appears to do nothing now
	|H - the following types are registered by default: gm_chat, player. The former opens GM Chat, and the latter when clicked
	opens a whisper to the player targetted by the link. Additional types may exist but would be registered by 3rd party addons
	so any exploit using it would depend on the presence of a specific 3rd party addon in the victim's client.

	This function:
	Removes invalidly placed |c and |r sequences, leaving the raw text (not blizzlike, but /care)
	Strips |H|h|h sequences down to the raw link text, minus the first and last characters (which SHOULD be the []) if links aren't enabled on the channel

	Don't worry about the GM tag icons and the raid target icons in chat, the former is inserted clientside depending on the sender flags in the message,
	the latter is transmitted as the text the user entered: {star}{skull} etc and is replaced client-side at the recipient.

	--Stewart, April 09
*/
bool WordFilter::ParseEscapeCodes(char* sMsg, bool bAllowLinks)
{
	if(!strstr(sMsg, "|"))
		return true;

	uint32 j = 0;
	for(j = 0; j < (((string)sMsg).length()); j++)
	{
		if(((string)sMsg).at(j) != char('|'))
			continue;

		//Myth fix server crashes, unhandled null pointers ftw
		if(j + 1 >= (((string)sMsg).length()))
			return true;

		string newstr;
		char* i;
		switch(((string)sMsg).at(j + 1))
		{
			case char('c'):
				if(((string)sMsg).length() < j + 10)
				{
					sMsg[j] = '\0';
					break;
				}
				i = sMsg + j + 10;
				if(strncmp(i, "|H", 2) == 0 && bAllowLinks)
					continue;
				newstr = ((string)sMsg).replace(j, 10, "");
				strcpy(sMsg, newstr.c_str());
				j = 0;
				break;
			case char('r'):
				i = (sMsg + j) - 2;
				if(strncmp(i, "|h", 2) == 0 && bAllowLinks)
				{
					continue;
				}
				newstr = ((string)sMsg).replace(j, 2, "");
				strcpy(sMsg, newstr.c_str());
				j = 0;
				break;
			case char('H'):
				//abc  |Hitem:111:2:3:4:5:6:7:8:9|h[Tough Jerky]|h|r
				//    ^=j                         ^=i-sMsg  (approx)
				i = strstr(sMsg, "|h");
				if(bAllowLinks && i)
					continue;
				if(i)
					newstr = ((string)sMsg).replace(j, ((size_t)(i - sMsg)) - (j), "");
				else
					newstr = ((string)sMsg).replace(j, strlen(sMsg) - j, "");
				strcpy(sMsg, newstr.c_str());

				j = 0;
				break;
			case char('h'):
				if(bAllowLinks)
					continue;
				if(((string)sMsg).at(j - 1) == char(']'))
					newstr = ((string)sMsg).replace(j - 1, 3, "");
				else
					newstr = ((string)sMsg).replace(j, 2, "");
				strcpy(sMsg, newstr.c_str());
				j = 0;
				break;
		}
	}
	return true;
}
