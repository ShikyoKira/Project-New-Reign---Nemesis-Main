/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_SIMPLE_COLLISION_RESPONSE_H
#define HK_DYNAMICS2_SIMPLE_COLLISION_RESPONSE_H

class hkContactPoint;
class hkpMotion;
class hkpVelocityAccumulator;
struct hkpSimpleConstraintUtilCollideParams;
class hkpRigidBody;

class hkpSimpleCollisionResponse
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpSimpleCollisionResponse );
		struct SolveSingleOutput
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpSimpleCollisionResponse::SolveSingleOutput );

			hkReal m_impulse;
			hkReal m_velocityKeyframedA;
			hkReal m_velocityKeyframedB;
		};

		/// This collides 2 objects: This function is called by the TOI solver
		static void HK_CALL solveSingleContact( const hkContactPoint& cp, hkTime time, hkpSimpleConstraintUtilCollideParams& params,
												hkpMotion* bodyA, hkpMotion* bodyB,
												class hkpDynamicsContactMgr* contactMgr, SolveSingleOutput& output );

		struct SolveSingleOutput2
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpSimpleCollisionResponse::SolveSingleOutput2 );

			hkReal m_impulse;
		};

		/// This collides 2 object. This function is called by the simple collision response.
		static void HK_CALL solveSingleContact2( class hkpSimpleContactConstraintData* constraintData,
												 const hkContactPoint& cp, hkpSimpleConstraintUtilCollideParams& params,
												 hkpRigidBody* rigidBodyA, hkpRigidBody* rigidBodyB, 
												 hkpVelocityAccumulator* bodyA, hkpVelocityAccumulator* bodyB,
												 SolveSingleOutput2& output );

};


#endif // HK_DYNAMICS2_SIMPLE_COLLISION_RESPONSE_H

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
