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

#ifndef _AUTH_BIGNUMBER_H
#define _AUTH_BIGNUMBER_H

#include "Common.h"
#include "ByteBuffer.h"

//#include "openssl/bn.h"
struct bignum_st;

class BigNumber
{
	public:
		BigNumber();
		BigNumber(const BigNumber &bn);
		BigNumber(uint32);
		~BigNumber();

		void SetDword(uint32);
		void SetQword(uint64);
		void SetBinary(const uint8 *bytes, int len);
		void SetHexStr(const char *str);

		void SetRand(int numbits);

		BigNumber operator=(const BigNumber &bn);
//	  BigNumber operator=(Sha1Hash &hash);

		BigNumber operator+=(const BigNumber &bn);
		BigNumber operator+(const BigNumber &bn) {
			BigNumber t(*this);
			return t += bn;
		}
		BigNumber operator-=(const BigNumber &bn);
		BigNumber operator-(const BigNumber &bn) {
			BigNumber t(*this);
			return t -= bn;
		}
		BigNumber operator*=(const BigNumber &bn);
		BigNumber operator*(const BigNumber &bn) {
			BigNumber t(*this);
			return t *= bn;
		}
		BigNumber operator/=(const BigNumber &bn);
		BigNumber operator/(const BigNumber &bn) {
			BigNumber t(*this);
			return t /= bn;
		}
		BigNumber operator%=(const BigNumber &bn);
		BigNumber operator%(const BigNumber &bn) {
			BigNumber t(*this);
			return t %= bn;
		}

		BigNumber ModExp(const BigNumber &bn1, const BigNumber &bn2);
		BigNumber Exp(const BigNumber &);

		int GetNumBytes(void);

		struct bignum_st *BN() { return _bn; }

		uint32 AsDword();
		uint8* AsByteArray();
		ByteBuffer AsByteBuffer();
		std::vector<uint8> AsByteVector();

		const char *AsHexStr();
		const char *AsDecStr();

	private:
		struct bignum_st *_bn;
		uint8 *_array;
};

#endif
