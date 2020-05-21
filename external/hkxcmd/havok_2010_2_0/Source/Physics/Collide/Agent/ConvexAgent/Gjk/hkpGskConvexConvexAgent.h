/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_GSK_CONVEX_CONVEX_AGENT_H
#define HK_COLLIDE2_GSK_CONVEX_CONVEX_AGENT_H

#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpClosestPointManifold.h>
#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpGskBaseAgent.h>

class hkpCollisionDispatcher;
struct hkpExtendedGskOut;

	/// This is an old GSK agent, which uses the non feature based closest point manifold.
class hkpGskConvexConvexAgent : public hkpGskBaseAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Registers the agent with the collision dispatcher.
		static void HK_CALL registerAgent(hkpCollisionDispatcher* dispatcher);

			// hkpCollisionAgent interface implementation.
		virtual void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface implementation.
		virtual void cleanup( hkCollisionConstraintOwner& constraintOwner );

	protected:

			//
			// Note: to use the next two inline functions include the .inl file
			//
			/// Constructor, called by the createGskConvexConvexAgent() function.
		hkpGskConvexConvexAgent(const hkpCdBody& bodyA, const hkpCdBody& bodyB, hkpContactMgr* mgr);

		/// Destructor, called by cleanup().
		~hkpGskConvexConvexAgent(){}

	public:
		/// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createGskConvexConvexAgent(const 	hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);

	protected:
		int		   m_numContactPoints;
		hkpClosestPointManifold::hkpAgentContactPoint m_contactPoints[hkpClosestPointManifold::HK_NUM_MAX_CONTACTS];

};


#endif // HK_COLLIDE2_GSK_CONVEX_CONVEX_AGENT_H

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
