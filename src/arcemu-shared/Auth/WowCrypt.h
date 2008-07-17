/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
#include <vector>
#include "Auth/Sha1.h"

class WowCrypt {
public:
	WowCrypt();
	~WowCrypt();

	const static size_t CRYPTED_SEND_LEN = 4;
	const static size_t CRYPTED_RECV_LEN = 6;

	void Init();

	void SetKey(uint8 *, size_t);

	void DecryptRecv(uint8 *, size_t);
	void EncryptSend(uint8 *, size_t);
    
    // encrypt 4 bytes
    void EncryptFourSend(uint8 * data);
    // decrypt 6 bytes
    void DecryptSixRecv(uint8 *data);
	
	// 2.4.3 new key generation procedure
	static void GenerateKey(uint8 *, uint8 *);

	bool IsInitialized() { return _initialized; }

private:
	std::vector<uint8> _key;
	uint8 _send_i, _send_j, _recv_i, _recv_j;
	bool _initialized;
};

#endif
