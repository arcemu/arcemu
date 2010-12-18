/*

Purpuse : make modifications in dbc file for ascent project to set values that are missing for spells in able to work
Warning : Output result is changed in order to obtain ascent specific data. Do not try to use or interpret data for anything else
Warning : SpellEntry + sql_translation_table + SPELL_ENTRY must have the same structure !!!

project status : not finished yet..not tested

*/

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>

#include "../../../src/arcemu-shared/Database/dbcfile.h"

#include "defines.h"

DBCFile dbc;

//make this after the main
void dump_as_sql();

void main()
{
	dbc.open("Spell.dbc");

	if(!dbc.getFieldCount())
	{
		printf("error, could not open dbc file\n");
		exit(1);
	}
	else printf("Opened DBC with %u fields and %u rows\n",(int)dbc.getFieldCount(),(int)dbc.getRecordCount());

	dump_as_sql();
//	getch();
}

void dump_as_sql()
{
	if(SPELL_COLUMN_COUNT!=dbc.getFieldCount())
	{
		printf("error,column counts do not match update code for latest dbc format\n");
		exit(1);
	}

	printf("will start dumping data into sql file (we will drop not required fields!)\n");

	FILE *fsql=fopen("dbc_spell.sql","w");

	//drop table if already exist
	fprintf(fsql,"%s","DROP TABLE IF EXISTS `dbc_spell`;\n");

	//create the table
	fprintf(fsql,"%s","CREATE TABLE dbc_spell (\n");

	for(int i=0;i<SPELL_COLUMN_COUNT;i++)
		if(sql_translation_table[i][2][0]=='0')
		{
			if(sql_translation_table[i][0][0]=='u')
				fprintf(fsql,"\t `%s` INT (11) UNSIGNED DEFAULT '0' NOT NULL,\n",sql_translation_table[i][1]);
			else if(sql_translation_table[i][0][0]=='i')
				fprintf(fsql,"\t `%s` INT (11) DEFAULT '0' NOT NULL,\n",sql_translation_table[i][1]);
			else if(sql_translation_table[i][0][0]=='f')
				fprintf(fsql,"\t `%s` FLOAT DEFAULT '0' NOT NULL,\n",sql_translation_table[i][1]);
			else if(sql_translation_table[i][0][0]=='s')
				fprintf(fsql,"\t `%s` VARCHAR(2000),\n",sql_translation_table[i][1]);
		}

	fprintf(fsql,"%s","PRIMARY KEY(id), UNIQUE(id), INDEX(id));\n");

	fprintf(fsql,"\n\n");
	//start dumping the data from the DBC

	char tstr[2000];
	for(unsigned int j=0;j<dbc.getRecordCount();j++)
	{
		//we start a new insert block
		if((j % SQL_INSERTS_PER_QUERY) == 0)
		{
			fprintf(fsql,"%s","INSERT INTO dbc_spell (");
			for(int i=0;i<SPELL_COLUMN_COUNT-1;i++)
				if(sql_translation_table[i][2][0]=='0')
					fprintf(fsql,"`%s`,",sql_translation_table[i][1]);
			fprintf(fsql,"`%s`) values \n",sql_translation_table[SPELL_COLUMN_COUNT-1][1]);
			fprintf(fsql," (");
		}
		else
			fprintf(fsql,",(");
		for(int i=0;i<SPELL_COLUMN_COUNT;i++)
			if(sql_translation_table[i][2][0]=='0')
			{
				if(i!=0)
					fprintf(fsql,",");
				if(sql_translation_table[i][0][0]=='u')
					fprintf(fsql,"%u",dbc.getRecord(j).getUInt(i));
				else if(sql_translation_table[i][0][0]=='i')
					fprintf(fsql,"%d",dbc.getRecord(j).getInt(i));
				else if(sql_translation_table[i][0][0]=='f')
					fprintf(fsql,"%f",dbc.getRecord(j).getFloat(i));
				else if(sql_translation_table[i][0][0]=='s')
				{
					const char *dstr=dbc.getRecord(j).getString(i);
					int otherindex=0;
					for(unsigned int k=0;k<=strlen(dstr);k++)
						if(dstr[k]=='\'' || dstr[k]=='"' || dstr[k]=='#' || dstr[k]=='-')
						{
							tstr[otherindex++] = '\\';
							tstr[otherindex++] = dstr[k];
						}
						else
							tstr[otherindex++] = dstr[k];
					fprintf(fsql,"\"%s\"",tstr);
				}
			}
		//we need to end an insert block
		if(((j+1) % SQL_INSERTS_PER_QUERY)==0)
			fprintf(fsql,");\n");
		else fprintf(fsql,")\n");
	}
	fprintf(fsql,";");

	fprintf(fsql,"\n\n");

	fclose(fsql);
}
