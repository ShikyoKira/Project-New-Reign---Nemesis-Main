/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template<typename Allocator>
void hkBlockStreamBase<Allocator>::clear(Allocator* HK_RESTRICT tlAllocator)
{
	HK_ASSERT( 0xf0345dff, !m_isLocked );
	int size = m_blocks.getSize();

	if ( m_partiallyFreed )
	{
		// compress used blocks
		Block** src = m_blocks.begin();
		Block** dst = src;
		for (int i =0; i < size; i++)
		{
			if ( *src )
			{
				*(dst++) = *(src);
			}
			src++;
		}
		size = (int)(dst - m_blocks.begin());
	}

	if ( size )
	{
		tlAllocator->blockFreeBatch( m_blocks.begin(),  size );
	}
	m_blocks.clear();
	m_numTotalElements = 0;

	m_partiallyFreed = false;
	HK_ON_DEBUG(checkConsistency());
}

template<typename Allocator>
typename hkBlockStreamBase<Allocator>::Block* hkBlockStreamBase<Allocator>::blockAlloc(Allocator* HK_RESTRICT tlAllocator)
{
	HK_ASSERT( 0xf034defd, tlAllocator->getUnderlyingAllocator() == m_allocator );
	Block* block = tlAllocator->blockAlloc();

	// zero if needed
	if ( m_zeroNewBlocks )
	{
		HK_ASSERT( 0xf0fdde34, (hkUlong(block) & 0x7f) == 0);
		hkString::memClear128( block, sizeof(Block) );
		HK_ON_DEBUG( block->m_allocator = m_allocator );
	}
	else
	{
		block->initialize();
	}
	block->m_blockIndexInStream = m_blocks.getSize();
	HK_ON_DEBUG( block->m_debugBlockStream = this );
	m_blocks.pushBack( block );

	return block;
}


template<typename Allocator>
void hkBlockStreamBase<Allocator>::freeBlockWithIndex(Allocator* HK_RESTRICT tlAllocator, Block* HK_RESTRICT block, int index)
{
	HK_ASSERT( 0xf03df1d8, block->m_blockIndexInStream == index && block->m_debugBlockStream == this && m_blocks[index] == block );
	tlAllocator->blockFree( block );
	m_blocks[index] = 0;
	m_partiallyFreed = true;
}

template<typename Allocator>
typename hkBlockStreamBase<Allocator>::Block* hkBlockStreamBase<Allocator>::popBack(Allocator* HK_RESTRICT tlAllocator)
{
	HK_ASSERT( 0xf034defd, tlAllocator->getUnderlyingAllocator() == m_allocator );
	Block* lastBlock = m_blocks[ m_blocks.getSize()-1 ];
	HK_ASSERT( 0xf034deff, lastBlock->m_blockIndexInStream == m_blocks.getSize()-1 && lastBlock->m_debugBlockStream == this );
	Block* prevLast  = m_blocks[ m_blocks.getSize()-2 ];
	HK_ASSERT(0xf034fddf, prevLast->m_next == lastBlock);
	tlAllocator->blockFree( lastBlock );
	m_blocks.popBack();
	prevLast->m_next = HK_NULL;

	HK_ON_DEBUG(checkConsistency());
	return prevLast;
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::initialize(Allocator* HK_RESTRICT tlAllocator)
{
	HK_ASSERT( 0xf034defd, tlAllocator->getUnderlyingAllocator() == m_allocator );

	clear(tlAllocator);
	blockAlloc(tlAllocator); 

	HK_ON_DEBUG(checkConsistency());
}

template<typename Allocator>
hkBlockStreamBase<Allocator>::hkBlockStreamBase( Allocator* HK_RESTRICT tlAllocator, bool zeroNewBlocks )
{ 
	//HK_ASSERT2( 0xf0343234, tlAllocator->getUnderlyingAllocator() == allocator, "Your thread local allocator does not match the allocator" );
	m_allocator = tlAllocator->getUnderlyingAllocator();
	m_numTotalElements = 0;
	HK_ON_DEBUG(m_isLocked = false);
	m_partiallyFreed = false;
	m_zeroNewBlocks = zeroNewBlocks;
	blockAlloc(tlAllocator); 

	HK_ON_DEBUG(checkConsistency());
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::append( Allocator* HK_RESTRICT tlAllocator, hkBlockStreamBase* HK_RESTRICT inStream )
{
	HK_ASSERT( 0xf034defd, tlAllocator->getUnderlyingAllocator() == m_allocator );

	HK_ASSERT( 0xf0345dfe, !m_isLocked && !inStream->m_isLocked && !m_partiallyFreed && !inStream->m_partiallyFreed);
	if( inStream->isEmpty() )
	{
		if ( m_blocks.getSize() == 0)	// just cleared so we need to create a dummy block
		{
			blockAlloc(tlAllocator); 
		}
		inStream->clear( tlAllocator );
		HK_ON_DEBUG(checkConsistency());
		return;
	}

	if ( m_blocks.getSize() == 0)	// just cleared so we can append
	{

	}
	else if( isEmpty() )
	{
		tlAllocator->blockFree( m_blocks[0] );
		m_blocks.clear();
	}
	else
	{
		//	modify nextPtr
		Block* prevLast = last();
		HK_ASSERT(0xf0345456, !prevLast->m_next && prevLast->m_numElements);
		prevLast->m_next = inStream->begin();
	}
	int oldSize = m_blocks.getSize();
	m_blocks.insertAt( oldSize, inStream->m_blocks.begin(), inStream->m_blocks.getSize() );
	inStream->m_blocks.clear();

	for (int i = oldSize; i< m_blocks.getSize(); i++)
	{
		m_blocks[i]->m_blockIndexInStream = i;
		HK_ON_DEBUG( m_blocks[i]->m_debugBlockStream = this );
	}
	m_numTotalElements += inStream->m_numTotalElements;
	inStream->m_numTotalElements = 0;

	HK_ON_DEBUG(checkConsistency());

	return;
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::append( Allocator* HK_RESTRICT tlAllocator, Marker* HK_RESTRICT ranges, int numRanges, int rangeStriding )
{
	HK_ASSERT( 0xf034defd, tlAllocator->getUnderlyingAllocator() == m_allocator );

	Marker* range = ranges;
	for (int i =0; i < numRanges; range = hkAddByteOffset( range, rangeStriding ), i++)
	{
		if ( range->isEmpty() )
		{
			continue;
		}

		if ( m_blocks.getSize() == 0)	// just cleared so we can append
		{

		}
		else if( isEmpty() )
		{
			tlAllocator->blockFree( m_blocks[0] );
			m_blocks.clear();
		}
		else
		{
			// see if we can fit the combined data
			//	modify nextPtr
			Block* lastBlock = last();
			HK_ASSERT(0xf0345456, !lastBlock->m_next && lastBlock->m_numElements);

			int bytesUsed = range->m_startBlock->m_bytesUsed - range->m_startByteLocation;
			if ( lastBlock->m_bytesUsed + bytesUsed <= Block::BLOCK_DATA_SIZE )
			{
				void* src = hkAddByteOffset(range->m_startBlock->begin(), range->m_startByteLocation );
				void *dst = hkAddByteOffset(lastBlock->begin(), lastBlock->m_bytesUsed );
				hkString::memCpy16NonEmpty( dst, src, bytesUsed );
				lastBlock->m_bytesUsed = Block::CountType(lastBlock->m_bytesUsed + Block::CountType(bytesUsed));
				m_numTotalElements += range->getNumElements();
			}
			else
			{
				if ( range->m_startByteLocation > 0)
				{
					// create new block 
					blockAlloc( tlAllocator );

				}
				lastBlock->m_next = range->m_startBlock;
			}
		}
		//m_blocks.insertAt( m_blocks.getSize(), range->m_startBlock, range->getNumBlocks() );
		HK_ASSERT(0x680e156b,0);	// ranges are not freed yet, but the blocks are partially appended
		//inStream->m_blocks.clear();
	}

	HK_ON_DEBUG(checkConsistency());
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::appendMt( Allocator* HK_RESTRICT tlAllocator, hkBlockStreamBase* HK_RESTRICT inStream )
{
	hkReferencedObject::lockAll();
	append( tlAllocator, inStream );
	HK_ON_DEBUG(checkConsistency());
	hkReferencedObject::unlockAll();
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::checkConsistency() const
{
#if defined(HK_DEBUG) && !defined(HK_PLATFORM_SPU)
	for (int bi = 0; bi < m_blocks.getSize()-1; bi++)
	{
		const Block* block = m_blocks[bi];
		HK_ASSERT2(0xad731113, 0 < block->m_numElements, "Empty blocks found withing stream. This will cause asserts "
			"with the iterators." );
		HK_ASSERT( 0xf0ccfe34, block->m_blockIndexInStream == bi );
		HK_ASSERT( 0xf0ccfe37, block->m_debugBlockStream	== this );
		HK_ASSERT( 0xf0ccfe35, block->m_numAllocatedElems == block->m_numElements );
		HK_ASSERT( 0xf0ccfe37, block->m_allocator == m_allocator );
	}
#endif
}


template<typename Allocator>
const void* hkBlockStreamBase<Allocator>::ReadIterator::advanceToNewBlock( )
{
	m_currentBlock = m_currentBlock->m_next;
	if( !m_currentBlock )
	{
		m_currentByteLocation = HK_NULL;
		return HK_NULL;
	}
	int numElementsToReadInThisBlock = hkMath::min2((int)m_currentBlock->m_numElements, (int)m_numElementsToReadInOtherBlocks.val());
	if( numElementsToReadInThisBlock<=0 )
	{
		m_currentByteLocation = HK_NULL;
		return HK_NULL;
	}
	m_numElementsToReadInThisBlock = numElementsToReadInThisBlock;
	m_numElementsToReadInOtherBlocks = m_numElementsToReadInOtherBlocks - m_numElementsToReadInThisBlock;
	m_currentByteLocation = (const char*)m_currentBlock->begin();

	HK_ASSERT( 0xf06576df, m_numElementsToReadInThisBlock > 0 );
	return m_currentBlock->begin();
}

template<typename Allocator>
const void* hkBlockStreamBase<Allocator>::ModifyIterator::advanceToNewBlock( )
{
	ReadIterator::advanceToNewBlock();

	this->m_numElementsToFreeInThisBlock = this->m_numElementsToReadInThisBlock;

	return this->m_currentBlock->begin();
}

template<typename Allocator>
const void* hkBlockStreamBase<Allocator>::ModifyIterator::freeAndAdvanceToNewBlock( )
{
	Block* currentBlock = getCurrentBlock();

	Block* HK_RESTRICT toFree = getCurrentBlock();
	this->m_currentBlock = currentBlock->m_next;

	if ( int(currentBlock->m_numAllocatedElems) == this->m_numElementsToFreeInThisBlock )
	{
		getBlockStream()->freeBlockWithIndex( m_tlAllocator, toFree, currentBlock->m_blockIndexInStream );
	}
	else
	{
		hkCriticalSection::atomicExchangeAdd( &currentBlock->m_numAllocatedElems, hkUint32(-this->m_numElementsToFreeInThisBlock) );
		// Note: there is still a small chance that 2 threads will not free a block, but the blocks will be free later anyway
	}

	if( !this->m_currentBlock )
	{
		this->m_currentByteLocation = HK_NULL;
		return HK_NULL;
	}

	currentBlock = getCurrentBlock();

	int numElementsToReadInThisBlock = hkMath::min2<int>((int)currentBlock->m_numElements, this->m_numElementsToReadInOtherBlocks.val());
	HK_ASSERT( 0x24fe8bfb, numElementsToReadInThisBlock >= 0 );
	
	if( numElementsToReadInThisBlock == 0 )
	{
		this->m_currentByteLocation = HK_NULL;
		return HK_NULL;
	}

	this->m_numElementsToReadInThisBlock = numElementsToReadInThisBlock;
	this->m_numElementsToFreeInThisBlock = numElementsToReadInThisBlock;
	this->m_numElementsToReadInOtherBlocks = this->m_numElementsToReadInOtherBlocks - this->m_numElementsToReadInThisBlock;
	this->m_currentByteLocation = (const char*)currentBlock->begin();

	return currentBlock->begin();
}

template<typename Allocator>
void* hkBlockStreamBase<Allocator>::WriteIterator::accessNextBlock()
{
	Block* nextBlock = this->m_blockStream->blockAlloc( this->m_tlAllocator );
	this->m_blockStream->finalizeWrite( this->m_currentBlock, nextBlock, this->m_currentBlockNumElems, this->m_currentByteLocation );
	this->m_currentBlock = nextBlock; 
	this->m_currentByteLocation = 0;
	this->m_currentBlockNumElems = 0;
	return nextBlock->begin();
}

template<typename Allocator>
bool hkBlockStreamBase<Allocator>::isEmpty() const
{
	HK_ASSERT2(0xf034dfdf, !this->m_isLocked, "You cannot use isEmpty() while writing to it (call writer::finalize() first.");
	return this->m_numTotalElements== 0;
}

template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::finalizeWrite(Block* HK_RESTRICT thisBlock, Block* HK_RESTRICT nextBlock, int thisBlockNumElements, int bytesUsed )
{
	thisBlock->m_next = nextBlock;
	//int newElements = thisBlockNumElements - thisBlock->m_numElements;
	thisBlock->m_numElements = typename Block::CountType(thisBlockNumElements);
	thisBlock->m_numAllocatedElems = thisBlockNumElements;
	thisBlock->m_bytesUsed   = typename Block::CountType(bytesUsed);
	this->m_numTotalElements += thisBlockNumElements;
}

template<typename Allocator>
int hkBlockStreamBase<Allocator>::getTotalNumElems() const
{
	HK_ASSERT2( 0xf032defd, this->m_isLocked == false, "You cannot call this function, when non-finalized write itrators are "
												 "linked to the stream.");
	HK_ASSERT2( 0xf032defd, this->m_partiallyFreed == false, "You cannot call this function when some blocks are freed "
													   "by a hkBlockStream::Consumer" );
	return this->m_numTotalElements;
}


template<typename Allocator>
hkBlockStreamBase<Allocator>::~hkBlockStreamBase()
{
	HK_ASSERT2( 0xf03454ed, this->m_blocks.getSize()==0, "You must call clear before the destructor" );
	//clear();
}


//	write iterator

template<typename Allocator>
hkBlockStreamBase<Allocator>::WriteIterator::WriteIterator( HK_ON_SPU(int dmaGroup) )
{
	m_blockStream = HK_NULL;

	HK_ON_DEBUG(m_finalized = true);
	HK_ON_DEBUG(m_accessSize = 0);
	HK_ON_DEBUG( m_currentBlock = HK_NULL);

}

template<typename Allocator>
hkBlockStreamBase<Allocator>::WriteIterator::~WriteIterator()
{
	HK_ASSERT(0xf034dedf, m_finalized);
}

template<typename Allocator>
HK_FORCE_INLINE int hkBlockStreamBase<Allocator>::WriteIterator::getTotalNumElems()
{
	HK_ON_DEBUG(m_finalized = false);
	return m_blockStream->m_numTotalElements + m_currentBlockNumElems;
}


template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::WriteIterator::setToEndOfStream( Allocator* allocator, hkBlockStreamBase* blockStream)
{
	Block* lastBlock = blockStream->last();
	HK_ASSERT( 0xf0345dfe, !blockStream->m_isLocked );
	HK_ON_DEBUG( blockStream->m_isLocked = true );
	m_tlAllocator			= allocator;
	m_blockStream			= blockStream;
	m_currentBlock			= lastBlock;
	m_currentByteLocation	= lastBlock->m_bytesUsed;
	m_currentBlockNumElems  = lastBlock->m_numElements;
	blockStream->m_numTotalElements = blockStream->m_numTotalElements - m_currentBlockNumElems;
	HK_ASSERT( 0xf0dfde34, blockStream->m_numTotalElements >= 0 );
	HK_ON_DEBUG(m_finalized = false);
}

template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::WriteIterator::setToStartOfStream( Allocator* allocator, hkBlockStreamBase* blockStream)
{
	HK_ASSERT( 0xf0345dfe, !blockStream->m_isLocked );
	HK_ASSERT( 0xf0456567, blockStream->isEmpty() && blockStream->m_numTotalElements == 0 );
	HK_ON_DEBUG( blockStream->m_isLocked = true );
	Block* firstBlock = blockStream->begin();
	hkMath::prefetch128( firstBlock );
	m_tlAllocator			= allocator;
	m_blockStream			= blockStream;
	m_currentBlock			= firstBlock;
	m_currentByteLocation	= 0;
	m_currentBlockNumElems  = 0;
	HK_ON_DEBUG(m_finalized = false);
}

template<typename Allocator>
void* hkBlockStreamBase<Allocator>::WriteIterator::access( int elementSize )
{
	HK_ON_DEBUG(m_accessSize = elementSize);
	 
	int nextLocation = m_currentByteLocation + elementSize;
	if( nextLocation <= Block::BLOCK_DATA_SIZE )
	{
		return hkAddByteOffset( m_currentBlock->begin(), m_currentByteLocation );
	}
	else
	{
		return accessNextBlock();
	}
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::WriteIterator::advance( int elemSize )
{
	HK_ASSERT( 0xf03454df, elemSize <= m_accessSize );
	HK_ON_DEBUG( m_accessSize = 0 );
	int nextLocation = m_currentByteLocation + elemSize;
	int numElements = m_currentBlockNumElems;
	m_currentByteLocation = nextLocation;
	m_currentBlockNumElems = numElements+1;
}

template<typename Allocator>
void* hkBlockStreamBase<Allocator>::WriteIterator::advanceAndReserveNext( int thisElemSize, int nextElemSize )
{
	advance( thisElemSize );
	return access( nextElemSize );
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::WriteIterator::finalize()
{
	//	when blockStream is created, it creates an empty block. Dont pop back this case
	if( m_currentBlockNumElems == 0 && m_blockStream->m_blocks.getSize() > 1 )
	{
		m_currentBlock = m_blockStream->popBack( m_tlAllocator );
	}
	else
	{
		m_blockStream->finalizeWrite( m_currentBlock, HK_NULL, m_currentBlockNumElems, m_currentByteLocation );
	}
	HK_ON_DEBUG(m_finalized = true);
	HK_ASSERT( 0xf0445dfe, m_blockStream->m_isLocked );
	HK_ON_DEBUG( m_blockStream->m_isLocked = false );
	HK_ON_DEBUG(m_blockStream->checkConsistency());
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::WriteIterator::hackMarkFinalized()
{
	HK_ON_DEBUG(m_finalized = true);
// 	HK_ASSERT( 0xf0445dfe, m_blockStream->m_isLocked );
// 	HK_ON_DEBUG( m_blockStream->m_isLocked = false );
}
// template<int SIZE>
// void hkBlockStream::WriteIterator::prefetch(int offset) const
// {
// 	hkMath::forcePrefetch<SIZE>( hkAddByteOffset(m_currentBlock->begin(), m_currentByteLocation+offset) );
// }

//////////////////////////////////////////////////////////////////////////
//
// Read Iterator
//
//////////////////////////////////////////////////////////////////////////

template<typename Allocator>
void hkBlockStreamBase<Allocator>::ReadIterator::setToMarker( const hkBlockStreamBase* HK_RESTRICT stream, const Marker* HK_RESTRICT marker )
{
	if (HK_NULL == marker->m_startBlock || 0 == marker->m_numElements)
	{
		m_currentBlock = HK_NULL;
		m_blockStream = HK_NULL;
		m_currentByteLocation = HK_NULL;

		m_numElementsToReadInThisBlock	 = 0;
		m_numElementsToReadInOtherBlocks = 0;

		return;
	}

	//HK_ASSERT( 0xf0dfde39, marker->m_startBlock != HK_NULL );
	//HK_ASSERT( 0xf0dfde39, marker->m_startBlock->m_numElements != 0 );
	//HK_ASSERT( 0xf0dfde39, marker->m_numElements > 0 );

	m_currentBlock			= marker->m_startBlock;
	m_currentByteLocation   = ((char*)marker->m_startBlock->begin()) + marker->m_startByteLocation;
	m_blockStream			= stream;

	int numDatasThisBlock   = marker->m_startBlock->m_numElements - marker->m_startElemIdxInBlock;
	m_numElementsToReadInThisBlock	 = hkMath::min2(numDatasThisBlock, marker->m_numElements);
	m_numElementsToReadInOtherBlocks = marker->m_numElements - m_numElementsToReadInThisBlock;
	if ( m_numElementsToReadInThisBlock == 0)
	{
		m_currentByteLocation = HK_NULL;
	}

	
	
	//HK_ASSERT( 0xf0345467, !m_currentBlock || m_numElementsToReadInThisBlock > 0 );
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::ReadIterator::setToStartOfStream( const hkBlockStreamBase* HK_RESTRICT stream )
{
	const Block* startBlock = stream->begin();
	const int currentByteLocation = 0;
	HK_ASSERT( 0xf0dfde39, startBlock != HK_NULL );

	m_currentBlock			= stream->begin();
	m_currentByteLocation   = ((char*)startBlock->begin()) + currentByteLocation;
	m_blockStream			= stream;

	int numDatasThisBlock   = startBlock->m_numElements;
	m_numElementsToReadInThisBlock	 = numDatasThisBlock;
	m_numElementsToReadInOtherBlocks = HK_INT32_MAX;
	if ( numDatasThisBlock == 0)
	{
		m_currentByteLocation = HK_NULL;
	}
	
	//HK_ASSERT( 0xf0456587, !m_currentBlock || m_numElementsToReadInThisBlock > 0 );
}


template<typename Allocator>
const void* hkBlockStreamBase<Allocator>::ReadIterator::access()
{
	return m_currentByteLocation.val();
}

template<typename Allocator>
const void* hkBlockStreamBase<Allocator>::ReadIterator::advanceAndAccessNext(int thisElemSize)
{
	HK_ASSERT( 0xf034df43, m_currentBlock );
	HK_ASSERT( 0xf0347687, m_numElementsToReadInThisBlock > 0 );

	m_numElementsToReadInThisBlock		= m_numElementsToReadInThisBlock-1;

	if( m_numElementsToReadInThisBlock > 0 )
	{
		const char* nextLocation = hkAddByteOffsetConst(m_currentByteLocation.val(), thisElemSize );
		HK_ASSERT( 0xf06576df, nextLocation <= m_currentBlock->end() );
		m_currentByteLocation = nextLocation;
		return (const void*)nextLocation;
	}
	else
	{
		const void* data = advanceToNewBlock();
		HK_ASSERT( 0xf06576df, m_currentByteLocation.val() + thisElemSize <= m_currentBlock->end() );
		return data;
	}
}

template<typename Allocator>
template<int SIZE>
void hkBlockStreamBase<Allocator>::ReadIterator::prefetch(int offset) const
{
	hkMath::forcePrefetch<SIZE>( m_currentByteLocation + offset );
}

template<typename Allocator>
hkBlockStreamBase<Allocator>::ReadIterator::~ReadIterator()
{
}

//////////////////////////////////////////////////////////////////////////
//
// Modify Iterator
//
//////////////////////////////////////////////////////////////////////////

template<typename Allocator>
void hkBlockStreamBase<Allocator>::ModifyIterator::setToMarker( Allocator* allocator, hkBlockStreamBase* HK_RESTRICT stream, const Marker* HK_RESTRICT marker )
{
	ReadIterator::setToMarker(stream, marker);

	m_tlAllocator			= allocator;
	this->m_numElementsToFreeInThisBlock	 = this->m_numElementsToReadInThisBlock;

	if (HK_NULL == marker->m_startBlock || 0 == marker->m_numElements)
	{
		m_numElementsToFreeInThisBlock = 0;
	}
}

template<typename Allocator>
void hkBlockStreamBase<Allocator>::ModifyIterator::setToStartOfStream( Allocator* HK_RESTRICT allocator, hkBlockStreamBase* HK_RESTRICT stream )
{
// 	HK_ASSERT( 0xf0345dfe, !blockStream->m_isLocked );	// enable this, but it requires a finalize()
// 	HK_ON_DEBUG( blockStream->m_isLocked = true );

	ReadIterator::setToStartOfStream(stream);
	m_tlAllocator = allocator;
	this->m_numElementsToFreeInThisBlock	= this->m_numElementsToReadInThisBlock;
}

template<typename Allocator>
const void* hkBlockStreamBase<Allocator>::ModifyIterator::advanceFreeAndAccessNext(int thisElemSize)
{
	HK_ASSERT( 0xf034df43, this->m_currentBlock );
	HK_ASSERT( 0xf0347687, this->m_numElementsToReadInThisBlock > 0 );

	this->m_numElementsToReadInThisBlock		= this->m_numElementsToReadInThisBlock-1;
	const char* nextLocation = hkAddByteOffsetConst(this->m_currentByteLocation.val(), thisElemSize );	// get this here as this is the most common code path and allows the compiler to optimize better

	if( this->m_numElementsToReadInThisBlock > 0 )
	{
		HK_ASSERT( 0xf06576df, nextLocation <= this->m_currentBlock->end() );
		this->m_currentByteLocation = nextLocation;
		return (const void*)nextLocation;
	}
	else
	{
		const void* data = freeAndAdvanceToNewBlock();
		HK_ASSERT( 0xf06576df, this->m_currentByteLocation.val() + thisElemSize <= this->m_currentBlock->end() );
		return data;
	}
}

//////////////////////////////////////////////////////////////////////////
//
// Marker
//
//////////////////////////////////////////////////////////////////////////

template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::Marker::setStartPoint(const typename hkBlockStreamBase::WriteIterator* HK_RESTRICT it)
{
	this->m_startBlock			= it->m_currentBlock;
	this->m_startByteLocation	= typename Block::CountType(it->m_currentByteLocation);
	this->m_startElemIdxInBlock	= typename Block::CountType(it->m_currentBlockNumElems);
	this->m_numElements			= -(it->m_blockStream->m_numTotalElements + it->m_currentBlockNumElems);
}

template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::Marker::setEndPoint(const typename hkBlockStreamBase::WriteIterator* HK_RESTRICT it)
{
	// the elements in the last block minus the existing elements in the first
	HK_ASSERT( 0xf0345456, it->m_blockStream->m_partiallyFreed == false );

	int currentTotal = it->m_blockStream->m_numTotalElements + it->m_currentBlockNumElems;
	int numElements  = currentTotal + m_numElements /* m_numElements is actually negative and initialized in setStartPoint */;

	HK_ASSERT2( 0xad341112, numElements >= 0, "Range corrupted." );

	if ( numElements )
	{
		if ( (this->m_startBlock != it->m_currentBlock) && this->m_startBlock->m_numElements == this->m_startElemIdxInBlock )
		{
			// first block is empty,
			// advance to next block
			this->m_startBlock = this->m_startBlock->m_next;
			this->m_startElemIdxInBlock = 0;
			this->m_startByteLocation = 0;
			//HK_ASSERT(0x2e937046, m_startBlock );
		}
	}

	this->m_numElements = numElements;
}


template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::Marker::setEmpty()
{
	this->m_startBlock = HK_NULL;
	this->m_numElements = 0;
}


template<typename Allocator>
HK_FORCE_INLINE void hkBlockStreamBase<Allocator>::Marker::setEntireStream(hkBlockStreamBase* HK_RESTRICT stream )
{
	bool isStreamEmpty = stream->isEmpty();
	this->m_startBlock = (!isStreamEmpty)? stream->begin(): HK_NULL;
	this->m_startByteLocation = 0;
	this->m_startElemIdxInBlock = 0;
	this->m_numElements = stream->getTotalNumElems();
}

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
