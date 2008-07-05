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

#ifndef _MD5_H
#define _MD5_H

#include <stdlib.h>
#include <openssl/md5.h>
#include "Common.h"

class MD5Hash
{
public:
	MD5Hash();
	~MD5Hash();

	void UpdateData(const uint8 *dta, int len);
	void UpdateData(const std::string &str);

	void Initialize();
	void Finalize();

	uint8 *GetDigest(void) { return mDigest; };
	int GetLength(void) { return MD5_DIGEST_LENGTH; };

private:
	MD5_CTX mC;
	uint8 mDigest[MD5_DIGEST_LENGTH];
};

#endif
