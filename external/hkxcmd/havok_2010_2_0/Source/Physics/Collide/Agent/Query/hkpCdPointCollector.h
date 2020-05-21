/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CD_POINT_COLLECTOR
#define HK_COLLIDE2_CD_POINT_COLLECTOR

#include <Common/Base/hkBase.h>

struct hkpCdPoint;

/// The base class to collect hkContactPoint and hkpCdBody information from the collision detection
///
/// This class is used for both getClosestPoints() and linearCast() functions.
/// IMPORTANT:
///  - If you are using this class for getClosestPoints(), the resulting distance is an Euclidean distance
///  - If you are using this class for linearCast(), the resulting distance is a fraction between 0 and 1
///  - If you want to reuse an object of this class, you must call reset
class hkpCdPointCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpCdPointCollector);
		
			/// The constructor calls reset(), and sets the m_earlyOutDistance to HK_REAL_MAX
		inline hkpCdPointCollector();

		inline virtual ~hkpCdPointCollector();

			/// This is the function called for every hit of the collision detector.
			/// Note: If this collector is being used for *linear casts* then for optimization
			/// purposes you should set the m_earlyOutDistance to:
			/// - 0.0 if you want to get no more hits
			/// - point.m_contact.getDistance() if you only want to get closer hits
			/// - don't set, if you want to get all hits.
			/// Note that setting this value has *no* optimization effect for e.g., getClosestPoints() calls.
		virtual void addCdPoint( const hkpCdPoint& point ) = 0;

			/// This resets the m_earlyOutDistance to HK_REAL_MAX.
			/// Reset the values if you want to reuse this class.
		virtual inline void reset();

			/// Gets the early out hit distance. This function is used as an optimization by various algorithms to
			/// break out of inner loops.
		inline hkReal getEarlyOutDistance( ) const;

	protected:

		hkReal m_earlyOutDistance;
};

#include <Physics/Collide/Agent/Query/hkpCdPointCollector.inl>

#endif // HK_COLLIDE2_CLOSEST_POINTS_COLLECTOR

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
