#ifndef ATOMICCOUNTER_HPP_
#define ATOMICCOUNTER_HPP_

namespace Arcemu{
	namespace Threading{

		//////////////////////////////////////////////////////////
		//class AtomicCounter
		//  Derives from AtomicULong.
		//  Implements atomic incrementation and decrementation
		//
		//////////////////////////////////////////////////////////
		class AtomicCounter : public AtomicULong{
		public:
			AtomicCounter(){ Value = 0; }
			AtomicCounter( unsigned long InitialValue ){ Value = InitialValue; }


			//////////////////////////////////////////////////////////
			//unsigned long operator++()
			// lockless threadsafe prefix increment operator
			//
			// Parameters
			//  None
			//
			// Return values
			//  Returns the incremented value
			/////////////////////////////////////////////////////////
			unsigned long operator++();



			/////////////////////////////////////////////////////////
			//unsigned long operator--()
			// lockless threadsafe prefix decrement operator
			//
			// Parameters
			//  None
			//
			// Return values
			//  Returns the decremented value
			////////////////////////////////////////////////////////
			unsigned long operator--();



		private:
			// Disabled copy constructor
			AtomicCounter( const AtomicCounter &other ){}

			// Disabled assignment operator
			AtomicCounter operator=( const AtomicCounter &other ){ return *this; }
		};
	}
}

#endif
