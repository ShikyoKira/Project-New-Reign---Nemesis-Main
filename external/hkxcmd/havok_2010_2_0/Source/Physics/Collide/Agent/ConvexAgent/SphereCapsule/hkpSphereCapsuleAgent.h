/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SPHERE_CAPSULE_AGENT_H
#define HK_COLLIDE2_SPHERE_CAPSULE_AGENT_H

#include <Physics/Collide/Agent/Util/LinearCast/hkpIterativeLinearCastAgent.h>

class hkpCollisionDispatcher;

	/// This agent handles collisions between hkpSphereShape and hkpCapsuleShape
class hkpSphereCapsuleAgent : public hkpIterativeLinearCastAgent
{
    public:
    HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			///Registers this agent with the collision dispatcher.
		static void HK_CALL registerAgent(hkpCollisionDispatcher* dispatcher);

			// hkpCollisionAgent interface implementation.
        virtual inline void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface implementation.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdPointCollector& pointDetails); 
			
			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  );

			// hkpCollisionAgent interface implementation.
		virtual void getPenetrations(const  hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetPenetrations(const  hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		virtual void cleanup(hkCollisionConstraintOwner& info);

	protected:

		/// Constructor, called by the agent creation functions.
		HK_FORCE_INLINE hkpSphereCapsuleAgent( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr );

        /// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createSphereCapsuleAgent(const 	hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);
        
		/// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createCapsuleSphereAgent(	const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);


	public:

		enum ClosestPointResult
		{
			ST_CP_MISS,
			ST_CP_HIT,
		};

	private:
		static HK_FORCE_INLINE ClosestPointResult getClosestPointInl( const  hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkContactPoint& cpoint);

	protected:
		static ClosestPointResult HK_CALL getClosestPoint( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkContactPoint& cpoint);


		hkContactPointId m_contactPointId;
};

#endif // HK_COLLIDE2_SPHERE_CAPSULE_AGENT_H

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
