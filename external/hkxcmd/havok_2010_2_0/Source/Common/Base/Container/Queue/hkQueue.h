/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_QUEUE_H
#define HKBASE_QUEUE_H



/// A FIFO circular queue
template <typename T>
class hkQueue
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkQueue );

			/// Creates a zero length queue.
		hkQueue();

			/// Creates an queue of capacity n. All elements are uninitialized.
		hkQueue( int capacity );

			/// Deallocates queue memory.
		~hkQueue();

			// Allocates more internal queue storage
		inline void setCapacity(int n);

			/// Pushes a new element to the back of the queue and expands the storage if necessary.
		inline void enqueue( const T& element );

			/// Pushes a new element to the front of the queue and expands the storage if necessary.
		inline void enqueueInFront( const T& element );

			/// Fills in the data with the element at the front of the queue
		inline void dequeue( T& data );

			/// Fills in the data with the element at the front of the queue but does not modify the queue
		inline void peek( T& data ) const;

			/// Clears the queue
		inline void clear();

			/// Are there any elements left on the queue?
		inline hkBool isEmpty() const;

			/// How many elements are on the queue?
		inline int getSize() const;

			/// Returns the total capacity of the queue storage
		inline int getCapacity() const;

			/// Set the initial capacity for the queue.
			/// This must be called before any elements have been added to the queue.
		//inline void setInitialCapacity( int capacity );

	private:

		inline void increaseCapacity();

			// frees the internal storage
		void releaseMemory();

			// queue memory management
		T* m_data;
		int m_capacity; 

			// queue FIFO management
		int m_head;
		int m_tail;
		int m_elementsInUse;
};

#	include <Common/Base/Container/Queue/hkQueue.inl>

#endif // HKBASE_QUEUE_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
