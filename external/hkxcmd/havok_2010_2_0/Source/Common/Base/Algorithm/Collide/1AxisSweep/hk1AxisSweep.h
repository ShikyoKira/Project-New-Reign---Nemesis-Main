/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_INTERNAL_1AXIS_SWEEP_H
#define HK_INTERNAL_1AXIS_SWEEP_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

struct hkKeyPair
{
	HK_DECLARE_PLACEMENT_ALLOCATOR();
	hkUint32 m_keyA;
	hkUint32 m_keyB;
};

	/// Utility to quickly find overlapping pairs of AABBs
class hk1AxisSweep
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hk1AxisSweep);

		struct AabbInt : public hkAabbUint32
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hk1AxisSweep::AabbInt );

			hkUint32& getKey()
			{
				// Simply returning m_min[3] here causes a compiler error when static code analysis is enabled as m_min only has 3 components.
				// minP[3] points to m_min's alignment padding, which is where the AabbInt's key is stored.
				hkUint32* minP = m_min;
				return minP[3];
			}
			const hkUint32& getKey() const
			{
				// Simply returning m_min[3] here causes a compiler error when static code analysis is enabled as m_min only has 3 components.
				// minP[3] points to m_min's alignment padding, which is where the AabbInt's key is stored.
				const hkUint32* minP = m_min;
				return minP[3];
			}

			HK_FORCE_INLINE bool operator<(const hk1AxisSweep::AabbInt& aabb1) const	{		return this->m_min[0] < aabb1.m_min[0];		}
			void operator=( const AabbInt& other )		{			hkString::memCpy16<sizeof(AabbInt)>( this, &other );			}

			void setEndMarker()
			{
				m_min[0] = hkUint32(-1);
			}

				/// set from a float AABB. The float values will be converted into ordered ints
			void set( const hkAabb& aabb, int key );

			static HK_FORCE_INLINE hkUint32 yzDisjoint( const hk1AxisSweep::AabbInt& aabb, const hk1AxisSweep::AabbInt& other )
			{
				hkUint32 yab = aabb.m_max[1] - other.m_min[1];
				hkUint32 yba = other.m_max[1] - aabb.m_min[1];
				hkUint32 zab = aabb.m_max[2] - other.m_min[2];
				hkUint32 zba = other.m_max[2] - aabb.m_min[2];
				hkUint32 combined = (yab | yba) | (zab | zba);
				return combined & 0x80000000;
			}
		};

			
		
		/// Iterator for going through the 1AxisSweep results. This is equivalent to using the single-array version of hk1AxisSweep::collide.
		/// Usage is
		/// \code
		///		hk1AxisSweep::IteratorAA iter( aabbs, numAabbs );
		///		for (; iter.isValid(); iter.next() )
		///		{
		///			hkKeyPair pair;
		///			iter.getKeyPair(pair);
		///			// Do something with pair
		///		}
		/// \endcode
		struct IteratorAA
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hk1AxisSweep::IteratorAA );
				inline IteratorAA(const hk1AxisSweep::AabbInt*, int numA);
				
					/// Returns whether or not the iterator has more results
				inline hkBool32 isValid() const HK_RESTRICT;

					/// Advances the iterator to the next result
				inline void next() HK_RESTRICT;

					/// Retrieves the current result
				inline void getKeyPair( hkKeyPair& pair );

			private:
				const hk1AxisSweep::AabbInt* HK_RESTRICT m_aabbs;
				int m_numAabbs;

					/// Index of the "outer" AABB
				int m_current;

					/// Index of the "inner" AABB
				int m_potential;
		};

		/// Iterator for going through the 1AxisSweep results. This is equivalent to using the two-array version of hk1AxisSweep::collide.
		/// Usage is
		/// \code
		///		hk1AxisSweep::IteratorAB iter( aabbsA, numA, aabbsB, numB );
		///		for (; iter.isValid(); iter.next() )
		///		{
		///			hkKeyPair pair;
		///			iter.getKeyPair(pair);
		///			// Do something with pair
		///		}
		/// \endcode
		struct IteratorAB
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hk1AxisSweep::IteratorAB );
				inline IteratorAB(hk1AxisSweep::AabbInt* pa, int numA, hk1AxisSweep::AabbInt* pb, int numB );
				
					/// Returns whether or not the iterator has more results
				inline hkBool32 isValid() const HK_RESTRICT;

					/// Advances the iterator to the next result
				inline void next() HK_RESTRICT;
					
					/// Retrieves the current result
				inline void getKeyPair( hkKeyPair& pair );

			private:

					/// Which part of the "while" loop we're current in.
				hkBool32 m_aIsBigger;
				const hk1AxisSweep::AabbInt* HK_RESTRICT m_currentPtr;
				const hk1AxisSweep::AabbInt* HK_RESTRICT m_potentialPtr;

				const hk1AxisSweep::AabbInt* HK_RESTRICT m_pa;
				const hk1AxisSweep::AabbInt* HK_RESTRICT m_pb;

				int m_numA;
				int m_numB;
		};

			/// This returns all overlapping AABB pairs, where one AABB is from the first list, and the other from the second list.
			/// Both lists must be appended with four AABB elements, where AABB.m_min[0] == HK_REAL_MAX.
			/// numA/numB should be equal to the actual number of elements excluding the padding AABBs.
		static int HK_CALL collide( const AabbInt* pa, int numA, const AabbInt* pb, int numB, hkKeyPair* HK_RESTRICT pairsOut, int maxNumPairs, hkPadSpu<int>& numPairsSkipped);

			/// This returns all overlapping AABB pairs when collide pa with itself
			/// The list must be appended with four AABB elements, where aabb.m_min[0] == HK_REAL_MAX.
			/// numA should be equal to the actual number of elements excluding the padding AABBs.
			/// The number of pairs which didn't fit into the buffer is written into numPairsSkippedInOut.
		static int HK_CALL collide( const AabbInt* pa, int numA, hkKeyPair* HK_RESTRICT pairsOut, int maxNumPairs, hkPadSpu<int>& numPairsSkippedOut);

			/// Utility function to sort AABBs in place. This implementation is really fast, don't try to beat it
		static void HK_CALL sortAabbs(hk1AxisSweep::AabbInt* aabbs, int size);
};

#include <Common/Base/Algorithm/Collide/1AxisSweep/hk1AxisSweep.inl>


#endif // HK_INTERNAL_1AXIS_SWEEP_H

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
