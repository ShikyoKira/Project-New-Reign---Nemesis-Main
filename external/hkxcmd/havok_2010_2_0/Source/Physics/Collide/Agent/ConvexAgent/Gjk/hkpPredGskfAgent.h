/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_PREDICTIVE_WRAPPER_AGENT_H
#define HK_COLLIDE2_PREDICTIVE_WRAPPER_AGENT_H

#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpGskfAgent.h>
#include <Physics/Internal/Collide/Gjk/GskManifold/hkpGskManifold.h>

class hkpCollisionDispatcher;

	/// This agent is a hkpGskfAgent which handles continuous collision detection .
class hkpPredGskfAgent : public hkpGskfAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Registers this agent with the collision dispatcher.
		static void HK_CALL registerAgent( hkpCollisionDispatcher* dispatcher );


			// hkpCollisionAgent interface implementation.
		virtual void processCollision( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result );

	public:

			/// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createPredGskfAgent( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr );

	public:
		HK_FORCE_INLINE hkpPredGskfAgent( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr );
};


hkpPredGskfAgent::hkpPredGskfAgent( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& inputIn, hkpContactMgr* mgr )
:	hkpGskfAgent( bodyA, bodyB, mgr )
{
}



#endif // HK_COLLIDE2_PREDICTIVE_WRAPPER_AGENT_H

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
