/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BLOCKSTREAM_BLOCK_H
#define HK_BLOCKSTREAM_BLOCK_H

template<int BLOCK_SIZE> struct hkBlockStreamBlock;

///	Block is aligned to BLOCK_ALIGNMENT byte
template<int BLOCK_SIZE>
struct hkBlockStreamBlockHeader
{
	typedef hkUint16 CountType;

	HK_ALIGN16( hkUint32 m_numAllocatedElems );	///< This is the total number of elements which are not consumed yet
	CountType	m_numElements;				///< this is the number of items in the m_data field, not the number of bytes
	CountType	m_bytesUsed;				///< The number of bytes used in this block
	int			m_blockIndexInStream;		///< The index of this block in a stream
	void*		m_allocator;				///< for debugging
	void*		m_debugBlockStream;			///< for debugging
	hkBlockStreamBlock<BLOCK_SIZE>* m_next;
};

template<int BLOCK_SIZE>
struct hkBlockStreamBlock : hkBlockStreamBlockHeader<BLOCK_SIZE>
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, hkBlockStreamBlock );

	enum
	{
		BLOCK_DATA_SIZE = BLOCK_SIZE - hkSizeOf( hkBlockStreamBlockHeader<BLOCK_SIZE> )
	};

	HK_FORCE_INLINE void initialize() 
	{ 
		this->m_numAllocatedElems = 0; 
		this->m_numElements = 0; 
		this->m_bytesUsed = 0; 
		this->m_next = HK_NULL; 
		HK_ON_DEBUG( hkString::memSet(m_data, 0xcf, BLOCK_DATA_SIZE) );
		this->m_blockIndexInStream = 0;
	}
	hkBlockStreamBlock(){ initialize(); }
	void* begin() { return m_data; }
	const void* begin() const { return m_data; }
	void* end()   { return this+1; }
	const void* end() const { return this+1; }

	HK_ALIGN16( hkUchar m_data[BLOCK_DATA_SIZE] );
};

#endif // HK_BLOCKSTREAM_BLOCK_H

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
