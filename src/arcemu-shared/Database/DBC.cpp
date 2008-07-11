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

#include "DBC.h"
#include "../NGLog.h"
#include <math.h>

class DBC;

DBC::DBC() {
	tbl = NULL;
	db = NULL;
	loaded = false;
	format = NULL;
}

void DBC::Load(const char *filename) {
	FILE *f = fopen(filename, "rb");
	if(!f)
	{
		//printf("DBC %s Doesnt exist!\n",filename);
		Log.Error("DBC", "DBC %s doesn't exist!\n", filename);
		return;
	}

	fseek(f, 4, SEEK_SET);
	fread(&rows,4, 1, f);
	fread(&cols, 4, 1, f);
	fread(&weird2, 4, 1, f);
	//int percol = weird2/cols;
	fread(&dblength, 4, 1, f);
#ifdef USING_BIG_ENDIAN
	swap32(&rows);
	swap32(&cols);
	swap32(&weird2);
	swap32(&dblength);
#endif

	tbl = new unsigned int[rows * cols];
	db = new char[dblength];
	format = new DBCFmat[cols];
	strcpy(name,filename);
	fread(tbl,rows*cols*4,1,f);
	fread(db,dblength,1,f);

#ifdef USING_BIG_ENDIAN
	/* burlex: this is a real hack. it'll mess up floats. i'm gonna rewrite the dbc interface soon :P */
	for(int i = 0; i < (rows*cols); ++i)
		tbl[i] = swap32((uint32)tbl[i]);
#endif

	fclose(f);
	loaded = true;

	Log.Notice("DBC", "Loaded %s (%u rows)", name, rows);
}


void DBC::Lookup(char* out, int row,int col,char isstr,bool onlystr)
{
	int fst = tbl[row*cols+col];
	char* str = db+fst;	
	if((fst > 0 && fst < dblength && col > 0 && !onlystr) || isstr == 1)
	{
		char bla = *(char*)(db+fst-1);
		if(bla == '\0' && fst != 1)
		{
			printf("%u\n",col);
			sprintf(out,"%s",str);
			return;
		}
	}
	sprintf(out,"%u",*(tbl+(row*cols+col)));
	return;
}

void DBC::CSV(char* filename, bool info)
{
	if(weird2 != cols*4) filename = strcat(filename,"-NOT.csv");
	FILE *f = fopen(filename, "wb");
	FILE *out = info ? f : stdout;
	fprintf(out,"Rows:%u\x0d\x0a",rows);
	fprintf(out,"Cols:%u\x0d\x0a",cols);
	fprintf(out,"Weird:%u\x0d\x0a",weird2);
	fprintf(out,"Theory:%u\x0d\x0a",(weird2 == cols*4));
	fprintf(out,"DBlength:%u\x0d\x0a",dblength);
	fprintf(out,"\x0d\x0a");

	for(int i=0; i < rows; i++)
	{
		for(int j=0; j < cols; j++)
		{
			char* str = new char[65535];
			Lookup(str,i,j);
			fprintf(f,"%s,",str);
			delete [] str;
		}
		fprintf(f,"\x0d\x0a");
	}
	fclose(f);
}

void DBC::RowToStruct(void* out, int row)
{
	memcpy(out,tbl+(row*cols),weird2);
}

void DBC::FormatCSV(const char* filename, bool info)
{
	//if(weird2 != cols*4) filename = strcat(filename,"-NOT.csv"); - Cause its a const.
	FILE *f = fopen(filename, "wb");
	FILE *out = info ? f : stdout;
	fprintf(out,"Rows:%u\x0d\x0a",rows);
	fprintf(out,"Cols:%u\x0d\x0a",cols);
	fprintf(out,"Weird:%u\x0d\x0a",weird2);
	fprintf(out,"Theory:%u\x0d\x0a",(weird2 == cols*4));
	fprintf(out,"DBlength:%u\x0d\x0a",dblength);
	fprintf(out,"\x0d\x0a");
	
	printf("Writing file (%s): 0%%",name);
	int percent=0,npercent;
	int fst;
	for(int i=0; i < rows; i++)
	{
		for(int j=0; j < cols; j++)
		{
			/*char* str = new char[512];
			LookupFormat(str,i,j);
			fprintf(f,"%s,",str);
			delete [] str;*/ // Old code -> too slow. keeping it for reference
			fst = tbl[i*cols+j];
			if(format[j] == F_STRING)
				fprintf(f,"\"%s\"",(char*)(db+fst));
			else if(format[j] == F_FLOAT)
				fprintf(f,"%f",*(float*)&fst);
			else
				fprintf(f,"%u",fst);
			fprintf(f,",");
			npercent = (int)((float)(i*cols+j)/(float)(rows*cols) * 100);
			if(npercent > percent)
			{
				printf("\rWriting file (%s): %u%%",name,npercent);
				percent = npercent;
			}
		}
		fprintf(f,"\x0d\x0a");
	}
	printf("\rWriting file (%s): 100%% - Done!\n",name);
	fclose(f);
}

void DBC::GuessFormat()
{
	int *ints,*floats,*strings;
	ints = new int[cols]; memset(ints,0x00,sizeof(int)*cols);
	floats = new int[cols]; memset(floats,0x00,sizeof(int)*cols);
	strings = new int[cols]; memset(strings,0x00,sizeof(int)*cols);
	printf("Guessing format (%s): 0%%",name);
	int percent=0,npercent;
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			DBCFmat f = GuessFormat(i,j);
			if(f == F_STRING) strings[j]++;
			else if(f == F_INT) ints[j]++;
			else if(f == F_FLOAT) floats[j]++;
			npercent = (int)((float)(i*cols+j)/(float)(rows*cols+cols) * 100);
			if(npercent > percent)
			{
				printf("\rGuessing format (%s): %u%%",name,npercent);
				percent = npercent;
			}
		}
	
	for(int j=0;j<cols;j++)
	{
		if(strings[j] > ints[j])
		{
			if(strings[j] > floats[j])
				format[j] = F_STRING;
			else
				format[j] = F_FLOAT;
		}
		else
		{
			if(ints[j] > floats[j])
				format[j] = F_INT;
			else
				format[j] = F_FLOAT;
		}
		npercent = (int)((float)(rows*cols+j)/(float)(rows*cols+cols) * 100);
		if(npercent > percent)
		{
			printf("\r%u%%",npercent);
			percent = npercent;
		}
	}
	delete [] ints;
	delete [] floats;
	delete [] strings;
	printf("\rGuessing format (%s): 100%% - Done!\n",name);
}

DBCFmat DBC::GuessFormat(int row, int col)
{
	unsigned int fst = tbl[row*cols+col];
	if(fst == 0) return F_NADA;
	else if(fst == 1) return F_INT;
	if(fst > 0 && fst < (unsigned int)dblength && col > 0 && *(char*)(db+fst-1) == 0x00) return F_STRING;
	if(fst > 100000000) return F_FLOAT;
	return F_INT;
}
void DBC::LookupFormat(char *out, int row, int col)
{
	int fst = tbl[row*cols+col];
	if(format[col] == F_STRING)
		sprintf(out,"%s",(char*)(db+fst));
	else if(format[col] == F_FLOAT)
		sprintf(out,"%f",*(float*)&fst);
	else
		sprintf(out,"%u",fst);
}

DBC::~DBC() {
	if(tbl) delete [] tbl;
	if(db) delete [] db;
	if(format) delete [] format;
}
