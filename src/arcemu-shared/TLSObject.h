#pragma once
#include "Common.h"

namespace Arcemu
{
	namespace Utility
	{
		template<typename T>
		class TLSObject
		{
			FastMutex createlock_;	//Lock held during creation and destruction.
#if PLATFORM == PLATFORM_WIN32
			DWORD key_;
#elif PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_UNIX
			pthread_key_t key_;
#else
#error Your platform does not support Thread Local Storage mechanism!
#endif
		public:
			TLSObject() { create(); }
			~TLSObject() { destroy(); }

			//************************************
			// Purpose : Does any instantiation required by the tls mechanism. Called in constructor.
			// Return : void
			//************************************
			void create()
			{
				createlock_.Acquire();
#if PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_UNIX
				pthread_key_create(&key_, NULL);
#elif PLATFORM == PLATFORM_WIN32
				key_ = TlsAlloc();
#endif
				createlock_.Release();
			}

			//************************************
			// Purpose : Sets the given value to a tls slot of the calling thread.
			// Parameter:	T - value to store in the tls slot
			// Return : void
			//************************************
			ARCEMU_INLINE void set(T val)
			{
#if PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_UNIX
				pthread_setspecific(key_, static_cast<const void*>(val) );
#elif PLATFORM == PLATFORM_WIN32
				TlsSetValue(key_, static_cast<LPVOID>(val);
#endif
			}

			//************************************
			// Purpose : Returns a reference to the stored tls value.
			// Return : T 
			//************************************
			ARCEMU_INLINE T get() const
			{
#if PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_UNIX
				return static_cast<T>( pthread_getspecific(key_) );
#elif PLATFORM == PLATFORM_WIN32
				return static_cast<T>( TlsGetValue(key_) );
#else
				return static_cast<T>(NULL);
#endif
			}

			//************************************
			// Purpose : Does any necessary cleanup required by the tls mechanism. Called in destructor
			// Return : void
			//************************************
			void destroy()
			{
				createlock_.Acquire();
#if PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_UNIX
				pthread_key_delete(key_);
#elif PLATFORM == PLATFORM_WIN32
				TlsFree(key_);
#endif
				createlock_.Release();
			}
			void operator=(T val) { set(val); }
			bool operator==(T val) const { return get() == val; }
			bool operator!=(T val) const { return operator==(val); }
		};
	}
}