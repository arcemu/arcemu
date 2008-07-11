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

#include "LogonStdAfx.h"
#include "../arcemu-shared/Auth/MD5.h"

#ifndef WIN32
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

initialiseSingleton(PatchMgr);
PatchMgr::PatchMgr()
{
	// load patches
#ifdef WIN32
	Log.Notice("PatchMgr", "Loading Patches...");
	char Buffer[MAX_PATH*10];
	char Buffer2[MAX_PATH*10];
	char Buffer3[MAX_PATH*10];

	WIN32_FIND_DATA fd;
	HANDLE fHandle;
	MD5Hash md5;
	Patch * pPatch;
	DWORD size,sizehigh;
	HANDLE hFile;
	uint32 srcversion;
	char locality[5];
	uint32 i;

	if(!GetCurrentDirectory(MAX_PATH*10, Buffer))
		return;

	strcpy(Buffer2,Buffer);
	strcat(Buffer, "\\ClientPatches\\*.*");
	fHandle = FindFirstFile(Buffer, &fd);
	if(fHandle == INVALID_HANDLE_VALUE)
		return;

	do 
	{
		snprintf(Buffer3,MAX_PATH*10,"%s\\ClientPatches\\%s",Buffer2,fd.cFileName);
		if(sscanf(fd.cFileName,"%4s%u.", locality, &srcversion) != 2)
			continue;

		hFile = CreateFile(Buffer3, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			continue;

		Log.Notice("PatchMgr", "Found patch for b%u locale `%s`.", srcversion,locality);
		pPatch = new Patch;
		size = GetFileSize(hFile, &sizehigh);
		pPatch->FileSize = size;
		pPatch->Data = new uint8[size];
		pPatch->Version = srcversion;
		for(i = 0; i < 4; ++i)
			pPatch->Locality[i] = tolower(locality[i]);
		pPatch->Locality[4] = 0;
		pPatch->uLocality = *(uint32*)pPatch->Locality;

		if(pPatch->Data==NULL)
		{
			// shouldn't really happen
			delete pPatch;
			CloseHandle(hFile);
			continue;
		}

		// read the whole file
		ASSERT(ReadFile(hFile, pPatch->Data, pPatch->FileSize, &size, NULL));
		ASSERT(size == pPatch->FileSize);

		// close the handle, no longer needed
		CloseHandle(hFile);

		// md5hash the file
		md5.Initialize();
		md5.UpdateData(pPatch->Data, pPatch->FileSize);
		md5.Finalize();
		memcpy(pPatch->MD5, md5.GetDigest(), MD5_DIGEST_LENGTH);
		
		// add the patch to the patchlist
		m_patches.push_back(pPatch);

	} while(FindNextFile(fHandle,&fd));
	FindClose(fHandle);
#else
	/* 
	 *nix patch loader
	 */
	Log.Notice("PatchMgr", "Loading Patches...");
	char Buffer[MAX_PATH*10];
	char Buffer2[MAX_PATH*10];
	char Buffer3[MAX_PATH*10];

	struct dirent ** list;
	int filecount;
	int read_fd;
	MD5Hash md5;
	Patch * pPatch;
	int size;
	uint32 srcversion;
	char locality[5];
	uint32 i;
	struct stat sb;

	strcpy(Buffer, "./ClientPatches");
	strcpy(Buffer2,Buffer);

	filecount = scandir("./ClientPatches", &list, 0, 0);
	if(filecount <= 0 || list==NULL)
	{
		Log.Error("PatchMgr", "No patches found.");
		return;
	}

	while(filecount--)
	{
		snprintf(Buffer3,MAX_PATH*10,"./ClientPatches/%s",list[filecount]->d_name);
		if(sscanf(list[filecount]->d_name,"%4s%u.", locality, &srcversion) != 2)
			continue;

		read_fd = open(Buffer3, O_RDONLY);
		if(read_fd <= 0)
		{
			printf("Cannot open %s\n", Buffer3);
			continue;
		}

		if(fstat(read_fd, &sb) < 0)
		{
			printf("Cannot stat %s\n", Buffer3);
			continue;
		}

		Log.Notice("PatchMgr", "Found patch for b%u locale `%s` (%u bytes).", srcversion,locality, sb.st_size);
		pPatch = new Patch;
		size = sb.st_size;
		pPatch->FileSize = size;
		pPatch->Data = new uint8[size];
		pPatch->Version = srcversion;
		for(i = 0; i < 4; ++i)
			pPatch->Locality[i] = tolower(locality[i]);
		pPatch->Locality[4] = 0;
		pPatch->uLocality = *(uint32*)pPatch->Locality;

		if(pPatch->Data==NULL)
		{
			// shouldn't really happen
			delete pPatch;
			close(read_fd);
			continue;
		}

		// read the whole file
		ASSERT(read(read_fd, pPatch->Data, size) == size);

		// close handle
		close(read_fd);

		// md5hash the file
		md5.Initialize();
		md5.UpdateData(pPatch->Data, pPatch->FileSize);
		md5.Finalize();
		memcpy(pPatch->MD5, md5.GetDigest(), MD5_DIGEST_LENGTH);

		// add the patch to the patchlist
		m_patches.push_back(pPatch);
		free(list[filecount]);
	}
	free(list);
#endif
}

PatchMgr::~PatchMgr()
{

}

Patch * PatchMgr::FindPatchForClient(uint32 Version, const char * Locality)
{
	char tmplocality[5];
	uint32 ulocality;
	uint32 i;
	vector<Patch*>::iterator itr;
	Patch * fallbackPatch = NULL;
	for(i = 0; i < 4; ++i)
		tmplocality[i]=tolower(Locality[i]);
	tmplocality[4]=0;
	ulocality = *(uint32*)tmplocality;

	for(itr = m_patches.begin(); itr != m_patches.end(); ++itr)
	{
		// since localities are always 4 bytes we can do a simple int compare,
		// saving a string compare ;)
		if((*itr)->uLocality==ulocality)
		{
			if(fallbackPatch==NULL && (*itr)->Version==0)
				fallbackPatch = (*itr);
			
			if((*itr)->Version == Version)
				return (*itr);
		}
	}

	return fallbackPatch;
}

void PatchMgr::BeginPatchJob(Patch * pPatch, AuthSocket * pClient, uint32 Skip)
{
	PatchJob * pJob;

	pJob = new PatchJob(pPatch,pClient,Skip);
	pClient->m_patchJob=pJob;
	m_patchJobLock.Acquire();
	m_patchJobs.push_back(pJob);
	m_patchJobLock.Release();
}

void PatchMgr::UpdateJobs()
{
	list<PatchJob*>::iterator itr, itr2;
	m_patchJobLock.Acquire();
	for(itr = m_patchJobs.begin(); itr != m_patchJobs.end();)
	{
		itr2 = itr;
		++itr;

		if(!(*itr2)->Update())
		{
			(*itr2)->GetClient()->m_patchJob=NULL;
			delete (*itr2);
			m_patchJobs.erase(itr2);
		}
	}
	m_patchJobLock.Release();
}

void PatchMgr::AbortPatchJob(PatchJob * pJob)
{
	list<PatchJob*>::iterator itr;
	m_patchJobLock.Acquire();
	for(itr = m_patchJobs.begin(); itr != m_patchJobs.end(); ++itr)
	{
		if((*itr)==pJob)
		{
			m_patchJobs.erase(itr);
			break;
		}
	}
	delete pJob;
	m_patchJobLock.Release();
}

// this is what blizz sends.
// Data (1412 bytes)
// Data (91 bytes)
// 1412+91=1503 (minus header bytes, 3) = 1500

#define TRANSFER_CHUNK_SIZE 1500

#pragma pack(push,1)

struct TransferInitiatePacket
{
	uint8 cmd;
	uint8 strsize;
	char name[5];
	uint64 filesize;
	uint8 md5hash[MD5_DIGEST_LENGTH];
};

struct TransferDataPacket
{
	uint8 cmd;
	uint16 chunk_size;
};

#pragma pack(pop)

bool PatchJob::Update()
{
	// don't update unless the write buffer is empty
	m_client->BurstBegin();
	if(m_client->GetWriteBuffer().GetSize()!=0)
	{
		m_client->BurstEnd();
		return true;
	}

	// send 1500 byte chunks
	TransferDataPacket header;
	bool result;
	header.cmd = 0x31;
	header.chunk_size = (m_bytesLeft>1500)?1500:m_bytesLeft;
	//Log.Debug("PatchJob", "Sending %u byte chunk", header.chunk_size);

	result = m_client->BurstSend((const uint8*)&header,sizeof(TransferDataPacket));
	if(result)
	{
		result = m_client->BurstSend(m_dataPointer, header.chunk_size);
		if(result)
		{
			m_dataPointer += header.chunk_size;
			m_bytesSent += header.chunk_size;
			m_bytesLeft -= header.chunk_size;
		}
	}

	if(result)
		m_client->BurstPush();

	m_client->BurstEnd();

	// no need to check the result here, could just be a full buffer and not necessarily a fatal error.
	return (m_bytesLeft>0)?true:false;
}

bool PatchMgr::InitiatePatch(Patch * pPatch, AuthSocket * pClient)
{
	// send initiate packet
	TransferInitiatePacket init;
	bool result;

	init.cmd = 0x30;
	init.strsize=5;
	init.name[0] = 'P'; init.name[1] = 'a'; init.name[2] = 't'; init.name[3] = 'c'; init.name[4] = 'h'; init.name[5] = 0;
	init.filesize = pPatch->FileSize;
	memcpy(init.md5hash, pPatch->MD5, MD5_DIGEST_LENGTH);

	// send it to the client
	pClient->BurstBegin();
	result = pClient->BurstSend((const uint8*)&init,sizeof(TransferInitiatePacket));
	if(result)
		pClient->BurstPush();
	pClient->BurstEnd();
	return result;
}
