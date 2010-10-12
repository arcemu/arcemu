/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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
 * spellfailure_extractor.cpp : Defines the entry point for the console application.
 */

#include <cstring>
#include <cstdio>
#include <cstdlib>

#define SEARCH_TEXT "SPELL_FAILED_SUCCESS"
#define SEARCH_TEXT2 "PETTAME_INVALIDCREATURE"
#define FIRST_FAILURE 0
#define INDEX_CANTDO 173

const char *Executable = "wow.exe";
const char *OutputFile = "SpellFailure.h";

const char *HDR = "/*\n * ArcEmu MMORPG Server\n * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>\n * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>" \
" *\n * This program is free software: you can redistribute it and/or modify\n * it under the terms of the GNU Affero General Public License as published by\n * the Free Software Foundation, either version 3 of the License, or" \
" * any later version.\n *\n * This program is distributed in the hope that it will be useful,\n * but WITHOUT ANY WARRANTY; without even the implied warranty of\n * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" \
" * GNU Affero General Public License for more details.\n *\n * You should have received a copy of the GNU Affero General Public License\n * along with this program.  If not, see <http://www.gnu.org/licenses/>.\n *\n */";

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
	FILE * in = fopen( Executable, "rb");
	FILE * out = fopen( OutputFile, "w");
	
	if( in == NULL ){
		printf("ERROR: Couldn't open %s for reading!\n", Executable );
		printf("Exiting.\n");
		return -1;
	}

	if( out == NULL ){
		printf("ERROR: Couldn't open %s for writing!\n", OutputFile );
		printf("Exiting.\n");
		return -1;
	}

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

	fprintf(out, "%s", HDR );
	fprintf(out, "\n\n" );
	fprintf(out, "\n#ifndef _SPELLFAILURE_H\n#define _SPELLFAILURE_H\n\nenum SpellCastError\n{\n");

	printf("Ripping...");
	char * p = (buffer + offset);
	char * name = p;
	int index = FIRST_FAILURE;
	size_t endoffset = find_string_in_buffer("SPELL_FAILED_UNKNOWN", strlen("SPELL_FAILED_UNKNOWN"), buffer, len);
	char *endp=(buffer + endoffset);
	do
	{
		// This is a terrible hack, it will most likely be incorrect later
		if( index == INDEX_CANTDO ){
			fprintf(out, "\t%-60s = %d,\n", "SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW",index);
			index++;
			continue;
		}

		name = p;
		fprintf(out, "\t%-60s = %d,\n", name,index);
		p--;
		if(p<endp)
			break;
		index++;
		reverse_pointer_back_to_string(&p, "SPELL_FAILED");
	}while( 1 );

	// fprintf(out, "\t%-60s = %d,\n", "SPELL_CANCAST_OK",255);
	fprintf(out, "};\n");
	fprintf(out, "#define SPELL_CANCAST_OK SPELL_FAILED_SUCCESS\n");
	fprintf(out, "\n");

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

