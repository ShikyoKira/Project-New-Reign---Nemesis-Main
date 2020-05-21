/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_FULL_MANIFOLD_AGENT_H
#define HK_COLLIDE2_FULL_MANIFOLD_AGENT_H

#include <Physics/Collide/Agent/Util/LinearCast/hkpIterativeLinearCastAgent.h>

	/// This agent performs the collision between a set of spheres and a 3 dimensional function
class hkpFullManifoldAgent : public hkpCollisionAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// register the this agent with everything
		static void HK_CALL registerAgent(hkpCollisionDispatcher* dispatcher);

			// hkpCollisionAgent interface implementation.
        virtual inline void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface implementation.
		virtual void getPenetrations(const  hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetPenetrations(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdPointCollector& collector); 
			
			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  );


		virtual void linearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector );

		static void HK_CALL staticLinearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector );


			/// hkAgent interface implementation
		void destroyContactPoints( hkCollisionConstraintOwner& constraintOwner );
		virtual void cleanup( hkCollisionConstraintOwner& constraintOwner );

		

	protected:
			/// Constructor, called by the createXXX functions
		hkpFullManifoldAgent(const hkpCdBody& A,const  hkpCdBody& B,const  hkpCollisionInput& input, hkpContactMgr* mgr);

			/// Destructor
		~hkpFullManifoldAgent(){}

	protected:
			/// create functions, known by the hkpCollisionDispatcher
		static hkpCollisionAgent* HK_CALL createFullManifoldAgent( const hkpCdBody& A, const hkpCdBody& B, const hkpCollisionInput& input, hkpContactMgr* mgr );
	
		struct ContactPoint
		{
			int m_feature;
			hkContactPointId m_contactPointId;
		};

		hkArray<ContactPoint> m_contactPointIds;
};

#endif // HK_COLLIDE2_HEIGHT_FIELD_COLLISION_SPHERES_AGENT_H

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
