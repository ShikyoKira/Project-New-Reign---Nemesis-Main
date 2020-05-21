/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKBITFIELD_H
#define HKBASE_HKBITFIELD_H

extern const hkClass hkBitFieldClass;

	/// A fixed size array of bits. Use this class sparingly, as bit twiddling
	/// is not efficient on all platforms.
class hkBitField
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ARRAY, hkBitField );
		HK_DECLARE_REFLECTION();

			/// Construct a bit field of size 0.
		HK_FORCE_INLINE hkBitField();

			/// Construct a bit field of size numBits.
		HK_FORCE_INLINE hkBitField( int numBits );

			/// Construct a bit field of size numBits, and set all the bits
			/// to the low bit in the initialValue parameter.
		HK_FORCE_INLINE hkBitField( int numBits, int initialValue );

			/// Non copying initialization from an memory buffer.
			/// Note this does not copy but uses it in place.
			/// Care needs to be taken that the data pointer points to is valid for the scope
			/// of this bitField. Pointer will not be deallocated on destruction.
		HK_FORCE_INLINE hkBitField( hkUint32* ptr, int numBits );

	private:

			/// Private copy constructor.
		hkBitField( const hkBitField& ) { }

	public:

			/// Destructor.
		inline ~hkBitField();

			/// Returns (in the lowest bit position) the bit at the given index.
		HK_FORCE_INLINE int get( int index ) const;

			/// Set the bit at the given index to 1.
		HK_FORCE_INLINE void set( int index );

			/// Set all the bits between start and start+numBits 1
		HK_FORCE_INLINE void setRange( int startIndex, int numBits );

			/// Clear the bit at the given index to 0.
		HK_FORCE_INLINE void clear( int index );

			/// Assign the bit at the given index to the low bit of the value parameter.
		HK_FORCE_INLINE void assign( int index, int value );
	
			/// Set all the bits to the given value.
		HK_FORCE_INLINE void assignAll( int value );

			/// Resize the bit field, new elements initialized with 'value'.
		void setSize( int numBits, int fillValue = 0 );

			/// Get the number of bits the bit field is set to.
		HK_FORCE_INLINE int getSize() const;

			/// Get the word given the word index
		HK_FORCE_INLINE hkUint32 getWord( int wordIndex ) const;

			/// Get number of words in the bitset
		HK_FORCE_INLINE int getNumWords() const;

			/// Count the number of 1 bits
		static int HK_CALL countOnes( const hkUint32* words, int numBits );

			/// Assignment operator.
		HK_FORCE_INLINE void operator = ( const hkBitField& bitField );

			/// Get the words
		HK_FORCE_INLINE hkUint32* getWords();

			/// Get the words, read only access
		HK_FORCE_INLINE const hkUint32* getWords() const;

			/// Gets the number of words required to store the specified number of bits
		static HK_FORCE_INLINE int HK_CALL getNumWordsRequired( int numBits );

			/// Perform bit-wise or with another bit field.
		HK_FORCE_INLINE void orWith(const hkBitField& other);

			/// Perform bit-wise and with another bit field.
		HK_FORCE_INLINE void andWith(const hkBitField& other);

			/// Count of set bits.
		HK_FORCE_INLINE int bitCount() const;

	private:

			// an array of words which hold the bits
		hkArray<hkUint32> m_words;

			// the number of bits the bit field is set to
		int m_numBits;

	private:

			// fill the bits that are beyond m_numBits but within m_numWords
		void fillUnusedBits( int fillValue );

	public:

		inline hkBitField( hkFinishLoadedObjectFlag flag );
};

template< int NBITS >
class hkInplaceBitField : public hkBitField
{
	public:

		enum { NUM_INT32 = (NBITS+7)/8 };

		hkInplaceBitField() : hkBitField(m_storage, NBITS)
		{
			hkString::memSet(&m_storage[0], 0, sizeof(m_storage));
		}

	private:

		hkUint32 m_storage[NUM_INT32];
};


#include <Common/Base/Container/BitField/hkBitField.inl>

#endif // HKBASE_HKBITFIELD_H

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
