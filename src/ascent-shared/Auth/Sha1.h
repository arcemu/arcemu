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
