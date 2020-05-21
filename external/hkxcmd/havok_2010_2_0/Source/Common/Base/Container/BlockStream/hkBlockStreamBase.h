/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLOCKSTREAM_BASE_H
#define HK_BLOCKSTREAM_BASE_H

#include <Common/Base/Container/BlockStream/hkBlockStreamBlock.h>
#include <Common/Base/Container/BlockStream/hkBlockStreamHeapAllocator.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>


//	when a block is allocated, it keeps the ptr in this class. 
//	when hkBlockStream is destroyed, all the blocks allocated by this stream is freed.

// When the default allocator is used, NULL can be passed as a valid argument to functions expecting an allocator.
template<typename Allocator = hkBlockStreamHeapAllocator<512> >
class hkBlockStreamBase
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, hkBlockStreamBase );

		typedef hkBlockStreamBlock<Allocator::BLOCK_SIZE> Block;

		class Marker;

		hkBlockStreamBase( Allocator* tlAllocator, bool zeroNewBlocks = false );

		HK_FORCE_INLINE ~hkBlockStreamBase();

			///	resets the stream after calling initialize, block stream has 1 empty block.
		void initialize( Allocator* HK_RESTRICT tlAllocator );

			/// append blockstream to this stream and delete inStream
		void append( Allocator* HK_RESTRICT tlAllocator, hkBlockStreamBase* inStream );

			/// append blockstream to this stream multi threaded safe
		void appendMt( Allocator* HK_RESTRICT tlAllocator, hkBlockStreamBase* inStream );

			/// append a list of ranges, creates lots of holes but is really fast
		void append( Allocator* HK_RESTRICT tlAllocator, Marker* ranges, int numRanges, int rangeStriding );

		HK_FORCE_INLINE bool isEmpty() const;

		HK_FORCE_INLINE int getTotalNumElems() const;

		void checkConsistency() const;

			//	clears the stream, does not reset it, only use if you call append immediately after this
		void clear( Allocator* HK_RESTRICT tlAllocator );


	protected:

			/// return the first block
		Block* begin(){ return m_blocks[0]; }

		const Block* begin() const { return m_blocks[0]; }

		Block* last() { return m_blocks[ m_blocks.getSize()-1 ]; }

		const Block* last() const { return m_blocks[ m_blocks.getSize()-1 ]; }

			/// allocate and append a new block
		Block* blockAlloc(Allocator* tlAllocator);

			/// free the last block
		Block* popBack( Allocator* tlAllocator );

		HK_FORCE_INLINE void freeBlockWithIndex(Allocator* HK_RESTRICT tlAllocator, Block* HK_RESTRICT block, int index);

		HK_FORCE_INLINE void finalizeWrite(Block* HK_RESTRICT thisBlock, Block* HK_RESTRICT nextBlock, int numDatas, int bytesUsed );

	public:

		//	WriteIterator			:	can write any sized elements. Slow
		//	FixedSizeWriteIterator	:	only writes fixed sized elements. Fast

		class WriteIterator
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, WriteIterator );

				HK_FORCE_INLINE WriteIterator( HK_ON_SPU(int dmaGroup = HK_DEFAULT_BLOCK_STREAM_DMA_GROUP) );

				HK_FORCE_INLINE ~WriteIterator();

				HK_FORCE_INLINE void setToEndOfStream( Allocator* allocator, hkBlockStreamBase* blockStream);

				HK_FORCE_INLINE void setToStartOfStream( Allocator* allocator, hkBlockStreamBase* blockStream);
				
				HK_FORCE_INLINE	void* advanceAndReserveNext( int thisElemSize, int nextElemSize );

				HK_FORCE_INLINE	void* access( int elementSize  );

				HK_FORCE_INLINE void advance( int elemSize );

					/// Needs to be called to make sure all the data gets written to the stream
				HK_FORCE_INLINE void finalize();

					/// return the total number of elements in a stream
				HK_FORCE_INLINE int getTotalNumElems();

				HK_FORCE_INLINE void hackMarkFinalized();

				HK_FORCE_INLINE void checkConsistency(){ HK_ON_DEBUG( if (m_blockStream) { m_blockStream->checkConsistency(); } ) }

				hkPadSpu<hkBlockStreamBase*> m_blockStream;
			protected:
				void *accessNextBlock( );
			protected:
				friend class Marker;
				hkPadSpu<Block*> m_currentBlock;
				hkPadSpu<int> m_currentByteLocation;
				hkPadSpu<int> m_currentBlockNumElems;
				hkPadSpu<Allocator*> m_tlAllocator;
				hkBool m_finalized;	///< for debugging
				int m_accessSize;	///< for debugging: the size used in access()
		};

		/// write iterator for fixed size objects, allows for better type checking and simpler accessors
		template<typename T>
		struct FixedSizeWriteIterator : WriteIterator
		{

			HK_FORCE_INLINE T* access(){ return (T*)WriteIterator::access( sizeof(T)); }

			HK_FORCE_INLINE void advance(){ WriteIterator::advance( sizeof(T) ); }
		};


		class ReadIterator
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, ReadIterator );


				HK_FORCE_INLINE void setToStartOfStream( const hkBlockStreamBase* stream );
				
				HK_FORCE_INLINE void setToMarker( const hkBlockStreamBase* stream, const Marker* marker );

					/// advance and access the next element, assumes the current entry is valid
				HK_FORCE_INLINE	const void* advanceAndAccessNext( int thisElemSize );

				HK_FORCE_INLINE	const void* access();

				template<int SIZE>
				HK_FORCE_INLINE	void prefetch(int offset) const;

				HK_FORCE_INLINE ~ReadIterator();

			protected:
				const void* advanceToNewBlock( );

			protected:
				friend class Marker;
				hkPadSpu<const Block*>		m_currentBlock;
				hkPadSpu<const char*>   m_currentByteLocation;
				hkPadSpu<int> m_numElementsToReadInOtherBlocks;
				hkPadSpu<int> m_numElementsToReadInThisBlock;
				hkPadSpu<const hkBlockStreamBase*> m_blockStream;
		};

		class ModifyIterator : public ReadIterator
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, ModifyIterator );


				HK_FORCE_INLINE void setToStartOfStream( Allocator* allocator, hkBlockStreamBase* stream );

				HK_FORCE_INLINE void setToMarker( Allocator* allocator, hkBlockStreamBase* stream, const Marker* marker );

					/// advance and access the next element, freeing the old data, assumes the current entry is valid
				HK_FORCE_INLINE	const void* advanceFreeAndAccessNext( int thisElemSize );

				HK_FORCE_INLINE	void* advanceAndAccessNextRw( int thisElemSize )
				{ return const_cast<void*>( hkBlockStreamBase<Allocator>::ReadIterator::advanceAndAccessNext(thisElemSize) );}

				HK_FORCE_INLINE	void* accessRw(){ return const_cast<void*>( hkBlockStreamBase<Allocator>::ReadIterator::access() );}

			protected:
				const void* advanceToNewBlock( );

					/// free current block
				const void* freeAndAdvanceToNewBlock( );

				// Non-const casting
				Block* getCurrentBlock() { return const_cast<Block*>( this->m_currentBlock.val() ); }
				char* getCurrentByteLocation() { return const_cast<char*>( this->m_currentByteLocation.val() ); }
				hkBlockStreamBase* getBlockStream() { return const_cast<hkBlockStreamBase*>( this->m_blockStream.val() ); }

			protected:
				friend class Marker;
				hkPadSpu<int> m_numElementsToFreeInThisBlock;
				hkPadSpu<Allocator*> m_tlAllocator;
		};


		class Marker
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, Marker );

				HK_FORCE_INLINE	Marker(): m_numElements(0) {}

				HK_FORCE_INLINE	void clear(){ m_startBlock = HK_NULL; m_numElements = 0; }

				HK_FORCE_INLINE bool isEmpty(){ return m_numElements == 0; }

				HK_FORCE_INLINE int getNumElements() const { return m_numElements; }

				HK_FORCE_INLINE void setNumElements(int num) { m_numElements = num; }

				HK_FORCE_INLINE void setStartPoint(const typename hkBlockStreamBase<Allocator>::WriteIterator* HK_RESTRICT it );

				HK_FORCE_INLINE void setEndPoint(const typename hkBlockStreamBase<Allocator>::WriteIterator* HK_RESTRICT it);

				HK_FORCE_INLINE void setEmpty();
				
				HK_FORCE_INLINE void setEntireStream(hkBlockStreamBase* HK_RESTRICT stream );


			public:
				friend class hkBlockStreamBase;
				friend class ReadIterator;

				hkBlockStreamBlock<Allocator::BLOCK_SIZE>* m_startBlock;

				typename Block::CountType m_startByteLocation;
				typename Block::CountType m_startElemIdxInBlock;
				int m_numElements;

		};


		HK_ALIGN16( hkBool  m_zeroNewBlocks );			// if set, all new blocks will be zeroed

	protected:
		typename Allocator::UnderlyingAllocator* m_allocator;

		hkBool  m_partiallyFreed;			// debugging
#if defined(HK_DEBUG)
		hkBool	m_isLocked;					// locked by writers and readers
#else
		hkBool	m_isLockedUnused;	
#endif
		int		m_numTotalElements;

		hkInplaceArray< Block*, 24 > m_blocks;
};

#include <Common/Base/Container/BlockStream/hkBlockStreamBase.inl>

#endif // HK_BLOCKSTREAM_BASE_H

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
