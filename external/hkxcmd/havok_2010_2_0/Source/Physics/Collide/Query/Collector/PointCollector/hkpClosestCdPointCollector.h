/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CLOSEST_CD_POINT_COLLECTOR_H
#define HK_CLOSEST_CD_POINT_COLLECTOR_H

#include <Physics/Collide/Agent/Query/hkpCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpRootCdPoint.h>


/// This class collects only the closest contact point returned in addCdPoint() callbacks
/// Please read the notes for hkpCdPointCollector for information about how these collectors are used
/// Note: As this class cannot store hkpCdBody information, which may be just temporary, it converts the
/// hkpCdBody into a hkpRootCdPoint.
class hkpClosestCdPointCollector : public hkpCdPointCollector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpClosestCdPointCollector);

			/// Constructor calls reset.
		inline hkpClosestCdPointCollector();

			/// Resets the early out condition,
			/// You must call this function if you want to reuse an object of this class.
		inline void reset();

		inline virtual ~hkpClosestCdPointCollector();

			/// Returns true, if this class has collected at least one hit
		inline hkBool hasHit( ) const;

			/// Get the full hit information.
		inline const hkpRootCdPoint& getHit() const;

			/// Returns only the physical hit information: position, normal, distance.
			/// This is identical to getHit().m_contact
		inline const hkContactPoint& getHitContact() const;

		inline const hkVector4& getUnweldedNormal() const { return m_unweldedNormal; }

	protected:

			// this implementation keeps track of the hkpCdPoint with the smallest distance
		virtual void addCdPoint( const hkpCdPoint& pointInfo ) ;
	
	protected:
		hkpRootCdPoint	m_hitPoint;

		hkVector4 m_unweldedNormal;
};

#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.inl>

#endif // HK_CLOSEST_CD_POINT_COLLECTOR_H

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
