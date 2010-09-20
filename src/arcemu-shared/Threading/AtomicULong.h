#ifndef ATOMICULONG_HPP_
#define ATOMICULONG_HPP_

namespace Arcemu{
	namespace Threading{

		/////////////////////////////////////////////////
		//class AtomicULong
		//  Stores an unsigned long atomically.
		//  Base class for all Arcemu atomics.
		//
		////////////////////////////////////////////////
		class AtomicULong{
		public:
			AtomicULong(){ Value = 0; }

			AtomicULong( unsigned long InitialValue ){ Value = InitialValue; }

			////////////////////////////////////////////////////////////
			//unsigned long SetVal( unsigned long NewValue )
			// lockless threadsafe set operation on the contained value
			//
			// Parameters
			//  unsigned long val  -  value to be set
			//
			// Return values
			//  Returns the initial value contained
			///////////////////////////////////////////////////////////
			unsigned long SetVal( unsigned long NewValue );


			///////////////////////////////////////////////////////////
			//unsigned long GetVal()
			// non-threadsafe get operation on the contained value
			//
			// Parameters
			//  None
			//
			// Return values
			//  Returns the value contained
			//////////////////////////////////////////////////////////
			unsigned long GetVal(){ return Value; }


		private:
			// Disabled copy constructor
			AtomicULong( const AtomicULong &other ){}

			// Disabled assignment operator
			AtomicULong operator=( AtomicULong &other ){ return *this; }


		protected:

#ifdef WIN32
			__declspec( align( 4 ) )  volatile unsigned long Value;
#else
			volatile unsigned long Value;
#endif
		};
	}
}

#endif
