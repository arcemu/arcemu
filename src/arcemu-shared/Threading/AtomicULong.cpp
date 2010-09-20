#include "../Common.h"

namespace Arcemu{
	namespace Threading{

		unsigned long AtomicULong::SetVal( unsigned long NewValue ){
			unsigned long ret = 0;
#ifdef WIN32
			ret = InterlockedExchange( reinterpret_cast< volatile LONG* >( &Value ), LONG( NewValue ) );
#else
#ifdef __GNUC__
			ret = __sync_val_compare_and_swap( &counter, Value, NewValue );
#else
#error Your platform (architecture and compiler) is NOT supported. Arcemu requires little endian architecture, and at least GCC 4.1
#endif
#endif
			return ret;

		}

	}
}