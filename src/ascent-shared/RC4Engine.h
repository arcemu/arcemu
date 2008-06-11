/****************************************************************************
 *
 * RC4 Encyption Class
 * Copyright (c) 2007 Burlex
 * Adapted from rc4.c to C++
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _SHARED_RC4ENGINE_H
#define _SHARED_RC4ENGINE_H

class RC4Engine
{
    unsigned char perm[256];
    unsigned char index1, index2;
    bool Initialized;

public:

    //! RC4Engine constructor. Must supply a key and the length of that array.
    RC4Engine(const unsigned char * keybytes, unsigned int keylen)
    {
        Setup(keybytes, keylen);
    }

    RC4Engine() : Initialized(false) {}

    //! Destructor
    ~RC4Engine() { }

    //! Initializes permutation, etc.
    void Setup(const unsigned char * keybytes, const unsigned int keylen)
    {
        unsigned int i = 0;
        unsigned char j = 0, k;

        // Initialize RC4 state (all bytes to 0)
        for(; i < 256; ++i)
            perm[i] = (uint8)i;

        // Set index values
        index1 = index2 = 0;

        // Randomize the permutation
        for(j = 0, i = 0; i < 256; ++i)
        {
            j += perm[i] + keybytes[i % keylen];
            k = perm[i];
            perm[i] = perm[j];
            perm[j] = k;
        }

        Initialized = true;
    }

    //! Processes the specified array. The same function is used for both
    //  encryption and decryption.
    void Process(unsigned char * input, unsigned char * output, unsigned int len)
    {
        unsigned int i = 0;
        unsigned char j, k;

        for(; i < len; ++i)
        {
            index1++;
            index2 += perm[index1];

            k = perm[index1];
            perm[index1] = perm[index2];
            perm[index2] = k;

            j = perm[index1] + perm[index2];
            output[i] = input[i] ^ perm[j];
        }
    }
};


//! Reverses the bytes in an array in the opposite order.
__inline void ReverseBytes(unsigned char * Pointer, unsigned int Length)
{
	unsigned char * Temp = (unsigned char*)malloc(Length);
	memcpy(Temp, Pointer, Length);

	for(unsigned int i = 0; i < Length; ++i)
		Pointer[i] = Temp[Length - i - 1];

	free(Temp);
}

#endif  // _SHARED_RC4ENGINE_H
