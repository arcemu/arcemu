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

#include "WowCrypt.h"

#include <algorithm>

#include <openssl/hmac.h>


WowCrypt::WowCrypt()
{
	m_initialized = false;
}


void WowCrypt::Init(uint8* K)
{
	static const uint8 s[16] = { 0xC2, 0xB3, 0x72, 0x3C, 0xC6, 0xAE, 0xD9, 0xB5, 0x34, 0x3C, 0x53, 0xEE, 0x2F, 0x43, 0x67, 0xCE };
	static const uint8 r[16] = { 0xCC, 0x98, 0xAE, 0x04, 0xE8, 0x97, 0xEA, 0xCA, 0x12, 0xDD, 0xC0, 0x93, 0x42, 0x91, 0x53, 0x57 };
	uint8 encryptHash[SHA_DIGEST_LENGTH];
	uint8 decryptHash[SHA_DIGEST_LENGTH];
	uint8 pass[1024];
	uint32 md_len;

	// generate c->s key
	HMAC(EVP_sha1(), s, 16, K, 40, decryptHash, &md_len);
	assert(md_len == SHA_DIGEST_LENGTH);

	// generate s->c key
	HMAC(EVP_sha1(), r, 16, K, 40, encryptHash, &md_len);
	assert(md_len == SHA_DIGEST_LENGTH);

	// initialize rc4 structs
	RC4_set_key(&m_clientDecrypt, SHA_DIGEST_LENGTH, decryptHash);
	RC4_set_key(&m_serverEncrypt, SHA_DIGEST_LENGTH, encryptHash);

	// initial encryption pass -- this is just to get key position,
	// the data doesn't actually have to be initialized as discovered
	// by client debugging.
	RC4(&m_serverEncrypt, 1024, pass, pass);
	RC4(&m_clientDecrypt, 1024, pass, pass);
	m_initialized = true;
}

WowCrypt::~WowCrypt()
{

}
