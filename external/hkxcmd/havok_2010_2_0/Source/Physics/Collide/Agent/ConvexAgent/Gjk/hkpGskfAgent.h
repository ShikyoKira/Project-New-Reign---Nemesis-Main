/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_GSKF_AGENT_H
#define HK_COLLIDE2_GSKF_AGENT_H

#include <Physics/Internal/Collide/Gjk/GskManifold/hkpGskManifold.h>
#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpGskBaseAgent.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

class hkpCollisionDispatcher;
struct hkpExtendedGskOut;

	/// The hkpGskfAgent is an hkpGskBaseAgent agent which uses a highly optimized
	/// feature based manifold. This manifold works really well for big triangles
	/// and has no parameters to tune
class hkpGskfAgent : public hkpGskBaseAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		static void HK_CALL initAgentFunc(hkpCollisionDispatcher::AgentFuncs& f);

			/// Registers the agent with the collision dispatcher.
		static void HK_CALL registerAgent(hkpCollisionDispatcher* dispatcher);

			// hkpCollisionAgent interface implementation.
		virtual void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface implementation.
		virtual void cleanup( hkCollisionConstraintOwner& constraintOwner );

			// hkpCollisionAgent interface implementation.
		virtual void removePoint( hkContactPointId idToRemove );

			// hkpCollisionAgent interface implementation.
		virtual void commitPotential( hkContactPointId newId );

			// hkpCollisionAgent interface implementation.
		virtual void createZombie( hkContactPointId idTobecomeZombie );

	protected:

		friend class hkpConvexConvexWelderAgent;

			//
			// Note: to use the next two inline functions include the .inl file
			//
			/// Constructor, called by the createGskConvexConvexAgent() function.
		hkpGskfAgent(const hkpCdBody& bodyA, const hkpCdBody& bodyB, hkpContactMgr* mgr);

		/// Destructor, called by cleanup().
		~hkpGskfAgent(){}

			// hkpCollisionAgent interface implementation.
		void processCollisionNoTim(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, int explicitlyAllowNewPoint, hkpProcessCollisionOutput& result);

	public:
		/// Agent creation function used by the hkpCollisionDispatcher.
		static hkpCollisionAgent* HK_CALL createGskfAgent(const 	hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr);

	protected:
	public:
		hkpGskManifold m_manifold;

};


#endif // HK_COLLIDE2_GSKF_AGENT_H

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
