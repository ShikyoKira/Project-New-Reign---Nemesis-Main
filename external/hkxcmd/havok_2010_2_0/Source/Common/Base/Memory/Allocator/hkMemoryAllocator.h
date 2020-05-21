/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_hkMemoryAllocator_H
#define HKBASE_hkMemoryAllocator_H

#ifdef __GCCXML__
#	define HK_PLACEMENT_ATTRIBUTE __attribute__((gccxml("placement")))
#else
#	define HK_PLACEMENT_ATTRIBUTE
#endif

#define HK_DECLARE_PLACEMENT_ALLOCATOR() \
	HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t, void* p) HK_PLACEMENT_ATTRIBUTE { return p; } \
	HK_FORCE_INLINE void HK_CALL operator delete(void*, void*) {} \
	HK_FORCE_INLINE void HK_CALL operator delete(void*) {}

// trick to have hkDeallocate() work with a void* argument (sizeof(void) is undefined, but makes sense to be 1)
template <typename T> struct hkSizeOfTypeOrVoid { enum { val = sizeof(T) }; };
template <> struct hkSizeOfTypeOrVoid<void> { enum { val = 1 }; };

	/// Base class for all memory allocation implementations.
	/// A derived class may simply override blockAlloc, blockFree,
	/// getMemoryStatistics and getAllocatedSize with an alternative
	/// allocation system.
	/// If an implementation may supply more memory than requested
	/// or may be able to reallocate without copying, it may benefit from
	/// also overriding the buf* methods.
class hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_PLACEMENT_ALLOCATOR();

		typedef void (HK_CALL *MemoryWalkCallback)(void* start, hk_size_t size, hkBool allocated, int pool, void* param);

			/// A struct holding memory allocation information.
			/// Not all allocators support all statistics. If a statistic is
			/// unavailable/unsupported, its value is -1.
		struct MemoryStatistics
		{
				/// The default constructor initializes all values to -1.
			MemoryStatistics() : m_allocated(-1), m_inUse(-1), m_peakInUse(-1), m_available(-1), m_totalAvailable(-1), m_largestBlock(-1) {}

				/// Infinite as far as the memory system is concerned.
			static const hkLong INFINITE_SIZE = -1;

				/// Total amount of allocated memory. Allocated memory means that it is 'owned' by the hkMemory reporting it -
				/// but the actual memory inside may be available or used. Thus m_inUse + m_available <= m_allocated. The difference will
				/// be due to overhead for managing the memory.
			hkLong m_allocated;

				/// Total used memory
			hkLong m_inUse;

				/// The peak usage
			hkLong m_peakInUse;

				/// Total available from the _allocated_ memory. This is NOT the total amount of memory that is potentially available,
				/// only the amount that remains available from what has been allocated.
			hkLong m_available;

				/// The approx total amount of memory that is available. The value is the amount of available memory (m_available)
				/// plus the approx amount of memory that is potentially available but not yet allocated.
				/// Depending on the allocator being used, this value may be meaningless. For example for an allocator with virtual memory
				/// the 'total amount of available' is somewhat meaningless. In those scenarios, m_totalMemory will be INFINITE_SIZE
			hkLong m_totalAvailable;

				/// Largest block available. On some memory allocators this is meaningless (for example with virtual memory). In that
				/// case this will be set to INFINITE_SIZE
			hkLong m_largestBlock;
		};

			/// The ExtendedInterface is an optional interface that an implementation of an Allocator may implement.
			/// In implementing the interface - the allocator can be used to provide easily a full implementation of
			/// a memory system, using the hkFreeListMemorySystem. (NOTE - that the underlying memory allocator does not
			/// need to be a freelist to work within the hkFreeListMemorySystem)
		struct ExtendedInterface
		{
			virtual ~ExtendedInterface() {}

				/// Try to coalesce freed blocks and return them to the parent allocator.
			virtual void garbageCollect() = 0;
				/// Do an incremental garbage collection. The parameter controls how much work will be performed in the
				/// collection. Passing 0 means the default amount. How the parameter controls the collection is dependent
				/// on the underlying implementation.
			virtual void incrementalGarbageCollect(int numBlocks) = 0;
				/// Set the soft memory limit
			virtual hkResult setMemorySoftLimit(hk_size_t maxMemory) = 0;
				///	Get the soft memory limit
			virtual hk_size_t getMemorySoftLimit() const = 0;
				/// Returns if an alloc of the size specified will succeed.
			virtual bool canAllocTotal( int numBytes ) = 0;
				/// Traverses all of the memory used by the subsystem (allocated and free). The order that allocations
				/// are returned is undefined. Not all memory allocators can walk memory - if an implementation cannot
				/// it will return HK_FAILURE from this method.
			virtual hkResult walkMemory(MemoryWalkCallback callback, void* param) = 0;
		};

			///
		virtual ~hkMemoryAllocator();

		
			/// Allocate numBytes.
		virtual void* blockAlloc( int numBytes ) =0;

			/// Free numBytes.
		virtual void blockFree( void* p, int numBytes ) =0;

			/// Request a buffer of reqNumBytesInOut.
			/// The implementation may modify reqNumInOut to return a larger
			/// block than requested.
			/// Default implementation forwards to blockAlloc.
		virtual void* bufAlloc( int& reqNumBytesInOut );

			/// Free a buffer.
			/// Default implementation forwards to blockFree.
			/// \a numElem is the number of elements as returned by the bufAlloc in reqNumInOut
		virtual void bufFree( void* p, int numBytes );

			/// Reallocate a buffer.
			/// Default implementation is blockAlloc, memCpy, blockFree.
		virtual void* bufRealloc( void* pold, int oldNumBytes, int& reqNumBytesInOut );
		

			/// Allocate several blocks at the same time.
			/// May be more efficient than individual allocations if the allocator must be locked.
		virtual void blockAllocBatch(void** ptrsOut, int numPtrs, int blockSize);

			/// Free several blocks at the same time.
		virtual void blockFreeBatch(void** ptrsIn, int numPtrs, int blockSize);

			/// Work out memory statistics. This function tries to work out fairly accurately details about memory usage -
			/// this functions performance may not be great depending on the underlying implementation.
			/// See hkMemoryStatistics for details of the information returned.
			/// NOTE! That if you have a memory limit set (via setMemoryLimit) the values returned will reflect that limit
			/// you must have the limit set to zero if you want to find out the 'true' memory statistics.
		virtual void getMemoryStatistics( MemoryStatistics& u )= 0;

			/// Return the padded size. Most memory system will round up the allocation size or add extra headers and footers.
			/// This function should return the (padded) size which potentially is usable by the user
			///
			/// The size (and any size between the original allocated size, and the value returned from getAllocatedSize) can be passed
			/// to this method, and it will produce the same result. Also note the block/bufFree can take any value between the original
			/// allocated size, and the size returned from getAllocatedSize, and work correctly.
		virtual int getAllocatedSize(const void* obj, int nbytes) = 0;

			/// Reset internal counts of mem usage.
		virtual void resetPeakMemoryStatistics() {}

			/// Allocate a buffer which is a multiple of SIZE_ELEM.
		template<typename TYPE>
		inline TYPE* _bufAlloc( int& reqNumInOut )
		{
			const int SIZE_ELEM = hkSizeOfTypeOrVoid<TYPE>::val;
			int n = reqNumInOut * SIZE_ELEM;
			void* p = bufAlloc( n );
			reqNumInOut = n / SIZE_ELEM;
			return static_cast<TYPE*>(p);
		}

			/// Free a buffer which is a multiple of SIZE_ELEM.
		template<typename TYPE>
		inline void _bufFree( void* p, int numElem )
		{
			const int SIZE_ELEM = hkSizeOfTypeOrVoid<TYPE>::val;
			bufFree( p, numElem * SIZE_ELEM );
		}

			/// Reallocate a buffer which is a multiple of SIZE_ELEM.
		template<typename TYPE>
		inline TYPE* _bufRealloc( void* pold, int oldNum, int& reqNumInOut )
		{
			const int SIZE_ELEM = hkSizeOfTypeOrVoid<TYPE>::val;
			int n = reqNumInOut*SIZE_ELEM;
			void* p = bufRealloc( pold, oldNum*SIZE_ELEM, n );
			reqNumInOut = n / SIZE_ELEM;
			return static_cast<TYPE*>(p);
		}

			/// Allocate a block to hold n TYPEs
		template<typename TYPE>
		inline TYPE* _blockAlloc( int n )
		{
			return static_cast<TYPE*>( blockAlloc(n*hkSizeOfTypeOrVoid<TYPE>::val) );
		}
			/// Free a block of n TYPEs
		template<typename TYPE>
		inline void _blockFree( TYPE* p, int n )
		{
			blockFree( p, n*hkSizeOfTypeOrVoid<TYPE>::val );
		}

	public:

		/// Status of the memory manager.
		enum MemoryState
		{
			MEMORY_STATE_OK,			///< Memory OK
			MEMORY_STATE_OUT_OF_MEMORY	///< Low on Memory
		};
};

#endif // HKBASE_hkMemoryAllocator_H

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
