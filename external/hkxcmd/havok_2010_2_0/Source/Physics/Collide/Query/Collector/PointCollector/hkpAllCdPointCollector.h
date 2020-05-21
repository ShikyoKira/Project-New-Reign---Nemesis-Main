/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ALL_CD_POINT_COLLECTOR_H
#define HK_ALL_CD_POINT_COLLECTOR_H

#include <Physics/Collide/Agent/Query/hkpCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpRootCdPoint.h>


/// This class collects all contact point information returned in addCdPoint() callbacks
/// Please read the notes for hkpCdPointCollector for information about how these collectors are used
/// This collector never causes an early out from a collision detection query.
/// Note: As this class cannot store hkpCdBody information, which may be just temporary, it converts the
/// hkpCdBody into a hkpRootCdPoint.
class hkpAllCdPointCollector : public hkpCdPointCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpAllCdPointCollector);

			/// Constructor calls reset.
		inline hkpAllCdPointCollector();

			/// Resets the early out condition and empties the array of hits.
			/// You must call this function if you want to reuse an object of this class.
		inline void reset();

		inline virtual ~hkpAllCdPointCollector();
		
			/// Get all the hits
		inline const hkArray<hkpRootCdPoint>& getHits() const;

			/// Get all the hits
		inline hkArray<hkpRootCdPoint>& getHits();

			/// Get the number of hits
		inline int getNumHits() const;

			/// Returns true, if this class has collected at least one hit
		inline hkBool hasHit( ) const;

			/// Sort the hits so that the closest hits are earliest in the hit list
		void sortHits();

	public:

			// implements the hkpCdPointCollector interface.
		virtual void addCdPoint( const hkpCdPoint& event ) ;

	protected:
		hkInplaceArray<hkpRootCdPoint,8> m_hits;
};

#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.inl>


#endif // HK_ALL_CD_POINT_COLLECTOR_H

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
