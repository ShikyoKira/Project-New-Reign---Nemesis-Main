/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_hkPaddedAllocator_H
#define HKBASE_hkPaddedAllocator_H

	/// Pads allocations and checks that padding is unmodified on free.
class hkPaddedAllocator : public hkMemoryAllocator
{
	//+hk.MemoryTracker(ignore=True)
public:
			///
		struct Cinfo
		{
				/// Default is 16 byte padding and values are signalling NaNs.
			Cinfo()
				: m_numQuadsPad(1)
				, m_padPattern(0x7ffdadad) // pAD begin. pAD End is +1 i.e. 0x7ffdadae
				, m_bodyPattern(0x7ffa110c)// ALLOC
				, m_freePattern(0x7fffefef)// FrEe
			{
			}

				/// Number of 16 byte pads.
			int m_numQuadsPad;
				/// Fill at ends.
			hkUint32 m_padPattern;
				/// Fill body of new allocation.
			hkUint32 m_bodyPattern;
				/// Scrub freed memory.
			hkUint32 m_freePattern;
		};

			/// Pad allocations with the given size.
		hkPaddedAllocator();

			/// Default fill values are used if not given.
		void init( hkMemoryAllocator* next, Cinfo* cinfo=HK_NULL );
			///
		void quit();

		virtual void* blockAlloc( int numBytes );
		virtual void blockFree( void* p, int numBytes );

		hkBool32 isOk( const void* p, int size ) const;

		virtual void getMemoryStatistics( MemoryStatistics& u );
		virtual int getAllocatedSize(const void* obj, int nbytes);

	protected:

		hkMemoryAllocator* m_next;
		Cinfo m_cinfo;
		int m_allocated;
		int m_inUse;
};

#endif // HKBASE_hkPaddedAllocator_H

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
