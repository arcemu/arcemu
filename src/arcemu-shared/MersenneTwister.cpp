/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/> 
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

#include "MersenneTwister.h"
#include "Timer.h"

#define NUMBER_OF_GENERATORS 5
Mutex * m_locks[NUMBER_OF_GENERATORS];
CRandomMersenne * m_generators[NUMBER_OF_GENERATORS];
uint32 counter=0;

uint32 generate_seed()
{
	uint32 mstime = getMSTime();
	uint32 stime = (uint32)time(NULL);
	uint32 rnd[2];
	rnd[0] = rand()*rand()*rand();
	rnd[1] = rand()*rand()*rand();

	uint32 val = mstime ^ rnd[0];
	val += stime ^ rnd[1];
	return val;
}

void InitRandomNumberGenerators()
{
	srand(getMSTime());
	for(uint32 i = 0; i < NUMBER_OF_GENERATORS; ++i) {
		m_generators[i] = new CRandomMersenne(generate_seed());
		m_locks[i] = new Mutex();
	}
}

void CleanupRandomNumberGenerators()
{	srand(getMSTime());
	for(uint32 i = 0; i < NUMBER_OF_GENERATORS; ++i) {
		delete m_generators[i];
		delete m_locks[i];
	}
}

double RandomDouble()
{
	double ret;
	uint32 c;
  counter = 0; // added  cebernic: 10/06/2008 why not init ?
  // might be overflow couple days?
  for(;;)
	{
		c=counter%NUMBER_OF_GENERATORS;
		if(m_locks[c]->AttemptAcquire())
		{
			ret = m_generators[c]->Random();
			m_locks[c]->Release();
			return ret;
		}

		++counter;
	}
}

uint32 RandomUInt(uint32 n)
{
	uint32 ret;
	uint32 c;
  counter = 0;
  for(;;)
	{
		c=counter%NUMBER_OF_GENERATORS;
		if(m_locks[c]->AttemptAcquire())
		{
			ret = m_generators[c]->IRandom(0, n);
			m_locks[c]->Release();
			return ret;
		}

		++counter;
	}
}

double RandomDouble(double n)
{
	return RandomDouble() * n;
}

float RandomFloat(float n)
{
	return float(RandomDouble() * double(n));
}

float RandomFloat()
{
	return float(RandomDouble());
}

uint32 RandomUInt()
{
	uint32 ret;
	uint32 c;
  counter = 0;
	for(;;)
	{
		c=counter%NUMBER_OF_GENERATORS;
		if(m_locks[c]->AttemptAcquire())
		{
			ret = m_generators[c]->IRandom(0, RAND_MAX);
			m_locks[c]->Release();
			return ret;
		}

		++counter;
	}
}

//////////////////////////////////////////////////////////////////////////


void CRandomMersenne::Init0(uint32 seed) {
	// Detect computer architecture
	union {double f; uint32 i[2];} convert;
	convert.f = 1.0;
	if (convert.i[1] == 0x3FF00000) Architecture = LITTLE_ENDIAN1;
	else if (convert.i[0] == 0x3FF00000) Architecture = BIG_ENDIAN1;
	else Architecture = NONIEEE;

	// Seed generator
	mt[0]= seed;
	for (mti=1; mti < MERS_N; mti++) {
		mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
	}
}

void CRandomMersenne::RandomInit(uint32 seed) {
	// Initialize and seed
	Init0(seed);

	// Randomize some more
	for (int i = 0; i < 37; i++) BRandom();
}


void CRandomMersenne::RandomInitByArray(uint32 seeds[], int length) {
	// Seed by more than 32 bits
	int i, j, k;

	// Initialize
	Init0(19650218);

	if (length <= 0) return;

	// Randomize mt[] using whole seeds[] array
	i = 1;  j = 0;
	k = (MERS_N > length ? MERS_N : length);
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + seeds[j] + j;
		i++; j++;
		if (i >= MERS_N) {mt[0] = mt[MERS_N-1]; i=1;}
		if (j >= length) j=0;}
	for (k = MERS_N-1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - i;
		if (++i >= MERS_N) {mt[0] = mt[MERS_N-1]; i=1;}}
	mt[0] = 0x80000000UL;  // MSB is 1; assuring non-zero initial array

	// Randomize some more
	mti = 0;
	for (int i = 0; i <= MERS_N; i++) BRandom();
}


uint32 CRandomMersenne::BRandom() {
	// Generate 32 random bits
	uint32 y;

	if (mti >= MERS_N) {
		// Generate MERS_N words at one time
		const uint32 LOWER_MASK = (1LU << MERS_R) - 1;       // Lower MERS_R bits
		const uint32 UPPER_MASK = 0xFFFFFFFF << MERS_R;      // Upper (32 - MERS_R) bits
		static const uint32 mag01[2] = {0, MERS_A};

		int kk;
		for (kk=0; kk < MERS_N-MERS_M; kk++) {    
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk+MERS_M] ^ (y >> 1) ^ mag01[y & 1];}

		for (; kk < MERS_N-1; kk++) {    
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk+(MERS_M-MERS_N)] ^ (y >> 1) ^ mag01[y & 1];}      

		y = (mt[MERS_N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[MERS_N-1] = mt[MERS_M-1] ^ (y >> 1) ^ mag01[y & 1];
		mti = 0;
	}

	y = mt[mti++];

#if 1
	// Tempering (May be omitted):
	y ^=  y >> MERS_U;
	y ^= (y << MERS_S) & MERS_B;
	y ^= (y << MERS_T) & MERS_C;
	y ^=  y >> MERS_L;
#endif

	return y;
}


double CRandomMersenne::Random() {
	// Output random float number in the interval 0 <= x < 1
	union {double f; uint32 i[2];} convert;
	uint32 r = BRandom();               // Get 32 random bits
	// The fastest way to convert random bits to floating point is as follows:
	// Set the binary exponent of a floating point number to 1+bias and set
	// the mantissa to random bits. This will give a random number in the 
	// interval [1,2). Then subtract 1.0 to get a random number in the interval
	// [0,1). This procedure requires that we know how floating point numbers
	// are stored. The storing method is tested in function RandomInit and saved 
	// in the variable Architecture.

	// This shortcut allows the compiler to optimize away the following switch
	// statement for the most common architectures:
#if defined(_M_IX86) || defined(_M_X64) || defined(__LITTLE_ENDIAN__)
	Architecture = LITTLE_ENDIAN1;
#elif defined(__BIG_ENDIAN__)
	Architecture = BIG_ENDIAN1;
#endif

#ifdef USING_BIG_ENDIAN
	convert.i[1] =  r << 20;
	convert.i[0] = (r >> 12) | 0x3FF00000;
	return convert.f - 1.0;
#else
	convert.i[0] =  r << 20;
	convert.i[1] = (r >> 12) | 0x3FF00000;
	return convert.f - 1.0;
#endif
	// This somewhat slower method works for all architectures, including 
	// non-IEEE floating point representation:
	//return (double)r * (1./((double)(uint32)(-1L)+1.));
}


int CRandomMersenne::IRandom(int min, int max) {
	// Output random integer in the interval min <= x <= max
	// Relative error on frequencies < 2^-32
	if (max <= min) {
		if (max == min) return min; else return 0x80000000;
	}
	// Multiply interval with random and truncate
	int r = int((max - min + 1) * Random()) + min; 
	if (r > max) r = max;
	return r;
}


int CRandomMersenne::IRandomX(int min, int max) {
	// Output random integer in the interval min <= x <= max
	// Each output value has exactly the same probability.
	// This is obtained by rejecting certain bit values so that the number
	// of possible bit values is divisible by the interval length
	if (max <= min) {
		if (max == min) return min; else return 0x80000000;
	}

	// 64 bit integers available. Use multiply and shift method
	uint32 interval;                    // Length of interval
	uint64 longran;                     // Random bits * interval
	uint32 iran;                        // Longran / 2^32
	uint32 remainder;                   // Longran % 2^32

	interval = uint32(max - min + 1);
	if (interval != LastInterval) {
		// Interval length has changed. Must calculate rejection limit
		// Reject when remainder = 2^32 / interval * interval
		// RLimit will be 0 if interval is a power of 2. No rejection then
		RLimit = uint32(((uint64)1 << 32) / interval) * interval - 1;
		LastInterval = interval;
	}
	do { // Rejection loop
		longran  = (uint64)BRandom() * interval;
		iran = (uint32)(longran >> 32);
		remainder = (uint32)longran;
	} while (remainder > RLimit);
	// Convert back to signed and return result
	return (int32)iran + min;
}


