/*****************************************************************************/
/* StormLibTest.cpp                       Copyright (c) Ladislav Zezula 2003 */
/*---------------------------------------------------------------------------*/
/* This module uses very brutal test methods for StormLib. It extracts all   */
/* files from the archive with Storm.dll and with stormlib and compares them,*/
/* then tries to build a copy of the entire archive, then removes a few files*/
/* from the archive and adds them back, then compares the two archives, ...  */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 25.03.03  1.00  Lad  The first version of StormLibTest.cpp                */
/*****************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include <list>


#define __STORMLIB_SELF__               // Don't use StormLib.lib
#include "StormLib.h"

#pragma warning(disable : 4505) 
#pragma comment(lib, "Winmm.lib")

//From Extractor
#include "adtfile.h"
#include "wdtfile.h"
#include "dbcfile.h"
#include "mpq.h"
#include "wmo.h"

//------------------------------------------------------------------------------
// Defines

#define MPQ_BLOCK_SIZE 0x1000

//-----------------------------------------------------------------------------
// from extractor
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define		ModelDBC_ModelId_col		0
#define		DisplayDBC_DisplayId_col	0
#define		DisplayDBC_ModelId_col		1
#define		DisplayDBC_Scale_col		4

char		input_path[1024]=".";
DBCFile		modeldbc;
DBCFile		displaydbc;
FILE		*out_sql_f;

void strtolower(char *dest, const char *src)
{
	if( !dest || !src )
		return;
	for(unsigned int i=0;i<strlen(src);i++)
		if( src[i] <= 'Z' && src[i] >= 'A' )
			dest[i] = src[i] + ('a' - 'A');
		else 
			dest[i] = src[i];
}

char tolower(char src)
{
	if( src <= 'Z' && src >= 'A' )
		return  src + ('a' - 'A');
	return src;
}

int FindModelIdForM2(const char *m2name,unsigned int prev_model_id)
{
	if( !m2name )
		return 0;

	char loverm2[500];
	unsigned int x=0;
	strtolower(loverm2, m2name);

	unsigned int comparecharcount=strlen( m2name );
	comparecharcount += 1; //this is because we have format ".m2" and comparing with ".mdx"

	//skip already processed ones
	if( prev_model_id )
		for(x=0;x<modeldbc.getRecordCount();x++)
			if( modeldbc.getRecord (x).getUInt( ModelDBC_ModelId_col ) == prev_model_id )
			{
				x++;
				break;
			}
	for(;x<modeldbc.getRecordCount();x++)
	{
		//compare 2 strings to see if the model name matches
		char *recname = (char *)modeldbc.getRecord(x).getString(2);
		if( !recname )
			continue;
		unsigned int reclen = strlen( recname );
		if( reclen != comparecharcount )
			continue;
		for(unsigned int charc=0;charc<comparecharcount;charc++)
		{
			if( loverm2[charc]=='.' )
				return modeldbc.getRecord (x).getUInt( ModelDBC_ModelId_col ); // the model id
			if(  loverm2[charc] != tolower( recname[charc] ) )
				break;
		}
	}
	return 0;
}

//a lot of display ids can use the same base model id
void GenerateDisplaySizeSQLs(const char *model_name, unsigned int model_id,float model_halfsize)
{
	for(unsigned int x=0;x<displaydbc.getRecordCount();x++)
		if( displaydbc.getRecord (x).getUInt( DisplayDBC_ModelId_col ) == model_id )
//			fprintf(out_sql_f,"Replace into unit_display_sizes ( displayid, halfsize, modelid, modelname ) values (%u,%f,%u,'%s');\n",displaydbc.getRecord (x).getUInt(DisplayDBC_DisplayId_col),model_halfsize,model_id,model_name);
			fprintf(out_sql_f,"Replace into unit_display_sizes ( displayid, halfsize, modelid, modelname ) values (%u,%f,%u,'%s');\n",displaydbc.getRecord (x).getUInt(DisplayDBC_DisplayId_col),model_halfsize*displaydbc.getRecord (x).getFloat(DisplayDBC_Scale_col),model_id,model_name);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//since we are using archives 1 model will be found more then once !
int GnerateCreatureSizeFromM2(const std::vector<std::string>& pArchiveNames)
{

    char* szListFile = ""; 
    char   szLocalFile[MAX_PATH] = "";
    HANDLE hMpq = ""; 
    BOOL bResult = FALSE;

    int nError = ERROR_SUCCESS;
    if(szListFile == NULL || *szListFile == 0)
        szListFile = NULL;

	for (size_t i=0; i<pArchiveNames.size(); i++)
    {
        //sprintf(tmp,"%s\\%s", input_path, ParsArchiveNames[i]);
        //if(!SFileOpenArchive(tmp, 0, 0, &hMpq))
        if(!SFileOpenArchive(pArchiveNames[i].c_str(), 0, 0, &hMpq))
            printf("NOT open!!! %s\n",pArchiveNames[i].c_str());

		printf("Processing archive %s \n",pArchiveNames[i].c_str());

        // Copy files from archive
        if(nError == ERROR_SUCCESS)
        {	
            SFILE_FIND_DATA wf;
            HANDLE hFind = SFileFindFirstFile(hMpq,"*.*2*", &wf, szListFile);
            bResult = TRUE;

            while(hFind != NULL && bResult == TRUE)
			{
				//we care about creatures only right now
//				if( (wf.cFileName[0] == 'C' || wf.cFileName[0] == 'c' )
//					&& (wf.cFileName[1] == 'R' || wf.cFileName[1] == 'r' )
//					)
	            {
					string filename = wf.cFileName;
					Model model(filename);
					if( model.open() )
					{
						float CanReachRad = model.GetModelCanReachRadius();
						if( CanReachRad )
						{
							int model_id=0;
							do { 
								model_id = FindModelIdForM2( wf.cFileName, model_id );
								if( model_id )
									GenerateDisplaySizeSQLs(wf.cFileName,model_id,CanReachRad);
//									fprintf( outf, "%s = %f = %u \n",wf.cFileName,CanReachRad,model_id);
								else printf("Could not determine model_id %s \n",wf.cFileName);
							}while( model_id );
						}
						else printf("Could not determine size of model %s \n",wf.cFileName);
					}
				}
			    // Find the next file
				bResult = SFileFindNextFile(hFind, &wf);
			}
            // Delete the extracted file in the case of an error
            if(nError != ERROR_SUCCESS)
                DeleteFile(szLocalFile);
            // Close the search handle
            if(hFind != NULL)
                SFileFindClose(hFind);

        }
    }
    // Close both archives
    if(hMpq != NULL)
        //SFileCloseArchive(hMpq);
        if(nError == ERROR_SUCCESS)
            printf("\nExtract m2 complete (No errors)\n");

    return nError;

}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void getGamePath()
{
#ifdef _WIN32
    HKEY key;
    DWORD t,s;
    LONG l;
    s = sizeof(input_path);
    memset(input_path,0,s);
    l = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Blizzard Entertainment\\World of Warcraft",0,KEY_QUERY_VALUE,&key);
    l = RegQueryValueEx(key,"InstallPath",0,&t,(LPBYTE)input_path,&s);
    RegCloseKey(key);
	if (strlen(input_path) > 0)
	{
		if (input_path[strlen(input_path) - 1] != '\\') strcat(input_path, "\\");
	}
    strcat(input_path,"Data\\");
#else
    strcpy(input_path,"data/");
#endif
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

bool scan_patches(char* scanmatch, std::vector<std::string>& pArchiveNames)
{
    int i;
    char path[512];
    std::list<std::string> matches;

    WIN32_FIND_DATA ffData;
    HANDLE hFind;

    for (i = 1; i <= 99; i++)
    {
        if (i != 1)
        {
            sprintf(path, "%s-%d.mpq", scanmatch, i);
        }
        else
        {
            sprintf(path, "%s.mpq", scanmatch);
        }

        hFind = INVALID_HANDLE_VALUE;
        hFind = FindFirstFile(path, &ffData);
        if (hFind == INVALID_HANDLE_VALUE) break;
        FindClose(hFind);

        matches.push_back(path);
    }

    matches.reverse();
    for (std::list<std::string>::iterator i = matches.begin(); i != matches.end(); i++)
    {
        pArchiveNames.push_back(i->c_str());
    }

    printf("\n");

    return(true);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

bool fillArchiveNameVector(std::vector<std::string>& pArchiveNames) {
    //srand((unsigned int)time(0));

    getGamePath();

    printf("\nGame path: %s\n", input_path);

    char path[512];
    std::vector<std::string> locales;

    // scan game directories
    WIN32_FIND_DATA ffData;
    HANDLE hFind;
    DWORD dwError;

    // first, scan for locales (4-letter directories)
    printf("Scanning for locales.\n");
    sprintf(path, "%s*.*", input_path);
    hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFile(path, &ffData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("\nCould not open data directory for reading. Aborting.\n");
        return(false);
    }
    do
    {
        if (ffData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (ffData.cFileName[0] != '.')
            {
                if (strlen(ffData.cFileName) == 4)
                {
                    printf("Found locale: %s\n", ffData.cFileName);
                    locales.push_back(ffData.cFileName);
                }
            }
        }
    } while (FindNextFile(hFind, &ffData) != 0);
    dwError = GetLastError();
    FindClose(hFind);
    if (dwError != ERROR_NO_MORE_FILES)
    {
        printf("\nError reading data directory while scanning locales. Aborting.\n");
        return(false);
    }
    printf("\n");

    if (locales.size() == 0)
    {
        printf("Sorry, no locales found. Aborting.\n");
        return(false);
    }

    // now, scan for the patch levels in the core dir
    printf("Loading patch levels from data directory.\n");
    sprintf(path, "%spatch", input_path);
    if (!scan_patches(path, pArchiveNames)) return(false);

    // now, scan for the patch levels in locale dirs
    printf("Loading patch levels from locale directories.\n");
    for (std::vector<std::string>::iterator i = locales.begin(); i != locales.end(); i++)
    {
        printf("Locale: %s\n", i->c_str());
        sprintf(path, "%s%s\\patch-%s", input_path, i->c_str(), i->c_str());
        if (!scan_patches(path, pArchiveNames)) return(false);
    }

    // open expansion and common files
    printf("Opening data files from data directory.\n");
    sprintf(path, "%sexpansion.mpq", input_path);
    pArchiveNames.push_back(path);
    sprintf(path, "%scommon.mpq", input_path);
    pArchiveNames.push_back(path);
    printf("\n");

    // open locale expansion and common files
    printf("Opening data files from locale directories.\n");
    for (std::vector<std::string>::iterator i = locales.begin(); i != locales.end(); i++)
    {
        printf("Locale: %s\n", i->c_str());
        sprintf(path, "%s%s\\expansion-locale-%s.mpq", input_path, i->c_str(), i->c_str());
        pArchiveNames.push_back(path);
        sprintf(path, "%s%s\\locale-%s.mpq", input_path, i->c_str(), i->c_str());
        pArchiveNames.push_back(path);
        printf("\n");
    }
    return true;
}

//compile and run the exe
// you need to have "CreatureModelData.dbc" for exe
int main()
{
	char *versionString = "V1.0 2008_06_07";

	if( !modeldbc.open("CreatureModelData.dbc") )
	{
		printf("Could not open 'CreatureModelData.dbc' \n");
		return 1;
	}

	if( !displaydbc.open("CreatureDisplayInfo.dbc") )
	{
		printf("Could not open 'CreatureDisplayInfo.dbc' \n");
		return 1;
	}

	out_sql_f = fopen("creature_display_size.sql","w");
	if( !out_sql_f )
	{
		printf(" Could not open output file\n");
		return 1;
	}
	fprintf(out_sql_f,"DROP TABLE IF EXISTS `unit_display_sizes`;\n");
	fprintf(out_sql_f,"CREATE TABLE IF NOT EXISTS `unit_display_sizes` (\n");
	fprintf(out_sql_f,"  `DisplayID` int(11) unsigned NOT NULL DEFAULT 0 ,\n");
	fprintf(out_sql_f,"  `halfsize` float NOT NULL DEFAULT '1' ,\n");
	fprintf(out_sql_f,"  `modelid` int(11) unsigned NOT NULL DEFAULT '0' ,\n");
	fprintf(out_sql_f,"  `modelname` varchar(500) ,\n");
	fprintf(out_sql_f,"  PRIMARY KEY (`DisplayID`),\n");
	fprintf(out_sql_f,"  UNIQUE KEY DisplayID (`DisplayID`),\n");
	fprintf(out_sql_f,"   KEY DisplayID_2 (`DisplayID`));\n");

	printf("Extract %s. Beginning work ....\n",versionString);
    // Set the lowest priority to allow running in the background
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    // patch goes first -> fake priority handling
    std::vector<MPQArchive*> archives;

    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    std::vector<std::string> archiveNames;

    fillArchiveNameVector(archiveNames);
    for (size_t i=0; i<archiveNames.size(); i++) {
        archives.push_back(new MPQArchive(archiveNames[i].c_str()));
    }

	//now fetch the model id for the filenames
    GnerateCreatureSizeFromM2(archiveNames);

	fclose( out_sql_f );
    printf("Extract %s. Work complete. No errors.",versionString);
}
