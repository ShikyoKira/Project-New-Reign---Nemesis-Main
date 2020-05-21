/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIMPLE_MIN_DISTANCE_COLLISION_INFO_COLLECTOR_H
#define HK_SIMPLE_MIN_DISTANCE_COLLISION_INFO_COLLECTOR_H

#include <Physics/Collide/Agent/Query/hkpCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpRootCdPoint.h>


/// This class collects the closest contact point, but does not
/// gather any other information. This is useful if you are interested only in the
/// distance, normal and position and not in hkShapeKeys and rootCollidables.
/// Use this class for the maximum performance.
///
/// \note Read the notes for hkpCdPointCollector.
class hkpSimpleClosestContactCollector : public hkpCdPointCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpSimpleClosestContactCollector);

			/// Constructor calls reset
		inline hkpSimpleClosestContactCollector();

			/// Resets the early out condition.
			/// You must call this function if you want to reuse an object of this class.
		inline void reset();

		inline virtual ~hkpSimpleClosestContactCollector();
		
			/// Returns true, if this class has collected a hit.
		inline hkBool hasHit( ) const;

			/// Returns only the physical hit information: position, normal, distance
		inline const hkContactPoint& getHitContact() const;

	protected:

			// this implementation keeps track of the hkpCdPoint with the smallest distance
		virtual void addCdPoint( const hkpCdPoint& pointInfo ) ;

	protected:
		hkBool m_hasHit;
		hkContactPoint	m_hitPoint;
};

#include <Physics/Collide/Query/Collector/PointCollector/hkpSimpleClosestContactCollector.inl>

#endif // HK_SIMPLE_MIN_DISTANCE_COLLISION_INFO_COLLECTOR_H

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
