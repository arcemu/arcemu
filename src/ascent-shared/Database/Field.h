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

#if !defined(FIELD_H)
#define FIELD_H

class Field
{
public:

	ASCENT_INLINE void SetValue(char* value) { mValue = value; }

	ASCENT_INLINE const char *GetString() { return mValue; }
	ASCENT_INLINE float GetFloat() { return mValue ? static_cast<float>(atof(mValue)) : 0; }
	ASCENT_INLINE bool GetBool() { return mValue ? atoi(mValue) > 0 : false; }
	ASCENT_INLINE uint8 GetUInt8() { return mValue ? static_cast<uint8>(atol(mValue)) : 0; }
	ASCENT_INLINE int8 GetInt8() { return mValue ? static_cast<int8>(atol(mValue)) : 0; }
	ASCENT_INLINE uint16 GetUInt16() { return mValue ? static_cast<uint16>(atol(mValue)) : 0; }
	ASCENT_INLINE uint32 GetUInt32() { return mValue ? static_cast<uint32>(atol(mValue)) : 0; }
	ASCENT_INLINE uint32 GetInt32() { return mValue ? static_cast<int32>(atol(mValue)) : 0; }
	uint64 GetUInt64() 
	{
		if(mValue)
		{
			uint64 value;
			#ifndef WIN32	// Make GCC happy.
			sscanf(mValue,I64FMTD,(long long unsigned int*)&value);
			#else
			sscanf(mValue, I64FMTD, &value);
			#endif
			return value;
		}
		else
			return 0;
	}

private:
		char *mValue;
};

#endif
