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

#include "MD5.h"

MD5Hash::MD5Hash()
{
	MD5_Init(&mC);
}

MD5Hash::~MD5Hash()
{

}

void MD5Hash::UpdateData(const std::string &str)
{
	UpdateData((const uint8*)str.data(), (int)str.length());
}

void MD5Hash::UpdateData(const uint8 *dta, int len)
{
	MD5_Update(&mC, dta, len);
}

void MD5Hash::Finalize()
{
	MD5_Final(mDigest, &mC);
}

void MD5Hash::Initialize()
{
	MD5_Init(&mC);
}

