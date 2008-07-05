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

#include "Sha1.h"
#include <stdarg.h>

Sha1Hash::Sha1Hash()
{
	SHA1_Init(&mC);
}

Sha1Hash::~Sha1Hash()
{
	// nothing.. lol
}

void Sha1Hash::UpdateData(const uint8 *dta, int len)
{
	SHA1_Update(&mC, dta, len);
}

void Sha1Hash::UpdateData(const std::string &str)
{
	UpdateData((uint8 *)str.c_str(), (int)str.length());
}

void Sha1Hash::UpdateBigNumbers(BigNumber *bn0, ...)
{
	va_list v;
	BigNumber *bn;

	va_start(v, bn0);
	bn = bn0;
	while (bn)
	{
		UpdateData(bn->AsByteArray(), bn->GetNumBytes());
		bn = va_arg(v, BigNumber *);
	}
	va_end(v);
}

void Sha1Hash::Initialize()
{
	SHA1_Init(&mC);
}

void Sha1Hash::Finalize(void)
{
	SHA1_Final(mDigest, &mC);
}
