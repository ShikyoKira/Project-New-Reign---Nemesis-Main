/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template<typename T, int BLOCK_SIZE>
hkBlockList<T, BLOCK_SIZE>::hkBlockList()
{ 
	m_firstBlock = HK_NULL;
	HK_ON_DEBUG( m_numBlocks = 0 );
	HK_ON_DEBUG( m_numTotalElements = 0 );
}

#if defined(HK_DEBUG)
template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::checkEmpty()
{
	HK_ASSERT( 0x341bbc78, m_numTotalElements == 0 );
	HK_ASSERT( 0x341bbc78, m_numBlocks == 0 );
}
#endif

template<typename T, int BLOCK_SIZE>
hkBlockList<T, BLOCK_SIZE>::BatchWriter::BatchWriter( HK_ON_SPU(int dmaGroup) )
{
	this->m_blockList = HK_NULL;

	m_prevBlock = HK_NULL;

}


template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchWriter::setToStartOfList( hkBlockList* blockList )
{
	HK_ASSERT( 0xf0456567, blockList->m_numBlocks == 0 );
	HK_ASSERT( 0xf0456568, blockList->m_numTotalElements == 0 );
	HK_ASSERT( 0xf0456569, blockList->m_firstBlock == HK_NULL );

	// Always have a block at the front.
	Block* newBlock = hkAllocateChunk<Block>(1, HK_MEMORY_CLASS_BASE );
	blockList->m_firstBlock = newBlock;
	HK_ON_DEBUG( blockList->m_numBlocks = 1 );
	Block* currentBlock = newBlock;
	currentBlock->initialize();
	m_blockList = blockList;
	m_currentBlock = newBlock;
	m_currentElement = 0;
}

template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchWriter::finalize()
{
}


template<typename T, int BLOCK_SIZE>
typename hkBlockList<T, BLOCK_SIZE>::Block* hkBlockList<T, BLOCK_SIZE>::BatchWriter::addBlockAtEnd( Block* lastBlock )
{
	HK_ASSERT2( 0x45fea890, lastBlock->m_next == HK_NULL, "lastBlock is not the last block." );
	HK_ON_DEBUG( ++m_blockList->m_numBlocks );
	Block* block = hkAllocateChunk<Block>(1, HK_MEMORY_CLASS_BASE );
	lastBlock->m_next = block;
	return block;
}


template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchWriter::writeBatch( const T* data, int numElements )
{
	HK_ON_DEBUG( m_blockList->m_numTotalElements += numElements );
	Block* currentBlock = this->m_currentBlock;
	while ( numElements > 0 )
	{
		int roomForElementsInBlock = Block::NUM_ELEMENTS_IN_BLOCK - this->m_currentElement;

		if ( !roomForElementsInBlock )
		{
			Block* newBlock = addBlockAtEnd( currentBlock );

			currentBlock = newBlock;
			currentBlock->initialize();

			roomForElementsInBlock = Block::NUM_ELEMENTS_IN_BLOCK;
			this->m_currentElement = 0;
			m_prevBlock = m_currentBlock;
			this->m_currentBlock = newBlock; 
		}
		const int numElementsThisBlock = hkMath::min2<int>( numElements, roomForElementsInBlock );
		
		for ( int i = 0; i < numElementsThisBlock; ++i )
		{
			currentBlock->m_data[m_currentElement] = data[i];
			++m_currentElement;
		}
		data += numElementsThisBlock;
		numElements -= numElementsThisBlock;
	}
	HK_ASSERT2( 0x3f38a9ac, numElements == 0, "Internal error. Too many elements copied." );
}



template<typename T, int BLOCK_SIZE>
hkBlockList<T, BLOCK_SIZE>::BatchConsumer::BatchConsumer( HK_ON_SPU( int dmaGroup ) )
{
	m_currentBlock = HK_NULL;
	m_blockList = HK_NULL;
}

template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchConsumer::setToStartOfList( hkBlockList* HK_RESTRICT list )
{
	m_blockList = list;
	m_currentBlock = list->m_firstBlock;
	m_currentElement = 0;
}


template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchConsumer::setNumElements( int numElements )
{
	HK_ASSERT2( 0x4faa7f89, numElements <= m_blockList->m_numTotalElements, "Trying to read more elements than are in the list." );
	this->m_numElementsToRead = numElements;
}


template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchConsumer::removeBlockFromStart( Block* firstBlock, Block* secondBlock )
{
	HK_ASSERT2( 0x45fea890, m_blockList->m_firstBlock == firstBlock, "firstBlock is not the first block" );
	// first block is not a local pointer on SPU.
	HK_ASSERT2( 0x45fea891, m_blockList->m_firstBlock->m_next == secondBlock, "secondBlock is not the second block." );
	HK_ASSERT2( 0x45fea892, m_blockList->m_numBlocks > 0, "Cannot remove a block from an empty list" );
	HK_ON_DEBUG( --m_blockList->m_numBlocks );
	hkDeallocateChunk<Block>( firstBlock, 1, HK_MEMORY_CLASS_BASE );
	// This means that the consumer will leave the block list in an invalid state in release builds.
	// This doesn't matter because we never need the m_next pointer after running a consumer over the list.
	HK_ON_DEBUG( m_blockList->m_firstBlock = secondBlock );
}


template<typename T, int BLOCK_SIZE>
int hkBlockList<T, BLOCK_SIZE>::BatchConsumer::accessBatch( T*& dataOut )
{
	HK_ASSERT( 0xc67c2ab8, m_currentElement <= Block::NUM_ELEMENTS_IN_BLOCK );
	if ( m_numElementsToRead && ( m_currentElement == Block::NUM_ELEMENTS_IN_BLOCK ) )
	{
		// We've read all of this block.
		Block* finishedBlock = m_currentBlock;
		Block* nextBlock = finishedBlock->m_next;
		removeBlockFromStart( this->m_currentBlock, nextBlock );
		this->m_currentBlock = nextBlock;

		this->m_currentElement = 0;
	}

	const int numElementsLeftInBlock = Block::NUM_ELEMENTS_IN_BLOCK - m_currentElement;
	int numElementsInBatch = hkMath::min2<int>( this->m_numElementsToRead, numElementsLeftInBlock );
	Block* currentBlock = m_currentBlock;
	dataOut = &currentBlock->m_data[m_currentElement];

	this->m_numElementsToRead -= numElementsInBatch;
	this->m_currentElement += numElementsInBatch;
	HK_ON_DEBUG( m_blockList->m_numTotalElements -= numElementsInBatch );
	HK_ASSERT2( 0x2314faef, m_blockList->m_numTotalElements >= 0, "The batch reader has read too many elements" );

	return numElementsInBatch;
}

template<typename T, int BLOCK_SIZE>
int hkBlockList<T, BLOCK_SIZE>::BatchConsumer::getElements( T*& dataOut )
{
	HK_ASSERT( 0xc67c2ab8, m_currentElement <= Block::NUM_ELEMENTS_IN_BLOCK );
	if ( ( m_currentElement == Block::NUM_ELEMENTS_IN_BLOCK ) )
	{
		// we need to wait for the next block, but we don't do that, just return 0
		dataOut = HK_NULL;
		return 0;
	}

	const int numElementsLeftInBlock = Block::NUM_ELEMENTS_IN_BLOCK - m_currentElement;
	Block* currentBlock = m_currentBlock;
	dataOut = &currentBlock->m_data[m_currentElement];

	return numElementsLeftInBlock;
}

template<typename T, int BLOCK_SIZE>
void hkBlockList<T, BLOCK_SIZE>::BatchConsumer::finalize()
{
	if ( this->m_currentBlock )
	{
		HK_ASSERT2( 0x0782a2b4, this->m_currentBlock->m_next == HK_NULL, "You stopped consuming before the last block." );
		removeBlockFromStart( this->m_currentBlock, HK_NULL );
		m_currentBlock = HK_NULL;
	}
	HK_ASSERT2( 0x0782a2b5, ( this->m_blockList == HK_NULL ) || ( (this->m_blockList->m_numTotalElements == 0) && ( this->m_blockList->m_numBlocks == 0) ), "You stopped consuming before the last block." );
	HK_ON_DEBUG( m_blockList = HK_NULL );
}


template<typename T, int BLOCK_SIZE>
hkBlockList<T, BLOCK_SIZE>::BatchConsumer::~BatchConsumer()
{
	finalize();
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
