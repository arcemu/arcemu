/*
 * MPQ Extractor ( SFmpq.dll wrapper )
 * Copyright (C) 2005-2007 WRS <thewrs@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 2008-12-03,04 Re-written in c++
 */

#include <stdio.h>
#include <direct.h>
#include <windows.h>
#include <string.h>
#include "sfmpq.h"

int main(int argc, char** argv)
{
	char version[] = "1.4";
	int hMPQ = 0;
	int hFile = 0;
	int FileSize = 0;
	int FileRead = 0;
	size_t slen = 0;
	mpqeOptions op;
	AddressTable at;
	op.option_verbose = false;
	op.option_outdir = NULL;
	op.option_lowercase = false;
	op.option_baseMPQ = NULL;
	op.option_searchglob = NULL;
	op.option_skipdirstructure = false;
	char* curdir;
	char* errbuf;
	
	printf("MPQ-Extractor v%s by WRS", version);
	if(!LoadAddresses(&at))
	{
		return -1;
	}
	printf(" powered by %s\n", at.pMpqGetVersionString());
	SFMPQVERSION v = at.pSFMpqGetVersion();
	printf("version: %d.%d.%d.%d\n", v.Major, v.Minor, v.Revision, v.Subrevision);
	if(argc<2)
	{
		ShowHelp();
		return -1;
	}
	errbuf = new char[ERRBUF_SIZE];
	curdir = new char[MAX_PATH];
	if(_getcwd(curdir, MAX_PATH) == NULL)
	{
		_strerror_s(errbuf, ERRBUF_SIZE, NULL);
		fprintf(stderr,"Unable to get current working directory : %s\n", errbuf);
		strcpy_s(curdir,MAX_PATH,".");
	}
	for(int i = 1; i < argc; ++i)
	{
		if(_stricmp(argv[i],"/p")==0) // use patch files if available
		{
// this can be a major slowdown in a batch file processing several MPQs
// for speed, extract all the other MPQs first then the patch-*.mpq last
// don't fatal-error out, just print warning message
			printf("Ignoring /p option (removed)\n");
		}
		else if(_stricmp(argv[i],"/l")==0)
		{
			printf("Lowercase output: Enabled\n");
			op.option_lowercase = true;
		}
		else if(_stricmp(argv[i],"/v")==0)
		{
			printf("Verbose output: Enabled\n");
			op.option_verbose = true;
		}
		else if(_stricmp(argv[i],"/d")==0)
		{
			if((i+1)>=argc)
			{
				fprintf(stderr,"Fatal: No output directory specified for /d\n");
				ShowHelp();
				return -1;
			}
			printf("Output directory: %s\n", argv[++i]);
			if(op.option_outdir != NULL)
			{
				delete [] op.option_outdir;
			}
			slen = strlen(argv[i])+1;
			op.option_outdir = new char[slen];
			strcpy_s(op.option_outdir, slen, argv[i]);
		}
		else if(_stricmp(argv[i],"/s")==0)
		{
			op.option_skipdirstructure = true;
			printf("Skipping directory structure creation\n");
		}
		else if(argv[i][0]=='/')
		{
			fprintf(stderr,"Fatal: Unknown option %s\n", argv[i]);
			ShowHelp();
			return -1;
		}
		else
		{
			if(op.option_baseMPQ == NULL)
			{
				slen = strlen(argv[i])+1;
				op.option_baseMPQ = new char[slen];
				strcpy_s(op.option_baseMPQ, slen, argv[i]);
			}
			else
			{
				if(op.option_searchglob != NULL)
				{
					delete [] op.option_searchglob;
				}
				slen = strlen(argv[i]) + 1;
				op.option_searchglob = new char[slen];
				strcpy_s(op.option_searchglob, slen, argv[i]);
			}
		}
	}
	if(op.option_baseMPQ == NULL)
	{
		fprintf(stderr,"Fatal: Did not provide MPQ file for extraction!\n");
		ShowHelp();
		return -1;
	}
	if(op.option_outdir == NULL)
	{
		op.option_outdir = new char[16];
		strcpy_s(op.option_outdir, 16, "MPQOUT");
	}
	if(strchr(op.option_baseMPQ,'\\')) // contains a backslash, change directory
	{
		char* d = op.option_baseMPQ;
		char* p = strrchr(op.option_baseMPQ,'\\');
		*p = '\0';
		p++;
		op.option_baseMPQ = p;
		if(_chdir(d)==-1)
		{
			_strerror_s(errbuf, ERRBUF_SIZE, NULL);
			fprintf(stderr,"Unable to change directory to %s : %s\n", d, errbuf);
			return -1;
		}
	}

	WIN32_FIND_DATA fd;
	HANDLE hf = FindFirstFile(op.option_baseMPQ, &fd);
	if(hf == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr,"Fatal: Could not open MPQ archive %s Error: %s\n", op.option_baseMPQ, GetErrorText(GetLastError()));
		_chdir(curdir);
		return -1;
	}
	// first, get number of matching mpq files ...
	DWORD nFiles = 1;
	while(GetLastError()!=ERROR_NO_MORE_FILES)
	{
		if(FindNextFile(hf, &fd))
		{
			++nFiles;
		}
	}
	FindClose(hf);
	// got number of files, now fill information
	FileList* fList = new FileList[nFiles];
	hf = FindFirstFile(op.option_baseMPQ, &fd);
	if(hf == INVALID_HANDLE_VALUE) // how did this happen? it was valid before!
	{
		fprintf(stderr,"Fatal: Could not open MPQ archive %s Error: %s\n", op.option_baseMPQ, GetErrorText(GetLastError()));
		_chdir(curdir);
		return -1;
	}
	// first, get number of matching mpq files ...
	DWORD index;
	for( index=0; index<nFiles; ++index )
	{
		fList[index].fwtime.HighPart = fd.ftLastWriteTime.dwHighDateTime;
		fList[index].fwtime.LowPart = fd.ftLastWriteTime.dwLowDateTime;
		fList[index].fname = new char[strlen(fd.cFileName)+1];
		strcpy(fList[index].fname, fd.cFileName);
		while(!FindNextFile(hf, &fd))
		{
			if(GetLastError()==ERROR_NO_MORE_FILES)
			{
				nFiles = index+1;
				break;
			}
		}
	}
	// got all mpq file names and last-write-times, now sort them (process oldest first)
	if(nFiles > 1) // no need to sort if there's only one
	{
		FileList tmp;
		// this is probably a slow sorter, but how many mpqs are there going to be anyway?
		for(index=1; index<nFiles; ++index)
		{
			if(fList[index-1].fwtime.QuadPart < fList[index].fwtime.QuadPart) // swap these
			{
				tmp.fname = fList[index].fname;
				tmp.fwtime = fList[index].fwtime;
				fList[index].fname = fList[index-1].fname;
				fList[index].fwtime = fList[index-1].fwtime;
				fList[index-1].fname = tmp.fname;
				fList[index-1].fwtime = tmp.fwtime;
				index = 0; // start over
			}
		}
	}
	// now process the mpqs
	for( index=0; index<nFiles; ++index )
	{
		mpqExtract(&at, &op, fList[index].fname);
	}
	_chdir(curdir);
	delete [] curdir;
	delete [] errbuf;
	return 0;
}

bool Match(char* pattern, char* s, bool caseSensitive)
{
	char Wildcards[] = {'*', '?'};
	int numWildcards = 2;
	bool foundWildcard = false;
	size_t slen;
	if(pattern == NULL)
		return false;
	if(s == NULL)
		return false;
	static char* p2 = NULL;
	static char* s2 = NULL;
	slen = strlen(pattern);
	if(slen == 0)
		return false;
	if(p2 != NULL)
		delete [] p2;
	p2 = new char [slen+2];
	strcpy_s(p2, slen+2, pattern);
	slen = strlen(s);
	if(slen == 0)
		return false;
	if(s2 != NULL)
		delete [] s2;
	s2 = new char [slen+2];
	strcpy_s(s2, slen+2, s);
	if(!caseSensitive)
	{
		slen = strlen(pattern);
		_strlwr_s(p2, slen+2);
		slen = strlen(s);
		_strlwr_s(s2, slen+2);
	}
	for(int i=0; i<numWildcards; ++i)
	{
		if(strchr(p2, Wildcards[i]) != NULL)
			foundWildcard = true;
	}
	if(!foundWildcard)
	{
		return (strcmp(s2,p2)==0?false:true);
	}
	uint32 i = 0;
	uint32 j = 0;
	while (i < strlen(s2) && j < strlen(p2) && p2[j] != '*')
	{
		if ((p2[j] != s2[i]) && (p2[j] != '?'))
			return false;
		i++;
		j++;
	}
	if (j == strlen(p2))
		return (strlen(s2)==strlen(p2));
		
	int cp = 0;
	int mp = 0;
	while(i < strlen(s2))
	{
		if (j < strlen(p2) && p2[j] == '*')
		{
			if ((j++) >= strlen(p2))
				return true;
			mp = j;
			cp = i+1;
		}
		else if (j < strlen(p2) && (p2[j] == s[i] || p2[j] == '?'))
		{
			j++;
			i++;
		}
		else
		{
			j = mp;
			i = cp++;
		}
	}

	while (j < strlen(p2) && p2[j] == '*')
		j++;
	return (j >= strlen(p2));
}

char* bytes2text(int bytes)
{	// not long?
	static char* r = new char[16];
	if (bytes < 1024)
	{
		sprintf_s(r, 16, "%luB",bytes);
	}
	else if (bytes < 1024 * 1024)
	{
		sprintf_s(r, 16, "%luK", (bytes / 1024));
	}
	else if (bytes < 1024 * 1024 * 1024)
	{
		sprintf_s(r, 16, "%luM", (bytes / 1024 / 1024, 1));
	}
	else
	{
		strcpy_s(r, 16, "moo"); // lol?
	}
	return r;
}


BOOL LoadAddresses(AddressTable* at)
{
	HMODULE hModule = NULL;
	hModule = LoadLibrary("SFMPQ.DLL");
	if(hModule==NULL)
	{
		fprintf(stderr,"\nError loading SFMPQ.DLL: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqGetVersionString = (Function_MpqGetVersionString)GetProcAddress(hModule,"MpqGetVersionString");
	if(at->pMpqGetVersionString == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqGetVersionString: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqGetVersion = (Function_MpqGetVersion)GetProcAddress(hModule,"MpqGetVersion");
	if(at->pMpqGetVersion == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqGetVersion: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFMpqGetVersionString = (Function_SFMpqGetVersionString)GetProcAddress(hModule,"SFMpqGetVersionString");
	if(at->pSFMpqGetVersionString == NULL)
	{
		fprintf(stderr,"\nError locating address of SFMpqGetVersionString: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFMpqGetVersionString2 = (Function_SFMpqGetVersionString2)GetProcAddress(hModule,"SFMpqGetVersionString2");
	if(at->pSFMpqGetVersionString2 == NULL)
	{
		fprintf(stderr,"\nError locating address of SFMpqGetVersionString2: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFMpqGetVersion = (Function_SFMpqGetVersion)GetProcAddress(hModule,"SFMpqGetVersion");
	if(at->pSFMpqGetVersion == NULL)
	{
		fprintf(stderr,"\nError locating address of SFMpqGetVersion: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileOpenArchive = (Function_SFileOpenArchive)GetProcAddress(hModule,"SFileOpenArchive");
	if(at->pSFileOpenArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileOpenArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileCloseArchive = (Function_SFileCloseArchive)GetProcAddress(hModule,"SFileCloseArchive");
	if(at->pSFileCloseArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileCloseArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileGetArchiveName = (Function_SFileGetArchiveName)GetProcAddress(hModule,"SFileGetArchiveName");
	if(at->pSFileGetArchiveName == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileGetArchiveName: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileOpenFile = (Function_SFileOpenFile)GetProcAddress(hModule,"SFileOpenFile");
	if(at->pSFileOpenFile == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileOpenFile: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileOpenFileEx = (Function_SFileOpenFileEx)GetProcAddress(hModule,"SFileOpenFileEx");
	if(at->pSFileOpenFileEx == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileOpenFileEx: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileCloseFile = (Function_SFileCloseFile)GetProcAddress(hModule,"SFileCloseFile");
	if(at->pSFileCloseFile == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileCloseFile: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileGetFileSize = (Function_SFileGetFileSize)GetProcAddress(hModule,"SFileGetFileSize");
	if(at->pSFileGetFileSize == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileGetFileSize: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileGetFileArchive = (Function_SFileGetFileArchive)GetProcAddress(hModule,"SFileGetFileArchive");
	if(at->pSFileGetFileArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileGetFileArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileGetFileName = (Function_SFileGetFileName)GetProcAddress(hModule,"SFileGetFileName");
	if(at->pSFileGetFileName == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileGetFileName: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileSetFilePointer = (Function_SFileSetFilePointer)GetProcAddress(hModule,"SFileSetFilePointer");
	if(at->pSFileSetFilePointer == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileSetFilePointer: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileReadFile = (Function_SFileReadFile)GetProcAddress(hModule,"SFileReadFile");
	if(at->pSFileReadFile == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileReadFile: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileSetLocale = (Function_SFileSetLocale)GetProcAddress(hModule,"SFileSetLocale");
	if(at->pSFileSetLocale == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileSetLocale: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileGetBasePath = (Function_SFileGetBasePath)GetProcAddress(hModule,"SFileGetBasePath");
	if(at->pSFileGetBasePath == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileGetBasePath: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileSetBasePath = (Function_SFileSetBasePath)GetProcAddress(hModule,"SFileSetBasePath");
	if(at->pSFileSetBasePath == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileSetBasePath: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileGetFileInfo = (Function_SFileGetFileInfo)GetProcAddress(hModule,"SFileGetFileInfo");
	if(at->pSFileGetFileInfo == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileGetFileInfo: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileSetArchivePriority = (Function_SFileSetArchivePriority)GetProcAddress(hModule,"SFileSetArchivePriority");
	if(at->pSFileSetArchivePriority == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileSetArchivePriority: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileFindMpqHeader = (Function_SFileFindMpqHeader)GetProcAddress(hModule,"SFileFindMpqHeader");
	if(at->pSFileFindMpqHeader == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileFindMpqHeader: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pSFileListFiles = (Function_SFileListFiles)GetProcAddress(hModule,"SFileListFiles");
	if(at->pSFileListFiles == NULL)
	{
		fprintf(stderr,"\nError locating address of SFileListFiles: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqOpenArchiveForUpdate = (Function_MpqOpenArchiveForUpdate)GetProcAddress(hModule,"MpqOpenArchiveForUpdate");
	if(at->pMpqOpenArchiveForUpdate == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqOpenArchiveForUpdate: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqCloseUpdatedArchive = (Function_MpqCloseUpdatedArchive)GetProcAddress(hModule,"MpqCloseUpdatedArchive");
	if(at->pMpqCloseUpdatedArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqCloseUpdatedArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqAddFileToArchive = (Function_MpqAddFileToArchive)GetProcAddress(hModule,"MpqAddFileToArchive");
	if(at->pMpqAddFileToArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqAddFileToArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqAddWaveToArchive = (Function_MpqAddWaveToArchive)GetProcAddress(hModule,"MpqAddWaveToArchive");
	if(at->pMpqAddWaveToArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqAddWaveToArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqRenameFile = (Function_MpqRenameFile)GetProcAddress(hModule,"MpqRenameFile");
	if(at->pMpqRenameFile == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqRenameFile: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqDeleteFile = (Function_MpqDeleteFile)GetProcAddress(hModule,"MpqDeleteFile");
	if(at->pMpqDeleteFile == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqDeleteFile: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqCompactArchive = (Function_MpqCompactArchive)GetProcAddress(hModule,"MpqCompactArchive");
	if(at->pMpqCompactArchive == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqCompactArchive: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqAddFileToArchiveEx = (Function_MpqAddFileToArchiveEx)GetProcAddress(hModule,"MpqAddFileToArchiveEx");
	if(at->pMpqAddFileToArchiveEx == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqAddFileToArchiveEx: %s\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddFileFromBufferEx = (Function_MpqAddFileFromBufferEx)GetProcAddress(hModule,"MpqAddFileFromBufferEx");
	if(at->pMpqAddFileFromBufferEx == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqAddFileFromBufferEx: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqAddFileFromBuffer = (Function_MpqAddFileFromBuffer)GetProcAddress(hModule,"MpqAddFileFromBuffer");
	if(at->pMpqAddFileFromBuffer == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqAddFileFromBuffer: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqAddWaveFromBuffer = (Function_MpqAddWaveFromBuffer)GetProcAddress(hModule,"MpqAddWaveFromBuffer");
	if(at->pMpqAddWaveFromBuffer == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqAddWaveFromBuffer: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	at->pMpqSetFileLocale = (Function_MpqSetFileLocale)GetProcAddress(hModule,"MpqSetFileLocale");
	if(at->pMpqSetFileLocale == NULL)
	{
		fprintf(stderr,"\nError locating address of MpqSetFileLocale: %s\n", GetErrorText(GetLastError()));
		return FALSE;
	}
	return TRUE;
}

void ShowHelp(void)
{
	printf("Extracts compressed files from MoPAQ archives.\n\n");
	printf("MPQE_CPP [options] <MPQfile> [glob]\n\n");
	printf("Options:\n");
//	printf(" /p\t\tExtract newer files from patch MPQ archives if available\n");
	printf(" /d <directory>\tSet output directory ( default: MPQOUT )\n");
	printf(" /v\t\tEnable verbose output\n");
	printf(" /l\t\tUse lowercase filenames\n");
	printf(" /s\t\tSkip directory structure creation [all files go in outdir]\n");
}

void mpqExtract(const AddressTable* at, mpqeOptions* op, char* fileMPQ)
{
	unsigned int buffersize = 1024*1024; // start at 1 MB
	static bool firsttime = true;
	static char* extractedFiles = NULL;
	static char* filebuffer = NULL;
	static char* errbuf = NULL;
	if(firsttime)
	{
		errbuf = new char[ERRBUF_SIZE];
		extractedFiles = new char[buffersize];
		extractedFiles[0] = 0x00;
		firsttime = false;
	}
	int hMPQ = 0;
	int hFile = 0;
	printf("Extracting from %s\n",fileMPQ);
	if(at->pSFileOpenArchive(fileMPQ, 0, 0, &hMPQ) != 1)
	{
		fprintf(stderr,"Error: Could not open %s: %s\n", fileMPQ, GetErrorText(GetLastError()));
		return;
	}
	if(at->pSFileOpenFileEx(hMPQ, "(listfile)", 0, &hFile) != 1)
	{
		at->pSFileCloseArchive(hMPQ);
		fprintf(stderr,"Error: Could not find (listfile) in %s: %s\n", fileMPQ, GetErrorText(GetLastError()));
		return;
	}
	byte* buffer;
	byte* baselist;
	char* list;
	uint32 FileSize = 0, FileSizeHigh = 0;
	FileSize = at->pSFileGetFileSize(hFile, &FileSizeHigh);
	baselist = new byte[FileSize];
	uint32 FileRead;
	if(at->pSFileReadFile(hFile, baselist, (uint32)FileSize, &FileRead, NULL) != 1)
	{
		at->pSFileCloseFile(hFile);
		at->pSFileCloseArchive(hMPQ);
		fprintf(stderr,"Error: Could not read (listfile) in %s\n",fileMPQ);
		return;
	}
	at->pSFileCloseFile(hFile);
	list = (char*)baselist;
	char* file = NULL;
	char* context1 = NULL;
	char* dirpath = NULL;
	char* shortfname = NULL;
	size_t slen = 0;
	while((file=strchr(list,'\n')) != NULL)
	{
		*file = '\0';
		dirpath = list;
		list = file+1;
		file = dirpath;
		dirpath = NULL;
		if(strlen(file)==0)
			return;
		if(strchr(file,'\r')!=NULL)
			*strchr(file,'\r') = '\0';
		if(strstr(extractedFiles, file) != NULL) // already got this one, go to next
		{
			continue;
		}
		slen = strlen(file) + 8;
		if(dirpath != NULL)
		{
			delete [] dirpath;
		}
		dirpath = new char[slen];
		shortfname = dirpath;
		strcpy_s(dirpath, slen, file);
// remove the filename from the end, so only the directory path is left
		shortfname = strrchr(dirpath,'\\');
		if(shortfname == NULL)
		{
			shortfname = dirpath;
		}
		else
		{
			*shortfname = 0x00;
			shortfname++;
		}
		if(op->option_lowercase)
		{
			slen = strlen(dirpath);
			_strlwr_s(dirpath, slen);
			slen = strlen(shortfname);
			_strlwr_s(shortfname, slen);
			slen = strlen(file);
			_strlwr_s(file, slen);
		}
		if(op->option_searchglob != NULL && !Match(op->option_searchglob, file, false))
			continue;
		if(at->pSFileOpenFileEx(hMPQ, file, 0, &hFile) != 1)
		{
			fprintf(stderr,"Error: Could not find %s in %s\n", file, fileMPQ);
			continue;
		}
		FileSize = at->pSFileGetFileSize(hFile, &FileSizeHigh);
		if(FileSize == 0)
		{
			if (op->option_verbose)
				printf("Skipping: %s (NULL)\n", file);
			continue;
		}
		buffer = new byte[FileSize];
		if(at->pSFileReadFile(hFile, buffer, FileSize, &FileRead, NULL) != 1)
		{
			at->pSFileCloseFile(hFile);
			fprintf(stderr,"Error: Could not read %s in %s\n", file, fileMPQ);
			continue;
		}
		if(!op->option_skipdirstructure) // create directory structure for the current file
		{
			char* dname = NULL;
			slen = strlen(op->option_outdir) + strlen(dirpath) + 8;
			dname = new char [slen];
			sprintf_s(dname, slen, "%s\\%s", op->option_outdir, dirpath);
			char* dptr = dname;
			while(strchr(dptr,'\\') != NULL)
			{
				errno_t err;
				dptr = strchr(dptr,'\\');
				*dptr = 0x00;
				if((dptr > dname) && (*(dptr-1)==':'))
				{
					*dptr = '\\';
					dptr++;
					continue;
				}
				err = _mkdir(dname);
				if(err==-1 && errno!=17) // EEXIST
				{
					_strerror_s(errbuf, ERRBUF_SIZE, NULL);
					fprintf(stderr,"Error creating directory %s : %s\n", dname, errbuf);
					delete [] dname;
					return;
				}
				*dptr = '\\';
				dptr++;
			}
			errno_t err = _mkdir(dname);
			if(err==-1 && errno!=17) // EEXIST
			{
				_strerror_s(errbuf, ERRBUF_SIZE, NULL);
				fprintf(stderr,"Error creating directory %s : %s\n", dname, errbuf);
				delete [] dname;
				continue;
			}
			else if(errno!=17 && op->option_verbose)
			{
				printf("Created directory %s\n", dname);
			}
			delete [] dname;
		}
		else // create just the base output directory
		{
			char* dptr = op->option_outdir;
			while(strchr(dptr,'\\') != NULL)
			{
				errno_t err;
				dptr = strchr(dptr,'\\');
				*dptr = 0x00;
				if((dptr > op->option_outdir) && (*(dptr-1)==':'))
				{
					*dptr = '\\';
					dptr++;
					continue;
				}
				err = _mkdir(op->option_outdir);
				if(err==-1 && errno!=17) // EEXIST
				{
					_strerror_s(errbuf, ERRBUF_SIZE, NULL);
					fprintf(stderr,"Error creating directory %s : %s\n", op->option_outdir, errbuf);
					return;
				}
				*dptr = '\\';
				dptr++;
			}
			errno_t err = _mkdir(op->option_outdir);
			if(err==-1 && errno!=17) // EEXIST
			{
				_strerror_s(errbuf, ERRBUF_SIZE, NULL);
				fprintf(stderr,"Error creating directory %s : %s\n", op->option_outdir, errbuf);
				continue;
			}
			else if(errno!=17 && op->option_verbose)
			{
				printf("Created directory %s\n", op->option_outdir);
			}
		}
		FILE* fs = NULL;
		char* fname = NULL;
		slen = strlen(op->option_outdir) + strlen(file) + 2;
		if( fname != NULL )
		{
			delete [] fname;
		}
		fname = new char[slen];
		if(op->option_lowercase)
		{
			size_t l2 = strlen(op->option_outdir);
			_strlwr_s(op->option_outdir, l2);
			l2 = strlen(file);
			_strlwr_s(file, l2);
		}
		if(op->option_skipdirstructure) // file goes into option_outdir\\shortfilename
		{
			sprintf_s(fname, slen, "%s\\%s", op->option_outdir, shortfname);
		}
		else // file goes into option_outdir\\file [file contains directory structure]
		{
			sprintf_s(fname, slen, "%s\\%s", op->option_outdir, file);
		}
		errno_t err = fopen_s(&fs, fname, "wb");
		if(fs == NULL)
		{
			_strerror_s(errbuf, ERRBUF_SIZE, NULL);
			fprintf(stderr,"Error creating file %s : %s\n", fname, errbuf);
			continue;
		}
		fwrite(buffer, FileSize, 1, fs);
		fflush(fs);
		fclose(fs);
		if(op->option_verbose)
			printf("Extracted: %s (%s)\n", file, bytes2text(FileSize));
		delete [] buffer;
		if(extractedFiles[0]==0x00) // first element?
		{
			strncpy_s(extractedFiles, buffersize, file, strlen(file));
		}
		else
		{
			strcat_s(extractedFiles, buffersize, file);
		}
		strcat_s(extractedFiles, buffersize, "+"); // token
		if((strlen(extractedFiles) % buffersize) < 256) // nearing end of buffer, increase it
		{
			filebuffer = new char[buffersize + (1024*1024)]; // increase buffer by 1 MB
			memcpy(filebuffer, extractedFiles, buffersize);
			delete [] extractedFiles;
			extractedFiles = filebuffer;
			filebuffer = NULL;
			buffersize += (1024*1024);
		}
	}
	delete [] baselist;
	at->pSFileCloseArchive(hMPQ);
}

char* GetErrorText(DWORD err)
{
	static char* s = NULL;
	if(s != NULL)
	{
		LocalFree(s);
		s = NULL;
	}
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&s,0,NULL);
	return s;
}