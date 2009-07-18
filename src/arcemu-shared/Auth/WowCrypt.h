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

#ifndef _WOWCRYPT_H
#define _WOWCRYPT_H

#include <stdlib.h>
#include "../Common.h"
#include "BigNumber.h"
#include <vector>
//#include "Auth/Sha1.h"
#include <openssl/evp.h>
#include <openssl/sha.h>

class WowCrypt {
public:
	WowCrypt();
	~WowCrypt();

	void Init(uint8 *K);
	void DecryptRecv(uint8 * data, size_t len);
	void EncryptSend(uint8 * data, size_t len);

	bool IsInitialized() { return _initialized; }

private:
	ARCEMU_INLINE void ciphers_setup()
	{
		EVP_CIPHER_CTX_init(&_Decrypt);
		EVP_EncryptInit_ex(&_Decrypt, EVP_rc4(), NULL, NULL, NULL);
		EVP_CIPHER_CTX_set_key_length(&_Decrypt, SHA_DIGEST_LENGTH);
		//----------------------------------------------------------
		EVP_CIPHER_CTX_init(&_Encrypt);
		EVP_EncryptInit_ex(&_Encrypt, EVP_rc4(), NULL, NULL, NULL);
		EVP_CIPHER_CTX_set_key_length(&_Encrypt, SHA_DIGEST_LENGTH);
	}

	ARCEMU_INLINE void ciphers_cleanup()
	{
		EVP_CIPHER_CTX_cleanup(&_Decrypt);
		EVP_CIPHER_CTX_cleanup(&_Encrypt);
	}

	ARCEMU_INLINE void cipher_init(EVP_CIPHER_CTX *c, uint8 *seed)
	{
		EVP_EncryptInit_ex(c, NULL, NULL, seed, NULL);
	}

	ARCEMU_INLINE void cipher_update(EVP_CIPHER_CTX *c, int len, uint8 *data)
	{
		int outlen = 0;
		EVP_EncryptUpdate(c, data, &outlen, data, len);
		EVP_EncryptFinal_ex(c, data, &outlen);
	}

	bool _initialized;
	EVP_CIPHER_CTX _Decrypt;
	EVP_CIPHER_CTX _Encrypt;
};

#endif
