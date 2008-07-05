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

#ifndef _AUTH_SHA1_H
#define _AUTH_SHA1_H

#include <stdlib.h>
#include "Common.h"
#include <openssl/sha.h>
#include "Auth/BigNumber.h"

class Sha1Hash
{
	public:
		Sha1Hash();
		~Sha1Hash();

		void UpdateFinalizeBigNumbers(BigNumber *bn0, ...);
		void UpdateBigNumbers(BigNumber *bn0, ...);

		void UpdateData(const uint8 *dta, int len);
		void UpdateData(const std::string &str);

		void Initialize();
		void Finalize();

		uint8 *GetDigest(void) { return mDigest; };
		int GetLength(void) { return SHA_DIGEST_LENGTH; };

		BigNumber GetBigNumber();

	private:
		SHA_CTX mC;
		uint8 mDigest[SHA_DIGEST_LENGTH];
};

#endif
