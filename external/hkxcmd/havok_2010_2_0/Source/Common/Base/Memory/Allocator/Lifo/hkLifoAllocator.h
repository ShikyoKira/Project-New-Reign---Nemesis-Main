/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_LIFO_ALLOCATOR_H
#define HK_BASE_LIFO_ALLOCATOR_H

	/// Specialized for LIFO or almost-LIFO pattern allocations.
	/// This allocator gets large blocks (slabs) from a parent allocator
	/// and slices them into smaller allocations. It is most efficient
	/// when the allocations are in pure LIFO order, but can handle a few
	/// out of order frees with only a small penalty.
	/// Requests larger than the slab size are forwarded to another allocator.
class hkLifoAllocator : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:

		HK_DECLARE_PLACEMENT_ALLOCATOR();

		hkLifoAllocator(int slabSize = 16384);

		void init(hkMemoryAllocator* slabAllocator, 
				  hkMemoryAllocator* largeAllocator,
				  hkMemoryAllocator* internalAllocator);
		void quit();

		virtual void* blockAlloc( int numBytes );

			/// Inline equivalent of blockAlloc
		HK_FORCE_INLINE void* fastBlockAlloc(int numBytes);

		virtual void blockFree( void* p, int numBytes );

			/// Inline equivalent of blockFree
		HK_FORCE_INLINE void fastBlockFree(void* p, int numBytes);

		virtual void* bufAlloc( int& reqNumBytesInOut );
		virtual void bufFree( void* p, int numBytes );
		virtual void* bufRealloc( void* pold, int oldNumBytes, int& reqNumBytesInOut );

		virtual void getMemoryStatistics( MemoryStatistics& u ) {}
		virtual int getAllocatedSize(const void* obj, int nbytes) {return nbytes; }

		hkBool32 isEmpty() const;

			/// number of large + slab allocations
		int numExternalAllocations() const;

	protected:

		void* allocateFromNewSlab(int nbytes);
		void popNonLifoFrees();
		void slowBlockFree(void*, int numBytesIn);
		void insertNonLifoFree(void* p, int nbytes);

	protected:

		class Implementation;

			/// Pointer to internal data.
		class Implementation* m_impl;

			/// The size of each slab.
		const int m_slabSize;

			/// The start of the free space on the current slab.
		void* m_cur;
			/// The end of the current slab.
		void* m_end;
			/// The end of the last exception. After a free, when m_cur is equal
			/// to this value, we need to process some deferred frees.
		void* m_firstNonLifoEnd;

			/// We cache the last freed slab to avoid a performance spike when
			/// repeatedly crossing a slab boundary.
		void* m_cachedEmptySlab;

			/// For allocations of m_slabSize
		hkMemoryAllocator* m_slabAllocator;
			/// For blocks larger than the slab size
		hkMemoryAllocator* m_largeAllocator;
			/// For internal bookkeeping
		hkMemoryAllocator* m_internalAllocator;
};

#include <Common/Base/Memory/Allocator/Lifo/hkLifoAllocator.inl>

#endif // HK_BASE_LIFO_ALLOCATOR_H

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
