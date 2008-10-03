/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"
extern "C" {
#include <pcre.h>
};

#define REPLACE_FILTER 1
#define SEARCH_FILTER 0

WordFilter * g_characterNameFilter;
WordFilter * g_chatFilter;

WordFilter::~WordFilter()
{
	size_t i;
	WordFilterMatch * p;

	if(!m_filters)
		return;

	for(i = 0; i < m_filterCount; ++i)
	{
		p = m_filters[i];
		if(p==NULL)
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

bool WordFilter::CompileExpression(const char * szExpression, void** pOutput, void** pExtraOutput)
{
	pcre * re;
	pcre_extra * ee = 0;
	const char * error;
	const char * error2 = 0;
	int erroffset;

	re = pcre_compile(szExpression, 0, &error, &erroffset, NULL);
	if(re != NULL)
		ee = pcre_study(re, 0, &error2);

	if(re == NULL || error2 != NULL)
	{
		if(re == NULL)
			Log.Error("WordFilter", "An error occurred while compiling the expression: \"%s\": %s", szExpression, error ? error : "unknown error");
		else
			Log.Error("WordFilter", "An error occurred while compiling extra data for the expression: \"%s\": %s", szExpression, error2);

		if(re)
			pcre_free(re);

		return false;
	}

    *pOutput = (void*)re;
	*pExtraOutput = (void*)ee;
	return true;
}

void WordFilter::Load(const char * szTableName)
{
	WordFilterMatch * pMatch;
	size_t i;
	list<WordFilterMatch*> lItems;
	list<WordFilterMatch*>::iterator itr;
	QueryResult * pResult = WorldDatabase.Query("SELECT * FROM %s", szTableName);
	if(pResult==NULL)
		return;

	do 
	{
		pMatch = new WordFilterMatch;
		pMatch->szMatch = (strlen(pResult->Fetch()[0].GetString()) > 1) ? strdup(pResult->Fetch()[0].GetString()) : NULL;
		if(pMatch->szMatch==NULL)
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
			pMatch->pCompiledIgnoreExpression=NULL;
			pMatch->pCompiledIgnoreExpressionOptions=NULL;
		}

		pMatch->iType = pResult->Fetch()[2].GetUInt32();
		lItems.push_back(pMatch);
	} while (pResult->NextRow());
	delete pResult;

	if(lItems.size()==0)
		return;

	m_filters = new WordFilterMatch*[lItems.size()];
	i = 0;
	for(itr = lItems.begin(); itr != lItems.end(); ++itr)
		m_filters[i++] = (*itr);

	m_filterCount = i;
}

bool WordFilter::Parse(string& sMessage, bool bAllowReplace /* = true */)
{
#define N 10
#define NC (N*3)
	size_t i;
	int ovec[N*3];
	int n;
	WordFilterMatch * pFilter;
	const char * szInput = sMessage.c_str();
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
					// our string didnt match any of the excludes, so it passes
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
