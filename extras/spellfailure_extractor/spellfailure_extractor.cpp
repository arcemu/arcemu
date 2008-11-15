// spellfailure_extractor.cpp : Defines the entry point for the console application.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>

#define SEARCH_TEXT "SPELL_FAILED_AFFECTING_COMBAT"
#define SEARCH_TEXT2 "PETTAME_INVALIDCREATURE"
#define FIRST_FAILURE 0

bool reverse_pointer_back_to_string(char ** ptr, char * str)
{
	size_t slen = strlen(str);
	size_t i;
	for(;;)
	{
		while((*ptr)[0] != str[0])
		{
			(*ptr)--;
		}

		for(i = 0; i < slen; ++i)
			if((*ptr)[i] != str[i])
				break;

		if(i == slen)
			return true;
		else
			(*ptr)--;
	}
	return false;
}
int find_string_in_buffer(char * str, size_t str_len, char * buf, size_t buf_len)
{
	char * p = buf;
	char * p_end = buf + buf_len;
	size_t remaining = buf_len;
	size_t i;

	for(;;)
	{
		while(*p != str[0] && p != p_end)
		{
			--remaining;
			++p;
		}

		if(p == p_end)
			break;

		if(remaining < str_len)
			break;

		for(i = 0; i < str_len; ++i)
		{
			if(p[i] != str[i])
				break;
		}

		if(i == str_len)
			return (int)(p - buf);

		*p++;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	FILE * in = fopen("WoW.exe", "rb");
	FILE * out = fopen("SpellFailure.h", "w");
	if(!in || !out)
		return 1;

	fseek(in, 0, SEEK_END);
	int len = ftell(in);
	fseek(in, 0, SEEK_SET);

	char * buffer = (char*)malloc(len);
	if(!buffer)
		return 2;

	if(fread(buffer, 1, len, in) != len)
		return 3;

	printf("Searching for `%s`...", SEARCH_TEXT);
	size_t offset = find_string_in_buffer(SEARCH_TEXT, strlen(SEARCH_TEXT), buffer, len);
	printf(" at %d.\n", offset);
	if(offset < 0)
		return 3;

	/* dump header */
	fprintf(out, "/****************************************************************************\n");
    fprintf(out, " *\n");
    fprintf(out, " * Spell System\n");
    fprintf(out, " * Copyright (c) 2007 Antrix Team\n");
    fprintf(out, " *\n");
    fprintf(out, " * This file may be distributed under the terms of the Q Public License\n");
    fprintf(out, " * as defined by Trolltech ASA of Norway and appearing in the file\n");
    fprintf(out, " * COPYING included in the packaging of this file.\n");
    fprintf(out, " *\n");
    fprintf(out, " * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n");
    fprintf(out, " * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n");
    fprintf(out, " *\n");
    fprintf(out, " */\n");
	fprintf(out, "\n#ifndef _SPELLFAILURE_H\n#define _SPELLFAILURE_H\n\nenum SpellCastError\n{\n");

	printf("Ripping...");
	char * p = (buffer + offset);
	char * name = p;
	int index = FIRST_FAILURE;
	size_t endoffset = find_string_in_buffer("SPELL_FAILED_UNKNOWN", strlen("SPELL_FAILED_UNKNOWN"), buffer, len);
	char *endp=(buffer + endoffset);
	do
	{
		name = p;
		fprintf(out, "\t%-60s = %d,\n", name,index);
		p--;
		if(p<endp)
			break;
		index++;
		reverse_pointer_back_to_string(&p, "SPELL_FAILED");
	}while( 1 );

	fprintf(out, "};\n");

	fprintf(out, "enum PetTameFailure\n{\n");
	offset = find_string_in_buffer(SEARCH_TEXT2, strlen(SEARCH_TEXT2), buffer, len);
	endoffset = find_string_in_buffer("PETTAME_UNKNOWNERROR", strlen("PETTAME_UNKNOWNERROR"), buffer, len);
	endp=(buffer + endoffset);
	p = (buffer + offset);
	name = p;
	index = 1;
	do
	{
		name = p;
		fprintf(out, "\t%-60s = %d,\n", name,index);
		p--;
		if(p<endp)
			break;
		index++;
		reverse_pointer_back_to_string(&p, "PETTAME");
	}while( 1 );
	fprintf(out, "};\n");

	fprintf(out, "\n#endif\n\n");
	fclose(out);
	fclose(in);
	free(buffer);

	printf("\nDone.\n");

	return 0;
}

