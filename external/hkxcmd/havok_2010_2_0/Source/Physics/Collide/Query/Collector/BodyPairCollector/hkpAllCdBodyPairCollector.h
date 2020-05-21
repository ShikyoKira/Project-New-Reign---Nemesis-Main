/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ALL_CD_BODY_PAIR_COLLECTOR_H
#define HK_ALL_CD_BODY_PAIR_COLLECTOR_H

#include <Physics/Collide/Agent/Query/hkpCdBodyPairCollector.h>
#include <Physics/Collide/Query/Collector/BodyPairCollector/hkpRootCdBodyPair.h>


	/// hkpAllCdBodyPairCollector collects all body pairs from the hkpCollisionAgent::getPenetrations() call
	/// It simply takes all hkpCdBody pairs and stores them into a list, which you can get from calling getHits().
	/// It never causes an early out.
	/// Note: As this class actually can't store the hkpCdBody, it converts each hkpCdBody into
	/// the root hkpCollidable and the (leave) hkpShapeKey. If you have only one hkpShapeCollection in your shape hierarchy,
	/// this information should be enough to identify your triangle. If you need access to all details, you have to
	/// implement your own hkpCdBodyPairCollector
class hkpAllCdBodyPairCollector : public hkpCdBodyPairCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpAllCdBodyPairCollector);

			/// Constructor calls reset()
		inline hkpAllCdBodyPairCollector();

			/// Resets the early out condition and empties the array of hits.
			/// You must call this function if you want to reuse an object of this class.
		inline virtual void reset();

		inline virtual ~hkpAllCdBodyPairCollector();

			/// Get all the hits
		inline const hkArray<hkpRootCdBodyPair>& getHits() const; 

	protected:

		virtual void addCdBodyPair( const hkpCdBody& bodyA, const hkpCdBody& bodyB );

	protected:
		hkInplaceArray<hkpRootCdBodyPair, 16> m_hits;
};

#include <Physics/Collide/Query/Collector/BodyPairCollector/hkpAllCdBodyPairCollector.inl>


#endif //HK_ALL_CD_BODY_PAIR_COLLECTOR_H

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
