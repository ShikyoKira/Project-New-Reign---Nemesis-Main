/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIRST_CD_BODY_PAIR_COLLECTOR_H
#define HK_FIRST_CD_BODY_PAIR_COLLECTOR_H

#include <Physics/Collide/Agent/Query/hkpCdBodyPairCollector.h>
#include <Physics/Collide/Query/Collector/BodyPairCollector/hkpRootCdBodyPair.h>


	/// hkpFirstCdBodyPairCollector collects the first body pair (e.g., from a hkpCollisionAgent::getPenetrations() call)
	/// It simply stores the pair, and sets the early out flag to true, to prevent any further collision detection being
	/// evaluated. It is useful if you want to simply know whether two objects (where one or both are shape collections)
	/// are overlapping or not, and you want to know the shape keys of the first overlapping object found.
	/// Note: As this class actually can't store the hkpCdBody, it converts each hkpCdBody into
	/// the rood hkpCollidable and the (leave) hkpShapeKey. If you have only one hkpShapeCollection in your shape hierarchy,
	/// this information should be enough to identify your triangle. If you need access to all details, you have to
	/// implement your own hkpCdBodyPairCollector
class hkpFirstCdBodyPairCollector : public hkpCdBodyPairCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpFirstCdBodyPairCollector);

			/// Constructor calls reset().
		inline hkpFirstCdBodyPairCollector();

		inline virtual ~hkpFirstCdBodyPairCollector();
		
			/// This function returns true if this class has collected a hit.
		inline hkBool hasHit( ) const;

			/// This function returns the full hit information
		inline const hkpRootCdBodyPair& getHit() const;

	protected:

		virtual void addCdBodyPair( const hkpCdBody& bodyA, const hkpCdBody& bodyB );
	
	protected:

		hkpRootCdBodyPair	m_cdBodyPair;
};

#include <Physics/Collide/Query/Collector/BodyPairCollector/hkpFirstCdBodyPairCollector.inl>


#endif //HK_FIRST_CD_BODY_PAIR_COLLECTOR_H

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
