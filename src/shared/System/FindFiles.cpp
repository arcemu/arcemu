/*
 * ArcEmu MMORPG Server
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

#include "FindFiles.hpp"

#ifdef WIN32
#include "Windows.h"
#else
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#endif

namespace Arcemu
{
	void FileUtils::findFilesByExtension( const char* dirName, const char *extension, std::vector< std::string > &files )
	{
#ifdef WIN32
		HANDLE hFile;
		WIN32_FIND_DATA FindData;
		memset(&FindData, 0, sizeof(FindData));

		std::string searchName;

		searchName = dirName;
		searchName += "\\*.*";
	
		hFile = FindFirstFile(searchName.c_str(), &FindData);
		FindNextFile(hFile, &FindData);

		while(FindNextFile(hFile, &FindData))
		{
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				searchName = dirName;
				searchName += "\\";
				searchName += FindData.cFileName;
				findFilesByExtension(searchName.c_str(), extension, files);
			}
			else
			{
				std::string fname = dirName;
				fname += "\\";
				fname += FindData.cFileName;

				const char *fn = fname.c_str();
				const char *ext = strrchr( fn, '.' );
				if( ext != NULL )
				{
					ext++;

					if( _stricmp( ext, extension ) == 0 )
					{
						files.push_back( fname );
					}
				}
			}
		}
		FindClose(hFile);
#else
		const char* pch = strrchr(dirName, '/');
		if(strcmp(dirName, "..") == 0 || strcmp(dirName, ".") == 0) return; //Against Endless-Loop
		if(pch != NULL && (strcmp(pch, "/..") == 0 || strcmp(pch, "/.") == 0 || strcmp(pch, "/.svn") == 0)) return;
		struct dirent** list;
		int filecount = scandir(dirName, &list, 0, 0);

		if(filecount <= 0 || !list)
			return;

		struct stat attributes;
		bool err;
		while(filecount--)
		{
			char dottedrelpath[1024];
			sprintf(dottedrelpath, "%s/%s", dirName, list[filecount]->d_name);
			if(stat(dottedrelpath, &attributes) == -1)
			{
				err = true;
			}
			else err = false;

			if(!err && S_ISDIR(attributes.st_mode))
			{
				findFilesByExtension(dottedrelpath, extension, files);
			}
			else
			{
				char* ext = strrchr(list[filecount]->d_name, '.');
				if( ext != NULL )
				{
					ext++;

					if( strcasecmp(ext, extension) == 0 )
					{
						files.push_back(dottedrelpath);
					}
				}
			}

			free(list[filecount]);
		}
		free(list);
#endif
	}

#ifdef WIN32

	bool FileUtils::FindFilesByMask(const char* where, const char* filename, FindFilesResult & r)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;

		std::string fname(where);
		fname += "*";

		if(filename != NULL)
		{
			fname += filename;
			fname += "*";
		}

		hFind = FindFirstFile(fname.c_str(), &FindFileData);

		if(hFind == INVALID_HANDLE_VALUE)
			return false;

		do
		{
			r.Add(FindFileData.cFileName);
		}
		while(FindNextFile(hFind, &FindFileData));

		FindClose(hFind);

		return true;
	}

#else

	bool FileUtils::FindFilesByMask(const char* where, const char* filename, FindFilesResult & r)
	{
		int n = 0;
		dirent** filelist = NULL;

		n = scandir(where, &filelist, NULL, NULL);

		if(n < 0)
			return false;
		else
			while(n-- > 0)
			{
				if((filename == NULL) || (strstr(filename, "*") != NULL) || (strstr(filelist[ n ]->d_name, filename) != NULL))
					r.Add(filelist[ n ]->d_name);

				free(filelist[ n ]);
			}

		free(filelist);

		return true;
	}

#endif
}
