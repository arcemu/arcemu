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
	op.option_usepatchfiles = false;
	op.option_verbose = false;
	op.option_outdir = NULL;
	op.option_lowercase = false;
	op.option_baseMPQ = NULL;
	op.option_searchglob = NULL;

	printf("MPQ-Extractor v%s by WRS", version);
	if(!LoadAddresses(&at))
	{
		return -1;
	}
	printf(" powered by %s\n", at.pMpqGetVersionString());
	printf("version: %f\n", at.pMpqGetVersion());
	printf("version: %s\n", at.pSFMpqGetVersionString());
	char test[64];
	printf("strlen: %d ",at.pSFMpqGetVersionString2(&test[0],63));
	printf("%s\n",test);
	SFMPQVERSION v = at.pSFMpqGetVersion();
	printf("version: %d.%d.%d.%d\n", v.Major, v.Minor, v.Revision, v.Subrevision);
	if(argc<2)
	{
		ShowHelp();
		return -1;
	}
	for(int i = 1; i < argc; ++i)
	{
		if(_stricmp(argv[i],"/p")==0) // use patch files if available
		{
			printf("Using patch MPQ archives if available: Enabled\n");
			op.option_usepatchfiles = true;
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
// worker
	FILE* tmp;
	errno_t err = fopen_s(&tmp, op.option_baseMPQ, "rb");
	if(tmp==NULL)
	{
		fprintf(stderr,"Fatal: Could not open MPQ archive %s Error: %lu\n", op.option_baseMPQ, err);
		return -1;
	}
	fclose(tmp);
	if(_strnicmp(op.option_baseMPQ,"patch",5)==0)
	{
		fprintf(stderr,"Error: %s is already a patch file, ignoring /p option\n", op.option_baseMPQ);
		op.option_usepatchfiles = false;
	}
	if(op.option_usepatchfiles)
	{
// TODO: add search for "patch*.MPQ" files [ FindFirstFile(), FindNextFile(), FindClose() ]
//		foreach(string patchfile in Directory.GetFiles(fi.DirectoryName, "patch*.MPQ"))
//			mpqExtract(&at, &op, patchfile);
		fprintf(stderr,"Error: patch file searching not yet implemented. Please specify a patch MPQ file on the command line.\n");
	}
	mpqExtract(&at, &op, op.option_baseMPQ);

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
	hModule = LoadLibrary(L"SFMPQ.DLL");
	if(hModule==NULL)
	{
		fprintf(stderr,"\nError %lu loading SFMPQ.DLL\n", GetLastError());
		return FALSE;
	}
	at->pMpqGetVersionString = (Function_MpqGetVersionString)GetProcAddress(hModule,"MpqGetVersionString");
	if(at->pMpqGetVersionString == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqGetVersionString.\n", GetLastError());
		return FALSE;
	}
	at->pMpqGetVersion = (Function_MpqGetVersion)GetProcAddress(hModule,"MpqGetVersion");
	if(at->pMpqGetVersion == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqGetVersion.\n", GetLastError());
		return FALSE;
	}
	at->pSFMpqGetVersionString = (Function_SFMpqGetVersionString)GetProcAddress(hModule,"SFMpqGetVersionString");
	if(at->pSFMpqGetVersionString == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFMpqGetVersionString.\n", GetLastError());
		return FALSE;
	}
	at->pSFMpqGetVersionString2 = (Function_SFMpqGetVersionString2)GetProcAddress(hModule,"SFMpqGetVersionString2");
	if(at->pSFMpqGetVersionString2 == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFMpqGetVersionString2.\n", GetLastError());
		return FALSE;
	}
	at->pSFMpqGetVersion = (Function_SFMpqGetVersion)GetProcAddress(hModule,"SFMpqGetVersion");
	if(at->pSFMpqGetVersion == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFMpqGetVersion.\n", GetLastError());
		return FALSE;
	}
	at->pSFileOpenArchive = (Function_SFileOpenArchive)GetProcAddress(hModule,"SFileOpenArchive");
	if(at->pSFileOpenArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileOpenArchive.\n", GetLastError());
		return FALSE;
	}
	at->pSFileCloseArchive = (Function_SFileCloseArchive)GetProcAddress(hModule,"SFileCloseArchive");
	if(at->pSFileCloseArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileCloseArchive.\n", GetLastError());
		return FALSE;
	}
	at->pSFileGetArchiveName = (Function_SFileGetArchiveName)GetProcAddress(hModule,"SFileGetArchiveName");
	if(at->pSFileGetArchiveName == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileGetArchiveName.\n", GetLastError());
		return FALSE;
	}
	at->pSFileOpenFile = (Function_SFileOpenFile)GetProcAddress(hModule,"SFileOpenFile");
	if(at->pSFileOpenFile == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileOpenFile.\n", GetLastError());
		return FALSE;
	}
	at->pSFileOpenFileEx = (Function_SFileOpenFileEx)GetProcAddress(hModule,"SFileOpenFileEx");
	if(at->pSFileOpenFileEx == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileOpenFileEx.\n", GetLastError());
		return FALSE;
	}
	at->pSFileCloseFile = (Function_SFileCloseFile)GetProcAddress(hModule,"SFileCloseFile");
	if(at->pSFileCloseFile == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileCloseFile.\n", GetLastError());
		return FALSE;
	}
	at->pSFileGetFileSize = (Function_SFileGetFileSize)GetProcAddress(hModule,"SFileGetFileSize");
	if(at->pSFileGetFileSize == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileGetFileSize.\n", GetLastError());
		return FALSE;
	}
	at->pSFileGetFileArchive = (Function_SFileGetFileArchive)GetProcAddress(hModule,"SFileGetFileArchive");
	if(at->pSFileGetFileArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileGetFileArchive.\n", GetLastError());
		return FALSE;
	}
	at->pSFileGetFileName = (Function_SFileGetFileName)GetProcAddress(hModule,"SFileGetFileName");
	if(at->pSFileGetFileName == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileGetFileName.\n", GetLastError());
		return FALSE;
	}
	at->pSFileSetFilePointer = (Function_SFileSetFilePointer)GetProcAddress(hModule,"SFileSetFilePointer");
	if(at->pSFileSetFilePointer == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileSetFilePointer.\n", GetLastError());
		return FALSE;
	}
	at->pSFileReadFile = (Function_SFileReadFile)GetProcAddress(hModule,"SFileReadFile");
	if(at->pSFileReadFile == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileReadFile.\n", GetLastError());
		return FALSE;
	}
	at->pSFileSetLocale = (Function_SFileSetLocale)GetProcAddress(hModule,"SFileSetLocale");
	if(at->pSFileSetLocale == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileSetLocale.\n", GetLastError());
		return FALSE;
	}
	at->pSFileGetBasePath = (Function_SFileGetBasePath)GetProcAddress(hModule,"SFileGetBasePath");
	if(at->pSFileGetBasePath == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileGetBasePath.\n", GetLastError());
		return FALSE;
	}
	at->pSFileSetBasePath = (Function_SFileSetBasePath)GetProcAddress(hModule,"SFileSetBasePath");
	if(at->pSFileSetBasePath == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileSetBasePath.\n", GetLastError());
		return FALSE;
	}
	at->pSFileGetFileInfo = (Function_SFileGetFileInfo)GetProcAddress(hModule,"SFileGetFileInfo");
	if(at->pSFileGetFileInfo == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileGetFileInfo.\n", GetLastError());
		return FALSE;
	}
	at->pSFileSetArchivePriority = (Function_SFileSetArchivePriority)GetProcAddress(hModule,"SFileSetArchivePriority");
	if(at->pSFileSetArchivePriority == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileSetArchivePriority.\n", GetLastError());
		return FALSE;
	}
	at->pSFileFindMpqHeader = (Function_SFileFindMpqHeader)GetProcAddress(hModule,"SFileFindMpqHeader");
	if(at->pSFileFindMpqHeader == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileFindMpqHeader.\n", GetLastError());
		return FALSE;
	}
	at->pSFileListFiles = (Function_SFileListFiles)GetProcAddress(hModule,"SFileListFiles");
	if(at->pSFileListFiles == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of SFileListFiles.\n", GetLastError());
		return FALSE;
	}
	at->pMpqOpenArchiveForUpdate = (Function_MpqOpenArchiveForUpdate)GetProcAddress(hModule,"MpqOpenArchiveForUpdate");
	if(at->pMpqOpenArchiveForUpdate == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqOpenArchiveForUpdate.\n", GetLastError());
		return FALSE;
	}
	at->pMpqCloseUpdatedArchive = (Function_MpqCloseUpdatedArchive)GetProcAddress(hModule,"MpqCloseUpdatedArchive");
	if(at->pMpqCloseUpdatedArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqCloseUpdatedArchive.\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddFileToArchive = (Function_MpqAddFileToArchive)GetProcAddress(hModule,"MpqAddFileToArchive");
	if(at->pMpqAddFileToArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqAddFileToArchive.\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddWaveToArchive = (Function_MpqAddWaveToArchive)GetProcAddress(hModule,"MpqAddWaveToArchive");
	if(at->pMpqAddWaveToArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqAddWaveToArchive.\n", GetLastError());
		return FALSE;
	}
	at->pMpqRenameFile = (Function_MpqRenameFile)GetProcAddress(hModule,"MpqRenameFile");
	if(at->pMpqRenameFile == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqRenameFile.\n", GetLastError());
		return FALSE;
	}
	at->pMpqDeleteFile = (Function_MpqDeleteFile)GetProcAddress(hModule,"MpqDeleteFile");
	if(at->pMpqDeleteFile == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqDeleteFile.\n", GetLastError());
		return FALSE;
	}
	at->pMpqCompactArchive = (Function_MpqCompactArchive)GetProcAddress(hModule,"MpqCompactArchive");
	if(at->pMpqCompactArchive == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqCompactArchive.\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddFileToArchiveEx = (Function_MpqAddFileToArchiveEx)GetProcAddress(hModule,"MpqAddFileToArchiveEx");
	if(at->pMpqAddFileToArchiveEx == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqAddFileToArchiveEx.\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddFileFromBufferEx = (Function_MpqAddFileFromBufferEx)GetProcAddress(hModule,"MpqAddFileFromBufferEx");
	if(at->pMpqAddFileFromBufferEx == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqAddFileFromBufferEx.\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddFileFromBuffer = (Function_MpqAddFileFromBuffer)GetProcAddress(hModule,"MpqAddFileFromBuffer");
	if(at->pMpqAddFileFromBuffer == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqAddFileFromBuffer.\n", GetLastError());
		return FALSE;
	}
	at->pMpqAddWaveFromBuffer = (Function_MpqAddWaveFromBuffer)GetProcAddress(hModule,"MpqAddWaveFromBuffer");
	if(at->pMpqAddWaveFromBuffer == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqAddWaveFromBuffer.\n", GetLastError());
		return FALSE;
	}
	at->pMpqSetFileLocale = (Function_MpqSetFileLocale)GetProcAddress(hModule,"MpqSetFileLocale");
	if(at->pMpqSetFileLocale == NULL)
	{
		fprintf(stderr,"\nError %lu locating address of MpqSetFileLocale.\n", GetLastError());
		return FALSE;
	}
	return TRUE;
}

void ShowHelp(void)
{
	printf("Extracts compressed files from MoPAQ archives.\n\n");
	printf("MPQE [options] <MPQfile> [glob]\n\n");
	printf("Options:\n");
	printf(" /p\t\tExtract newer files from patch MPQ archives if available\n");
	printf(" /d <directory>\tSet output directory ( default: MPQOUT )\n");
	printf(" /v\t\tEnable verbose output\n");
	printf(" /l\t\tUse lowercase filenames\n");
}

void mpqExtract(const AddressTable* at, mpqeOptions* op, char* fileMPQ)
{
	static char* extractedFiles = new char[1024*1024];
	int hMPQ = 0;
	int hFile = 0;
	printf("Extracting from %s\n",fileMPQ);
	if(at->pSFileOpenArchive(fileMPQ, 0, 0, &hMPQ) != 1)
	{
		fprintf(stderr,"Error: Could not open %s\n", fileMPQ);
		return;
	}
	if(at->pSFileOpenFileEx(hMPQ, "(listfile)", 0, &hFile) != 1)
	{
		at->pSFileCloseArchive(hMPQ);
		fprintf(stderr,"Error: Could not find (listfile) in %s\n", fileMPQ);
		return;
	}
	byte* buffer;
	uint32 FileSize = 0, FileSizeHigh = 0;
	FileSize = at->pSFileGetFileSize(hFile, &FileSizeHigh);
	buffer = new byte[FileSize];
	uint32 FileRead;
	if(at->pSFileReadFile(hFile, buffer, (uint32)FileSize, &FileRead, NULL) != 1)
	{
		at->pSFileCloseFile(hFile);
		at->pSFileCloseArchive(hMPQ);
		fprintf(stderr,"Error: Could not read (listfile) in %s\n",fileMPQ);
		return;
	}
	at->pSFileCloseFile(hFile);
	char* file;
	char* list = (char*)buffer;
	char* context1 = NULL;
	char* dirpath = NULL;
	size_t slen;
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
		if(strstr(extractedFiles,file) != NULL)
			continue;
		slen = strlen(file) + 8;
		if(dirpath != NULL)
		{
			delete [] dirpath;
		}
		dirpath = new char[slen];
		strcpy_s(dirpath, slen, file);
// remove the filename from the end, so only the directory path is left
		_strrev(dirpath);
		if(strchr(dirpath,'\\') != NULL)
		{
			*strchr(dirpath,'\\') = '\0';
		}
		_strrev(dirpath);
		if(op->option_lowercase)
		{
			slen = strlen(dirpath);
			_strlwr_s(dirpath, slen);
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
		char* dname = NULL;
		slen = strlen(op->option_outdir) + strlen(dirpath) + 8;
		dname = new char [slen];
		sprintf_s(dname, slen, "%s\\%s", op->option_outdir, dirpath);
		_mkdir(dname);
		delete [] dname;
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
		sprintf_s(fname, slen, "%s\\%s", op->option_outdir, file);
		errno_t err = fopen_s(&fs, fname, "wb");
		if(fs == NULL)
		{
			fprintf(stderr,"Error creating file %s\n", file);
			continue;
		}
		fwrite(buffer, FileSize, 1, fs);
		fflush(fs);
		fclose(fs);
		if(op->option_verbose)
			printf("Extracted: %s (%s)\n", file, bytes2text(FileSize));

		strncpy_s(extractedFiles, 1024*1024, file, strlen(file));
		strcat_s(extractedFiles, 1024*1024, "+"); // token
	}
	at->pSFileCloseArchive(hMPQ);
}

