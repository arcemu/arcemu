#include "../Common.h"

namespace Arcemu{
	namespace Threading{

		unsigned long AtomicCounter::operator++(){
			unsigned long val = 0;

#ifdef WIN32
			val = InterlockedIncrement( reinterpret_cast< volatile LONG* >( &Value ) );
#else
#ifdef __GNUC__
			val = __sync_add_and_fetch( &Value, 1 );
#else
#error Your platform (architecture and compiler) is NOT supported. Arcemu requires little endian architecture, and at least GCC 4.1
#endif
#endif

			return val;
		}

		unsigned long AtomicCounter::operator--(){
			unsigned long val = 0;

#ifdef WIN32
			val = InterlockedDecrement( reinterpret_cast< volatile LONG* >( &Value ) );
#else
#ifdef __GNUC__
			val = __sync_add_and_fetch( &Value, -1 );
#else
#error Your platform (architecture and compiler) is NOT supported. Arcemu requires little endian architecture, and at least GCC 4.1
#endif
#endif
			return val;
		}

	}
}