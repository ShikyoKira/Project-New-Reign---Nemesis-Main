/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FLAG_CD_BODY_PAIR_COLLECTOR_H
#define HK_FLAG_CD_BODY_PAIR_COLLECTOR_H

#include <Physics/Collide/Agent/Query/hkpCdBodyPairCollector.h>


/// hkpFlagCdBodyPairCollector collects only a boolean flag, indicating whether there is a hit or not
/// It is useful if you want to simply know whether two objects (where one or both are shape collections)
/// are overlapping or not.
class hkpFlagCdBodyPairCollector : public hkpCdBodyPairCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpFlagCdBodyPairCollector);

			/// Constructor calls reset().
		inline hkpFlagCdBodyPairCollector();

		inline virtual ~hkpFlagCdBodyPairCollector();
		
			/// This function returns true if this class has collected a hit.
		inline hkBool hasHit( ) const;

	protected:
		virtual void addCdBodyPair( const hkpCdBody& bodyA, const hkpCdBody& bodyB );
};

#include <Physics/Collide/Query/Collector/BodyPairCollector/hkpFlagCdBodyPairCollector.inl>

#endif //HK_FLAG_CD_BODY_PAIR_COLLECTOR_H

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
