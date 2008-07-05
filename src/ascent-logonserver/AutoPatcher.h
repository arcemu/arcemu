/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef _AUTOPATCHER_H
#define _AUTOPATCHER_H

struct Patch
{
	uint32 FileSize;
	uint8 MD5[16];
	uint8 * Data;
	uint32 Version;
	char Locality[5];
	uint32 uLocality;
};

class PatchJob
{
	Patch * m_patchToSend;
	AuthSocket * m_client;
	uint32 m_bytesSent;
	uint32 m_bytesLeft;
	uint8 * m_dataPointer;

public:
	PatchJob(Patch * patch,AuthSocket* client,uint32 skip) : m_patchToSend(patch),m_client(client),m_bytesSent(skip),m_bytesLeft(patch->FileSize-skip),m_dataPointer(patch->Data+skip) {}
	ASCENT_INLINE AuthSocket * GetClient() { return m_client; }
	bool Update();
};

class PatchMgr : public Singleton<PatchMgr>
{
public:
	PatchMgr();
	~PatchMgr();

	Patch * FindPatchForClient(uint32 Version, const char * Locality);
	void BeginPatchJob(Patch * pPatch, AuthSocket * pClient, uint32 Skip);
	void UpdateJobs();
	void AbortPatchJob(PatchJob * pJob);
	bool InitiatePatch(Patch * pPatch, AuthSocket * pClient);

protected:
	vector<Patch*> m_patches;

	Mutex m_patchJobLock;
	list<PatchJob*> m_patchJobs;
};

#endif
