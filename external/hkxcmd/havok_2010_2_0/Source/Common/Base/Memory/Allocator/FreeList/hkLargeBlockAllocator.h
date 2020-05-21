/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_LARGE_BLOCK_ALLOCATOR_H
#define HK_LARGE_BLOCK_ALLOCATOR_H

	/// This is a large memory block allocator, which is designed to handle allocations 256 bytes or larger. Smaller
	/// allocations will be serviced but will internally degenerate to a 256 byte block. Thus this allocator is not
	/// best used for small high performance chunks of memory. In that situation a freelist or pool memory style
	/// allocator is more suitable.
	/// 
	/// What this allocator does allow for is good usage of varying sized chunks of memory. A 128k chunk of memory or a 2.7k
	/// chunk of memory both have the same small overhead.
	/// 
	/// The allocator is designed to be fast and efficient. When a block is requested a search is performed down a tree
	/// of available blocks to find the one most suitable. Most of the complexity of the algorithm comes from this inplace
	/// tree traversal.
	/// 
	/// The allocator is designed to have good cache usage
	/// 
	/// Some notes on the algorithm:
	/// 
	///     - Adjacent free chunks are always merged - thus the chunks adjacent to a free chunk must be in use.
	///     - Each chunk consists of a header, which records the size of this block, and therefore implicitly the start of the next
	///       block.
	///     - Each header records: If this chunk is in use, if the previous chunk is in use, if the previous
	///       chunk is NOT in use, how large the previous chunk is.
	///     - When a chunk is not in use its header is 'extended' into the payload - such that the block can be a tree member
	///     - There is the concept of the 'top block' for allocations in a contiguous chunk of memory, this is where an
	///       allocation would take place when all else fails. In this algorithm the top block is special
	///       because its contents may not be correctly constructed
	///     - Allocations of actual memory go through the hkMemoryBlockServer. Multiple non contiguous
	///       allocations are possible :) When this happens internally blocks are set up so the chunks
	///       of memory between the blocks owned by this allocator appear as 'inuse', when in reality they
	///       don't belong to this allocator.
	///     - Allocations from the hkMemoryBlockServer are held in hkMemPages
class hkLargeBlockAllocator : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
	public:

		HK_DECLARE_PLACEMENT_ALLOCATOR();

		typedef unsigned int BinIndex;
		typedef unsigned int BinMap;
			/// Has the same amount of bits as a hk_size_t but is signed.
		typedef int SignedSizeT;

			// Internal
		struct MemChunk
		{
			//+hk.MemoryTracker(ignore=True)
			static const hk_size_t PINUSE_BIT = 1;
			static const hk_size_t CINUSE_BIT = 2;
			static const hk_size_t INUSE_BITS = 3;

			static const hk_size_t ALIGN = 16;
			static const hk_size_t ALIGN_MASK = ALIGN-1;

				/// The bytes from the MemChunk to the payload inside
				/// Would be better to do sizeof(MemChunk), but I can't do that here - so I do sizeof(hk_size_t)*2
			static const hk_size_t PAYLOAD_OFFSET = (sizeof(hk_size_t)*2 + ALIGN_MASK)&~ALIGN_MASK;

				/// Returns true if the previous block is in use
			HK_FORCE_INLINE bool isPinuse() const { return (head&PINUSE_BIT)!=0; }

				/// Returns true if this block is in use
			HK_FORCE_INLINE bool isInuse() const { return (head&CINUSE_BIT)!=0; }

				/// The chunk size is the total size (including the hkMallocChunk data)
			HK_FORCE_INLINE hk_size_t getChunkSize() const { return (head&~INUSE_BITS); }

				/// Clear the previous in use flag
			HK_FORCE_INLINE void clearPinuse() { head &= ~PINUSE_BIT; }

				/// Clear the in use flag
			HK_FORCE_INLINE void clearInuse() { head &= ~CINUSE_BIT; }

				/// Returns the next chunks previous in use
			HK_FORCE_INLINE bool isNextPinuse() { return nextChunk()->isPinuse(); }

				/// Get the address of the next chunk
			HK_FORCE_INLINE MemChunk* nextChunk() { return (MemChunk*)(((char*)this) + getChunkSize()); }

				/// Get the address of the previous chunk
			HK_FORCE_INLINE MemChunk* previousChunk() { return (MemChunk*)(((char*)this) - prevFoot); }

				/// Return memory as chunk x bytes after the chunk
			HK_FORCE_INLINE MemChunk* chunkPlusOffset(hk_size_t s) { return (MemChunk*)(((char*)this)+s); }

				/// Return memory as chunk x bytes before the chunk
			HK_FORCE_INLINE MemChunk* chunkMinusOffset(hk_size_t s) { return (MemChunk*)(((char*)this)-s); }

				/// Return the address of the contained data
			HK_FORCE_INLINE void* getPayload() { return (void*)(((char*)this) + PAYLOAD_OFFSET); }

				/// Get the size of the payload
			HK_FORCE_INLINE hk_size_t getPayloadSize() { return getChunkSize() - PAYLOAD_OFFSET; }

				/// Turn an address into a memory block
			HK_FORCE_INLINE static MemChunk* toChunk(void* in) { return (MemChunk*)(((char*)in) - PAYLOAD_OFFSET); }

				/// Set cinuse and pinuse of this chunk and pinuse of next chunk
			HK_FORCE_INLINE void setInuseAndPinuse(hk_size_t s)
			{
				head = (s|PINUSE_BIT|CINUSE_BIT);
				MemChunk* next = chunkPlusOffset(s);
				next->head |= PINUSE_BIT;
			}

				/// Set inuse
			HK_FORCE_INLINE void setInuse(hk_size_t s)
			{
				head = (head & PINUSE_BIT)|s|CINUSE_BIT;
				MemChunk* next = chunkPlusOffset(s);
				next->head |= PINUSE_BIT;
			}

				/// Set ths size, inuse and pinuse of the chunk
			HK_FORCE_INLINE void setSizeAndPinuseOfInuseChunk(hk_size_t s)
			{
				head = (s|PINUSE_BIT|CINUSE_BIT);
			}

				/// Get size at footer
			HK_FORCE_INLINE hk_size_t getFoot(hk_size_t s)  { return ((MemChunk*)(((char*)this)+s))->prevFoot; }

				/// Set the footer size
			HK_FORCE_INLINE void setFoot(hk_size_t s) { ((MemChunk*)(((char*)this) + s))->prevFoot = s; }

				/// Set size, pinuse bit, and foot
			HK_FORCE_INLINE void setSizeAndPinuseOfFreeChunk(hk_size_t s)
			{
				head = (s|PINUSE_BIT);
				setFoot(s);
			}
				/// Set size, pinuse bit, foot, and clear next pinuse
			HK_FORCE_INLINE void setFreeWithPinuse(hk_size_t s,MemChunk* n)
			{
				n->clearPinuse();
				setSizeAndPinuseOfFreeChunk(s);
			}

				/// Returns true if a pointer is aligned appropriately
			static bool isAligned(const void* ptr){ return (((hk_size_t)(ptr))&ALIGN_MASK)==0; }

			// Members

				/// Size of previous chunk including header etc (if free).
			hk_size_t prevFoot;
				/// Size and inuse bits.
			hk_size_t head;
		};

			// Assumes the fields are placed after MemChunk
		struct FreeMemChunk : public MemChunk
		{
			//+hk.MemoryTracker(ignore=True)
				//  double links -- used only if free.
			FreeMemChunk* next;
			FreeMemChunk* prev;
		};

		struct MemTreeChunk : public FreeMemChunk
		{
			//+hk.MemoryTracker(ignore=True)

			MemTreeChunk* child[2];
			MemTreeChunk* parent;
			BinIndex index;

				/// The leftmost child
			MemTreeChunk* leftMostChild() const { return child[0]?child[0]:child[1]; }
		};

			// Internal
		struct MemPage
		{
			//+hk.MemoryTracker(ignore=True)
				/// The previous memory block
			MemPage* m_prev;
				/// The next memory block
			MemPage* m_next;

				/// Stores the amount of allocations in this page
			int m_numAllocs;
				/// The total size as passed back from the interface
			int m_size;

				/// The payload start
			char* m_start;
				/// The payload end
			char* m_end;

				/// The first chunk in this page
			MemChunk* getFirstChunk() const { return (MemChunk*)m_start; }
				/// The last chunk in this page
			MemChunk* getFooter() const { return (MemChunk*)(m_end - MemChunk::PAYLOAD_OFFSET); }

				/// If there is nothing in the page this is the size of a chunk from the start of available space
				/// up to the footer. Its the largest free block that can be stored in a page.
			hk_size_t getMaxChunkSize() const { return  (m_end - m_start) - MemChunk::PAYLOAD_OFFSET; }

				// The contents size
			hk_size_t getContentsSize() const { return hk_size_t(m_end - m_start); }
				/// Gets the start of the contents
			char* getStart() const { return m_start;  }
				/// Gets a byte past the end
			char* getEnd() { return m_end; }
		};

			// Internal
		struct FixedMemoryBlockServer : public hkMemoryAllocator
		{
			//+hk.MemoryTracker(ignore=True)
			HK_DECLARE_PLACEMENT_ALLOCATOR();

				/// Give it a block of memory you want it to use. Will automatically align in ctor - but that means
				/// m_start may not be the start you passed in if it wasn't aligned.
			FixedMemoryBlockServer(void* start, int size);

			virtual void* bufAlloc(int& nbytes);
			virtual void bufFree(void* p, int nbytes);
			virtual void* blockAlloc(int nbytes) { HK_ASSERT(0x438afc1b, 0); return HK_NULL; }
			virtual void blockFree(void* p, int nbytes) { HK_ASSERT(0x438afc1b, 0); }
			virtual int getAllocatedSize(const void* obj, int nbytes) { return nbytes; }
			virtual void getMemoryStatistics(MemoryStatistics& s);

			inline hkBool32 inUse() { return m_start != HK_NULL; }
			hkBool resize(void* data, int oldSize, hk_size_t newSize, hk_size_t& sizeOut);

				/// Is 16 byte aligned
			char* m_start;

				/// The end address in bytes
			char* m_end;

				/// Where the limit is currently set to
			char* m_limit;
				/// The current break point (the boundary between allocated and unallocated)
			char* m_break;
		};

			/// Listener for limited memory situations.
		class LimitedMemoryListener
		{
			//+hk.MemoryTracker(ignore=True)
			public:
				HK_DECLARE_PLACEMENT_ALLOCATOR();

				virtual ~LimitedMemoryListener(){}

					/// Called when a request is denied. An implementation can free up memory, and on
					/// returning from this call the allocator will try and allocate again.
				virtual void cannotAllocate(hk_size_t size) =0;

					/// This is called when the allocator has given up allocating. Generally this will be called after
					/// a call to cannotAllocate, although that is not a requirement of an implementation.
				virtual void allocationFailure(hk_size_t size) = 0;
		};


		enum AllocType { CRITICAL_ALLOC,NORMAL_ALLOC };

			/// Used for finding all of the used blocks
		typedef void (HK_CALL *MemBlockCallback)(void* start, hk_size_t blockSize,void* param);

			/// Ctor
		hkLargeBlockAllocator(hkMemoryAllocator* server);

			/// Ctor
		hkLargeBlockAllocator(void* block, int size);

		void init(hkMemoryAllocator* server) { m_server = server; }

			/// Dtor
		~hkLargeBlockAllocator();

			// hkMemoryAllocator interface
		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );
		virtual void getMemoryStatistics( MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);

			/// Try to find the given ptr.
		hkBool isValidAlloc(void* in);

            /// Frees all allocations
        void freeAll();

            /// Called for all of the allocated chunks of memory
        void forAllAllocs(MemBlockCallback callback,void* param);


            ///// Resizes a block - is guaranteed to succeed if the size is less than or equal to the size returned by getSize.
            ///// Does not allow to make a block bigger than what was returned from getAllocSize
            ///// disabled - has a nasty bug
        //void resizeAlloc(void* p,hk_size_t newSize);

            ///// Determines if mem points to a valid block
        //hkBool checkAlloc(void* mem);

        /// Runs thru all the allocations making sure they appear valid. Will return false if any seems invalid
        hkBool checkAllocations(void** allocs,int size);

            /// Free up any unused space
        void garbageCollect();

            /// Set the limited memory listener
        void setLimitedMemoryListener(LimitedMemoryListener* listener) { m_limitedListener = listener; }

            /// Get the limited memory listener
        LimitedMemoryListener* getLimitedMemoryListener() const { return m_limitedListener; }

            /// Works out what the largest block available is
        hk_size_t findLargestBlockSize();

            /// Get the top block size
        hk_size_t getTopBlockSize() const { return m_topsize; }

			/// Returns the total amount of memory returned to the application
		hk_size_t getSumAllocatedSize() const { return m_sumAllocatedSize; }

			/// Same as getSumAllocatedSize() plus internal overhead
		hk_size_t getSumAllocatedWithOverhead() const { return m_sumAllocatedWithMgrOverhead; }

			/// Returns the total amount of memory allocated in bytes: Should be getSumAllocatedWithOverhead plus free memory with its overhead
        hk_size_t calculateMemoryUsedByThisAllocator() const;

        class Iterator
        {
			//+hk.MemoryTracker(ignore=True)
            public:
            Iterator(MemChunk* chunk, MemPage* page, MemChunk* end):
                m_chunk(chunk),
                m_page(page),
                m_end(end)
            {}
            Iterator():
                m_chunk(HK_NULL)    ///< Make invalid
            {}
            hkBool isValid() const { return m_chunk != HK_NULL; }
            hk_size_t getSize() const { return m_chunk->getChunkSize(); }
            void* getAddress() const { return m_chunk->getPayload(); }
            hkBool isInuse() const { return m_chunk->isInuse(); }

            MemChunk* m_chunk;
            MemPage* m_page;
            MemChunk* m_end;
        };

            /// Gets an iterator
            /// Doing any allocations/frees will cause undefined behaviour
            /// Blocks will be returned in order from lowest to highest in memory
        Iterator getIterator();

            /// Moves the iterator to the next block of memory. Returns false if the iterator is now invalid
        hkBool nextBlock(Iterator& iter);

            /// Get the block server
        hkMemoryAllocator* getBlockServer() const { return m_server; }

            /// Ctor
        hkLargeBlockAllocator();

#ifdef HK_DEBUG
        static void HK_CALL selfTest();
        static void HK_CALL allocatorTest(hkLargeBlockAllocator& allocator,int testSize);
#endif

	protected:
                /// Number of tree bins
        static const unsigned int NTREEBINS =  (32U);
                /// The shift used to get tree bins
        static const unsigned int TREEBIN_SHIFT = (8U);
                /// The minimum large block size
        static const hk_size_t MIN_LARGE_SIZE = hk_size_t(1)<< TREEBIN_SHIFT;

			/// Amount of bits in a size_t
        static const hk_size_t SIZE_T_BITSIZE = (sizeof(hk_size_t) << 3);

            /// Returns true if an allocated block looks correctly formed
        bool _checkUsedAlloc(const void* mem);

            /// If called after a successful free should return true
        hkBool _checkFreeChunk( MemChunk* p);

            /// The amount of memory used is cached - and returned by getTotalMemoryUsed. This method works out by traversing
            /// all the blocks how much memory is used
        hk_size_t _calculateUsed() const;

        hk_size_t _findLargestTreeBlockSize(MemTreeChunk* t,hk_size_t largest);

        void _markTreeMap(BinIndex i) { m_treemap |= (1<<i); }
        void _clearTreeMap(BinIndex i) { m_treemap &= ~(1<<i); }
        bool _isTreeMapMarked(BinIndex i) const { return (m_treemap&(1<<i))!=0; }

        void _insertLargeChunk( MemTreeChunk* x, hk_size_t s);
        void _unlinkLargeChunk( MemTreeChunk* x);

        void* _allocLarge( hk_size_t nb);
        void* _allocFromTop(hk_size_t bytes);

        void _makeTopValid() const;
        void* _alloc(hk_size_t nb);

        void _init();

            // How the hades does this work?
        static BinIndex _computeTreeIndex(hk_size_t s)
        {
            hk_size_t x = s >> TREEBIN_SHIFT;
                if (x == 0) { return 0; }
                if (x > 0xFFFF) { return NTREEBINS-1; }

            unsigned int y = (unsigned int)x;
            unsigned int n = ((y - 0x100) >> 16) & 8;
            unsigned int k = (((y <<= n) - 0x1000) >> 16) & 4;
            n += k;
            n += k = (((y <<= k) - 0x4000) >> 16) & 2;
            k = 14 - n + ((y <<= k) >> 15);
            return (BinIndex)((k << 1) + ((s >> (k + (TREEBIN_SHIFT-1)) & 1)));
        }

        static BinIndex _bitToIndex(unsigned int x)
        {
            unsigned int y = x - 1;
            unsigned int k = y >> (16-4) & 16;
            unsigned int n = k;        y >>= k;
            n += k = y >> (8-3) &  8;  y >>= k;
            n += k = y >> (4-2) &  4;  y >>= k;
            n += k = y >> (2-1) &  2;  y >>= k;
            n += k = y >> (1-0) &  1;  y >>= k;
            return n+y;
        }

			/// pad request bytes into a usable size
        HK_FORCE_INLINE static hk_size_t _padRequest(hk_size_t req)
        {
            return (req + MemChunk::PAYLOAD_OFFSET + MemChunk::ALIGN_MASK)&~MemChunk::ALIGN_MASK;
        }

                /// Bit representing maximum resolved size in a treebin at i
        static int _bitForTreeIndex(int i)
        {
            return (i == NTREEBINS-1)? (SIZE_T_BITSIZE-1) : (((i) >> 1) + TREEBIN_SHIFT - 2);
        }
                /// Shift placing maximum resolved bit in a treebin at i as sign bit
        static int _leftShiftForTreeIndex(int i)
        {
            return ((i == NTREEBINS-1)? 0 : ((SIZE_T_BITSIZE-hk_size_t(1)) - ((i >> 1) + TREEBIN_SHIFT - 2)));
        }
                /// The size of the smallest chunk held in bin with index i
        static hk_size_t _minSizeForTreeIndex(int i)
        {
            return ((hk_size_t(1) << (((i) >> 1) + TREEBIN_SHIFT)) |  (((hk_size_t)((i) & hk_size_t(1))) << (((i) >> 1) + TREEBIN_SHIFT - 1)));
        }

            /// Tries to resize the page allocated from a SingleBlockServer. If successful will fix up the top block etc
        hkBool _resizeSingleBlockServerPage(hk_size_t newSize);

            /// Returns true if n is greater than p
        template <typename S,typename T>
        static bool _okNext(const S* p,const T* n) { return ((const char*)(p) < (const char*)(n)); }

            /// Does noddy checks to see if an address is possibly correct
        HK_FORCE_INLINE bool _isOkAddress(const void *a)const  { return ((char*)(a) >= m_leastAddr); }

            /// isolate the least set bit of a bitmap
        HK_FORCE_INLINE static int _leastBit(int x) { return ((x) & -(x)); }

            /// mask with all bits to left of least bit of x on
        HK_FORCE_INLINE static int _leftBits(int x) { return ((x<<1) | -(x<<1)); }

            /// An index to a set bit
        HK_FORCE_INLINE static int _indexToBit(int x) { return 1<<x; }

            /// mask with all bits to left of or equal to least bit of x on
        HK_FORCE_INLINE static int _sameOrLeftBits(int x) { return ((x) | -(x)); }

        static HK_FORCE_INLINE hkBool _comparePointers( void* a,void* b)
        {
            return (char*)a < (char*)b;
        }

            /// True if there are no allocated pages
        hkBool _hasPages() { return m_pages.m_next != &m_pages; }

        static void HK_CALL _addAlloc(void* data,hk_size_t size,void* param);

	protected:
		    // A listener which handles situations when memory allocation gets tight
        LimitedMemoryListener* m_limitedListener;

			// A bit field with a bit indicating if a tree bucket has contents or not
        BinMap m_treemap;

			//
        char* m_leastAddr;

			// The 'topmost memory' chunk
        MemChunk* m_top;

			// Cached size of topmost block
        hk_size_t m_topsize;

			// The tree bins
        MemTreeChunk* m_treebins[NTREEBINS];

			// Fixed block memory, if using. Check with inUse() method.
		FixedMemoryBlockServer m_fixedBlockServer;

			// Server for getting large memory blocks. Either points to internal fixed memory block or to another allocator.
        hkMemoryAllocator* m_server;

			// Used as top when starting up
		MemChunk m_zeroChunk;

			// This dummy page is the start of a doubly linked list of memory pages allocated through the server interface
		MemPage m_pages;

			// The total amount of allocated memory with overhead
		hk_size_t m_sumAllocatedWithMgrOverhead;

			/// The sum of all allocated memory
		hk_size_t m_sumAllocatedSize;		
};

#endif // HK_LARGE_BLOCK_ALLOCATOR_H

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
