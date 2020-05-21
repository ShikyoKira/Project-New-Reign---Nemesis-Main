/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLOCK_LIST_H
#define HK_BLOCK_LIST_H


/// A container consisting of a list of same-sized blocks holding Ts.
/// The list is written to using a BatchWriter, and then read from
/// using a BatchReader. No other use-case is supported.
template<typename T, int BLOCK_SIZE>
class hkBlockList
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkBlockList );

	public:
		struct Block
		{
			enum 
			{ 
				NUM_ELEMENTS_IN_BLOCK = ( BLOCK_SIZE - hkSizeOf( Block* ) ) / hkSizeOf( T )
			};

			HK_FORCE_INLINE void initialize() 
			{ 
				this->m_next = HK_NULL; 
				HK_ON_DEBUG( hkString::memSet( m_data, 0xcf, NUM_ELEMENTS_IN_BLOCK * hkSizeOf( T ) ) );
			}

				/// The link to the next block.
			HK_ALIGN16( Block* m_next );

				/// The data contained in the block.
			T m_data[NUM_ELEMENTS_IN_BLOCK];
		};

	public:
			/// Create a list with no blocks.
		hkBlockList();

#if defined(HK_DEBUG)
			/// Return the number of blocks in the list.
		int getTotalNumElems() { return m_numTotalElements; }

			/// Assert that the list has no blocks.
		void checkEmpty();
#endif

	public:
		
		class BatchWriter
		{
			public:
				HK_FORCE_INLINE BatchWriter( HK_ON_SPU( int dmaGroup ) );
				
					/// Sets the writer to the start of an empty list
				HK_FORCE_INLINE void setToStartOfList( hkBlockList* blockList );

					/// Write a batch of same-sized elements into the list.
				HK_FORCE_INLINE void writeBatch( const T* data, int numElements );

					/// Finish up.
				HK_FORCE_INLINE void finalize();

			protected:
					/// A new block is return which is added after lastBlock.
				HK_FORCE_INLINE Block* addBlockAtEnd( Block* lastBlock );

			public:
				hkPadSpu<hkBlockList<T, BLOCK_SIZE> *> m_blockList;

					/// The current block (a PPU pointer when on SPU)
				hkPadSpu<Block*> m_currentBlock;
				
					/// The index of the next element at which writing will be done.
				hkPadSpu<int> m_currentElement;

					/// The last block written to (a PPU pointer when on SPU).
				hkPadSpu<Block*> m_prevBlock;
		};

		class BatchConsumer
		{
			public:
				HK_FORCE_INLINE BatchConsumer( HK_ON_SPU( int dmaGroup ) );

				HK_FORCE_INLINE ~BatchConsumer();

					/// Sets the reader to the start of a list
				HK_FORCE_INLINE void setToStartOfList( hkBlockList* list );

					/// Deallocate the last block in the list if the consumer hasn't
					/// done so already.
				HK_FORCE_INLINE void finalize();

					/// Set the maximum number of elements to obtain.
				HK_FORCE_INLINE void setNumElements( int numElements );

					/// Get the pointer to and return the count of the next batch.
					/// \param numElementsInBatch is zero when it has reached the end.
				HK_FORCE_INLINE	int accessBatch( T*& dataOut );

					/// Get a pointer to the remaining elements in the current block (use 0xcd as terminator)
				HK_FORCE_INLINE	int getElements( T*& dataOut );

			protected:
				/// Remove the first block.
				HK_FORCE_INLINE void removeBlockFromStart( Block* firstBlock, Block* secondBlock );
		
			protected:
				hkPadSpu<hkBlockList<T, BLOCK_SIZE> *> m_blockList;

				/// The current block. This is a PPU pointer on SPU.
				hkPadSpu<Block*> m_currentBlock;

					/// The index to the next element to read within the current block.
				hkPadSpu<int> m_currentElement;
				
					/// Cause accessBatch to return only this many elements.
				hkPadSpu<int> m_numElementsToRead;
		};

	protected:
			/// The first block in the list.
			/// This is invalidated by the consumer (in release builds).
		HK_ALIGN16( Block* m_firstBlock ); 

			/// The number of blocks in the list (debug only)
		int m_numBlocks;

			/// The number of Ts in the list (debug only)
		int m_numTotalElements;
};

#include <Common/Base/Container/BlockList/hkBlockList.inl>

#endif // HK_BLOCK_LIST_H

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
