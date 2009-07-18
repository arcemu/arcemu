/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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


void WowCrypt::Init(uint8 *K)
{
	static const uint8 s[16] = { 0xF4, 0x66, 0x31, 0x59, 0xFC, 0x83, 0x6E, 0x31, 0x31, 0x02, 0x51, 0xD5, 0x44, 0x31, 0x67, 0x98 };
	static const uint8 r[16] = { 0x22, 0xBE, 0xE5, 0xCF, 0xBB, 0x07, 0x64, 0xD9, 0x00, 0x45, 0x1B, 0xD0, 0x24, 0xB8, 0xD5, 0x45 };
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
