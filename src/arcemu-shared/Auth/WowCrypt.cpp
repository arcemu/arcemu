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
	_initialized = false;
	ciphers_setup();
}


void WowCrypt::Init(uint8 *K)
{
	//VLack: Andy, a thousand thanks for this initialization block below! Don't laugh please, I've kept the SARC4 code too :P
	HMAC_CTX send_hmac, recv_hmac;
	const uint8 s[16] = { 0xF4, 0x66, 0x31, 0x59, 0xFC, 0x83, 0x6E, 0x31, 0x31, 0x02, 0x51, 0xD5, 0x44, 0x31, 0x67, 0x98 };
	const uint8 r[16] = { 0x22, 0xBE, 0xE5, 0xCF, 0xBB, 0x07, 0x64, 0xD9, 0x00, 0x45, 0x1B, 0xD0, 0x24, 0xB8, 0xD5, 0x45 };
	HMAC_CTX_init(&send_hmac);
	HMAC_CTX_init(&recv_hmac);
	HMAC_Init_ex(&send_hmac, r, 16, EVP_sha1(), NULL);
	HMAC_Init_ex(&recv_hmac, s, 16, EVP_sha1(), NULL);
	HMAC_Update(&send_hmac, K, 40);
	HMAC_Update(&recv_hmac, K, 40);
	unsigned int length = 0;
	uint8 send_digest[20];
	uint8 recv_digest[20];
	HMAC_Final(&send_hmac, send_digest, &length);
	HMAC_Final(&recv_hmac, recv_digest, &length);

	cipher_init(&_Decrypt,(uint8*)recv_digest);
	cipher_init(&_Encrypt,(uint8*)send_digest);

	uint8 decryptRotateBuffer[1024];
	memset(decryptRotateBuffer, 0, 1024);
	cipher_update(&_Decrypt, 1024, (uint8*)decryptRotateBuffer);

	uint8 encryptRotateBuffer[1024];
	memset(encryptRotateBuffer, 0, 1024);
	cipher_update(&_Encrypt, 1024, (uint8*)encryptRotateBuffer);

	_initialized = true;
}


void WowCrypt::DecryptRecv(uint8 *data, size_t len)
{
	if (!_initialized) return;

	cipher_update(&_Decrypt, len, (uint8*)data);
}


void WowCrypt::EncryptSend(uint8 *data, size_t len)
{
	if (!_initialized) return;

	cipher_update(&_Encrypt, len, (uint8*)data);
}


WowCrypt::~WowCrypt()
{
	ciphers_cleanup();
}
