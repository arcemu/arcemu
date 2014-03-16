/**
 @file AtomicInt32.h

 @maintainer Morgan McGuire, http://graphics.cs.williams.edu

 @created 2005-09-01
 @edited  2006-06-21
 */
#ifndef G3D_ATOMICINT32_H
#define G3D_ATOMICINT32_H

#include "G3D/platform.h"
#include "G3D/g3dmath.h"

#if defined(G3D_OSX)
#   include <libkern/OSAtomic.h>
#endif

namespace G3D {

/**
 An integer that may safely be used on different threads without
 external locking.

 On Win32, Linux, FreeBSD, and Mac OS X this is implemented without locks.  

 <B>BETA API</B>  This is unsupported and may change
 */
class AtomicInt32 {
private:
#   if defined(G3D_WIN32) 
    volatile long           m_value;
#   elif defined(G3D_OSX)
    int32_t                 m_value;
#   else
    volatile int32          m_value;
#   endif


public:

    /** Initial value is undefined. */
    AtomicInt32() {}

    /** Atomic set */
    explicit AtomicInt32(const int32 x) {
        m_value = x;
    }

    /** Atomic set */
    AtomicInt32(const AtomicInt32& x) {
        m_value = x.m_value;
    }

    /** Atomic set */
    const AtomicInt32& operator=(const int32 x) {
        m_value = x;
        return *this;
    }

    /** Atomic set */
    void operator=(const AtomicInt32& x) {
        m_value = x.m_value;
    }

    /** Returns the current value */
    int32 value() const {
        return m_value;
    }

    /** Returns the old value, before the add. */
    int32 add(const int32 x) {
#       if defined(G3D_WIN32)

            return InterlockedExchangeAdd(&m_value, x);

#       elif defined(G3D_LINUX) || defined(G3D_FREEBSD)

/*
 * since this is linux or freebsd, we should be using gcc right now.
 * So why not use the builtin atomic functions provided by it?
 * 
 * Of course this is a bad idea, someone may be using llvm, icc or
 * something really exotic. anyway, We guess GCC for now.
 *
 * there are new atomic functions since gcc-4.7,
 * while the older ones will be deprecated in the future,
 * but I dont care for now.
 */

            return __sync_fetch_and_add(&m_value, 1);

#       elif defined(G3D_OSX)

            int32 old = m_value;
            OSAtomicAdd32(x, &m_value);
            return old;

#       endif
    }

    /** Returns old value. */
    int32 sub(const int32 x) {
        return add(-x);
    }

    void increment() {
#       if defined(G3D_WIN32)
            // Note: returns the newly incremented value
            InterlockedIncrement(&m_value);
#       elif defined(G3D_LINUX) || defined(G3D_FREEBSD)
            add(1);
#       elif defined(G3D_OSX)
            // Note: returns the newly incremented value
            OSAtomicIncrement32(&m_value);
#       endif
    }

    /** Returns zero if the result is zero after decrement, non-zero otherwise.*/
    int32 decrement() {
#       if defined(G3D_WIN32)
            // Note: returns the newly decremented value
            return InterlockedDecrement(&m_value);
#       elif defined(G3D_LINUX)  || defined(G3D_FREEBSD)

/*
 * since this is linux or freebsd, we should be using gcc right now.
 * So why not use the builtin atomic functions provided by it?
 * 
 * Of course this is a bad idea, someone may be using llvm, icc or
 * something really exotic. anyway, We guess GCC for now.
 *
 * there are new atomic functions since gcc-4.7,
 * while the older ones will be deprecated in the future,
 * but I dont care for now.
 */
            return __sync_sub_and_fetch(&m_value, 1) == 0;

#       elif defined(G3D_OSX)
            // Note: returns the newly decremented value
            return OSAtomicDecrement32(&m_value);
#       endif
    }


    /** Atomic test-and-set:  if <code>*this == comperand</code> then <code>*this := exchange</code> else do nothing.
        In both cases, returns the old value of <code>*this</code>.
    
        Performs an atomic comparison of this with the Comperand value. 
        If this is equal to the Comperand value, the Exchange value is stored in this.
        Otherwise, no operation is performed.

        Under VC6 the sign bit may be lost.
     */ 
    int32 compareAndSet(const int32 comperand, const int32 exchange) {
#       if defined(G3D_WIN32)
            return InterlockedCompareExchange(&m_value, exchange, comperand);
#       elif defined(G3D_LINUX) || defined(G3D_FREEBSD)

/*
 * since this is linux or freebsd, we should be using gcc right now.
 * So why not use the builtin atomic functions provided by it?
 * 
 * Of course this is a bad idea, someone may be using llvm, icc or
 * something really exotic. anyway, We guess GCC for now.
 *
 * there are new atomic functions since gcc-4.7,
 * while the older ones will be deprecated in the future,
 * but I dont care for now.
 */

            return __sync_val_compare_and_swap(&m_value, comperand, exchange);

#       elif defined(G3D_OSX)
            // Based on Apache Portable Runtime
            // http://koders.com/c/fid3B6631EE94542CDBAA03E822CA780CBA1B024822.aspx
            int32 ret;
            asm volatile ("lock; cmpxchgl %1, %2"
                          : "=a" (ret)
                          : "r" (exchange), "m" (m_value), "0"(comperand)
                          : "memory", "cc");
            return ret;

            // Note that OSAtomicCompareAndSwap32 does not return a useful value for us
            // so it can't satisfy the cmpxchgl contract.
#       endif
    }

};

} // namespace

#endif
