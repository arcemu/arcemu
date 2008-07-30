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

#ifndef _RANDOMGEN_H
#define _RANDOMGEN_H

#include "Common.h"
#include "Singleton.h"

SERVER_DECL void InitRandomNumberGenerators();
SERVER_DECL void ReseedRandomNumberGenerators();
SERVER_DECL void CleanupRandomNumberGenerators();
SERVER_DECL double RandomDouble();
SERVER_DECL double RandomDouble(double n);
SERVER_DECL float RandomFloat();
SERVER_DECL float RandomFloat(float n);
SERVER_DECL uint32 RandomUInt();
SERVER_DECL uint32 RandomUInt(uint32 n);

/*************************** RANDOMC.H ***************** 2007-09-22 Agner Fog *
*
* This file contains class declarations and other definitions for the C++ 
* library of uniform random number generators.
*
* Overview of classes:
* ====================
*
* class CRandomMersenne:
* Random number generator of type Mersenne twister.
* Source file mersenne.cpp
*
* class CRandomMother:
* Random number generator of type Mother-of-All (Multiply with carry).
* Source file mother.cpp
*
*
* Member functions (methods):
* ===========================
*
* All these classes have identical member functions:
*
* Constructor(uint32 seed):
* The seed can be any integer. Usually the time is used as seed.
* Executing a program twice with the same seed will give the same sequence of
* random numbers. A different seed will give a different sequence.
*
* void RandomInit(uint32 seed);
* Re-initializes the random number generator with a new seed.
*
* void RandomInitByArray(uint32 seeds[], int length);
* In CRandomMersenne only: Use this function if you want to initialize with
* a seed with more than 32 bits. All bits in the seeds[] array will influence
* the sequence of random numbers generated. length is the number of entries
* in the seeds[] array.
*
* double Random();
* Gives a floating point random number in the interval 0 <= x < 1.
* The resolution is 32 bits in CRandomMother and CRandomMersenne.
*
* int IRandom(int min, int max);
* Gives an integer random number in the interval min <= x <= max.
* (max-min < MAXINT).
* The precision is 2^-32 (defined as the difference in frequency between 
* possible output values). The frequencies are exact if max-min+1 is a
* power of 2.
*
* int IRandomX(int min, int max);
* Same as IRandom, but exact. In CRandomMersenne only.
* The frequencies of all output values are exactly the same for an 
* infinitely long sequence. (Only relevant for extremely long sequences).
*
* uint32 BRandom();
* Gives 32 random bits. 
*
*
* Example:
* ========
* The file EX-RAN.CPP contains an example of how to generate random numbers.
*
*
* Optimized version:
* ==================
* Faster versions of these random number generators are provided as function
* libraries in asmlib.zip. These function libraries are coded in assembly
* language and support only x86 platforms, including 32-bit and 64-bit
* Windows, Linux, BSD, Mac OS-X (Intel based). Use asmlibran.h from asmlib.zip
*
*
* Non-uniform random number generators:
* =====================================
* Random number generators with various non-uniform distributions are available
* in stocc.zip (www.agner.org/random).
*
*
* Further documentation:
* ======================
* The file randomc.htm contains further documentation on these random number
* generators.
*
*
* Copyright:
============
* � 1997 - 2007 Agner Fog. All software in this library is published under the
* GNU General Public License with the further restriction that it cannot be 
* used for gambling applications. See licence.htm
*******************************************************************************/


/***********************************************************************
Define random number generator classes
***********************************************************************/

class CRandomMersenne {                // Encapsulate random number generator
#if 0
	// Define constants for type MT11213A:
#define MERS_N   351
#define MERS_M   175
#define MERS_R   19
#define MERS_U   11
#define MERS_S   7
#define MERS_T   15
#define MERS_L   17
#define MERS_A   0xE4BD75F5
#define MERS_B   0x655E5280
#define MERS_C   0xFFD58000
#else    
	// or constants for type MT19937:
#define MERS_N   624
#define MERS_M   397
#define MERS_R   31
#define MERS_U   11
#define MERS_S   7
#define MERS_T   15
#define MERS_L   18
#define MERS_A   0x9908B0DF
#define MERS_B   0x9D2C5680
#define MERS_C   0xEFC60000
#endif
public:
	CRandomMersenne(uint32 seed) {      // Constructor
		RandomInit(seed); LastInterval = 0;}
	void RandomInit(uint32 seed);       // Re-seed
	void RandomInitByArray(uint32 seeds[], int length); // Seed by more than 32 bits
	int IRandom (int min, int max);     // Output random integer
	int IRandomX(int min, int max);     // Output random integer, exact
	double Random();                    // Output random float
	uint32 BRandom();                   // Output random bits
private:
	void Init0(uint32 seed);            // Basic initialization procedure
	uint32 mt[MERS_N];                  // State vector
	int mti;                            // Index into mt
	uint32 LastInterval;                // Last interval length for IRandomX
	uint32 RLimit;                      // Rejection limit used by IRandomX
	enum TArch {LITTLE_ENDIAN1, BIG_ENDIAN1, NONIEEE}; // Definition of architecture
	TArch Architecture;                 // Conversion to float depends on architecture
};    

#endif

