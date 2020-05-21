/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE int hkBitField::getNumWords() const
{
	return m_words.getSize();
}

HK_FORCE_INLINE hkUint32 hkBitField::getWord( int wordIndex ) const
{
	HK_ASSERT( 0x38a87b3b, wordIndex >= 0 && wordIndex < getNumWords() );

	return m_words[wordIndex];
}

HK_FORCE_INLINE hkUint32* hkBitField::getWords()
{
	return m_words.begin();
}

HK_FORCE_INLINE const hkUint32* hkBitField::getWords() const
{
	return m_words.begin();
}

HK_FORCE_INLINE void hkBitField::assignAll( int value )
{
	HK_ASSERT( 0xa59289bb, value >= 0 && value <= 1 );

	hkUint32 fill = value ? 0xffffffff : 0;

	for( int i = 0; i < getNumWords(); i++ )
	{
		m_words[i] = fill;
	}
}

HK_FORCE_INLINE hkBitField::hkBitField()
:	m_numBits(0)
{
}

HK_FORCE_INLINE hkBitField::hkBitField( int numBits )
:	m_words( ( numBits + 31 ) >> 5 ),
	m_numBits(numBits)
{
}

HK_FORCE_INLINE hkBitField::hkBitField( int numBits, int initialValue )
:	m_words( ( numBits + 31 ) >> 5 ),
	m_numBits(numBits)
{
	HK_ASSERT( 0xa63ab345, initialValue >= 0 && initialValue <= 1 );
	assignAll( initialValue );
}

HK_FORCE_INLINE hkBitField::hkBitField( hkUint32* ptr, int numBits )
:   m_words( ptr, ( numBits + 31 ) >> 5, ( numBits + 31 ) >> 5 ),
	m_numBits(numBits)
{
}

inline hkBitField::~hkBitField()
{
}

HK_FORCE_INLINE int hkBitField::getSize() const
{
	return m_numBits;
}

HK_FORCE_INLINE int hkBitField::get( int index ) const
{
	HK_ASSERT( 0x48d17bd3, index >= 0 && index < getSize() );

	int arrayIndex = index >> 5;
	return ( ( m_words[arrayIndex] >> ( index & 0x1f ) ) & 1 );
}

HK_FORCE_INLINE void hkBitField::set( int index )
{
	HK_ASSERT( 0x48a97bc3, index >= 0 && index < getSize() );

	int arrayIndex = index >> 5;
	m_words[arrayIndex] |= ( 1 << ( index & 0x1f ) );
}

HK_FORCE_INLINE void hkBitField::setRange( int startIndex, int numBits )
{
	HK_ASSERT( 0x48a97bce, startIndex >= 0 && startIndex < getSize() );
	int endIndex = startIndex + numBits;
	HK_ASSERT( 0x48a97bcf, endIndex >= 0 && endIndex < getSize() );
	hkUint32 firstWord =    0xffffffff << (startIndex & 0x1f);
	hkUint32 lastWord  = ~ (0xffffffff << (endIndex   & 0x1f) );
	int arrayStart = startIndex >> 5;
	int arrayEnd   = endIndex >> 5;

	if ( arrayEnd > arrayStart )
	{
		m_words[arrayStart] |= firstWord;
		if( lastWord )	// we need this check, or we would fall off the array at the end
		{
			m_words[arrayEnd] |= lastWord;
		}
	}
	else
	{
		m_words[arrayStart] |= firstWord & lastWord;
	}

	for (int i = arrayStart+1; i < arrayEnd; i++ )
	{
		m_words[i] = hkUint32(-1);
	}
}


HK_FORCE_INLINE void hkBitField::clear( int index )
{
	HK_ASSERT( 0x38a87bb3, index >= 0 && index < getSize() );

	int arrayIndex = index >> 5;
	m_words[arrayIndex] &= ~( 1 << ( index & 0x1f ) );
}

HK_FORCE_INLINE void hkBitField::assign( int index, int value )
{
	HK_ASSERT( 0x48a27b13, index >= 0 && index < getSize() );
	HK_ASSERT( 0xe68bb345, value >= 0 && value <= 1 );

	int arrayIndex = index >> 5;
	hkUint32 mask = 1 << (index & 0x1f);

	m_words[arrayIndex] = ( m_words[arrayIndex] & ~mask ) | ( mask & ~( value - 1 ) );
}

HK_FORCE_INLINE void hkBitField::operator = ( const hkBitField& bitField )
{
	m_words = bitField.m_words;
	m_numBits = bitField.m_numBits;
}

HK_FORCE_INLINE hkBitField::hkBitField( hkFinishLoadedObjectFlag flag ) : m_words(flag)
{
}

HK_FORCE_INLINE int HK_CALL hkBitField::getNumWordsRequired( int numBits )
{
	return ( numBits+31 ) >> 5;
}

HK_FORCE_INLINE void hkBitField::orWith(const hkBitField& other)
{
	HK_ASSERT(0x1cb1342b, other.m_numBits == m_numBits);
	for (int j = 0; j < m_words.getSize(); j++)
	{
		m_words[j] |= other.m_words[j];
	}
}

HK_FORCE_INLINE void hkBitField::andWith(const hkBitField& other)
{
	HK_ASSERT(0x6707e555, other.m_numBits == m_numBits);
	for (int j = 0; j < m_words.getSize(); j++)
	{
		m_words[j] &= other.m_words[j];
	}
}

HK_FORCE_INLINE int hkBitField::bitCount() const
{
	return countOnes( m_words.begin(), m_numBits );
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
