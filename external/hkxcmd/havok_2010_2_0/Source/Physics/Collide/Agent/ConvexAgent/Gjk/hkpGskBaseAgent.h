/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_GSK_BASE_AGENT_H
#define HK_COLLIDE2_GSK_BASE_AGENT_H

#include <Physics/Collide/Agent/Util/LinearCast/hkpIterativeLinearCastAgent.h>
#include <Physics/Internal/Collide/Gjk/hkpGskCache.h>

class hkpCollisionDispatcher;
struct hkpExtendedGskOut;
class hkpGsk;

	/// A GSK agent (all classes deriving from this class) handles collisions between pairs of hkConvexShapes using the GSK algorithm.
	/// For all shape pairs, where no direct implementation is available, the collision detection falls back
	/// to this hkpGskBaseAgent agent.
	/// As this agent has no clue about the real surface of the shape, it only can try to sample
	/// (hkpConvexShape::getSupportingVertex()) the shapes involved.
	/// This is quite a slow process (bad), however this agent can use frame coherency very very effectively.
	/// As a result we get a number of properties:
	///   - The first or uncached call to this agent is quite expensive (2-5 times the cached non first call)
	///   - If caching is used and the objects don't penetrate, than the performance is good,
	///     typically only 1.5-3 times slower than a specialized agent (e.g., box-box or sphere-sphere)
	///   - If the objects penetrate, than a slower algorithm gets started (about 2-5 times slower than the non penetrating one)
	///   - To do caching effectively, quite some memory is needed (the size of this agent is roughly 64 Bytes)
	///   - If you use caching and the distance between the two objects is big, than the algorithm has an
	///     early out (called tim) which is really fast (as fast as a sphere-sphere check).
	///
	/// In real game scenarios the following is
	///   - If two convex objects collide, this agent behaves pretty well. As the number of convex-convex object
	///     pairs in a scene are often limited, memory is no big issue.
	///   - if a convex object hits a triangle landscape, quite some triangle can become potential collision partners.
	///     E.g. if a ragdoll with 10 bones falls onto a flowerpot with 100 individual triangles, 10 * 100 agents are
	///     created, resulting in ~80 kBytes of memory consumption. If you are low on memory, you have to be very
	///     careful when designing your collision geometry
	///
	/// The hkpGskBaseAgent serves as a base for all different variants of GSK agents and
	/// implements all functions but the process collision call
class hkpGskBaseAgent : public hkpIterativeLinearCastAgent
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			// hkpCollisionAgent interface implementation. this is not implemented and asserts
		virtual void processCollision(const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface implementation.
		virtual void getPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  );

			// hkpCollisionAgent interface implementation.
		virtual void cleanup( hkCollisionConstraintOwner& constraintOwner );

	public:

			//
			// Note: to use the next two inline functions include the .inl file
			//
			/// Constructor, called by the createGskConvexConvexAgent() function.
		hkpGskBaseAgent(const hkpCdBody& bodyA, const hkpCdBody& bodyB, hkpContactMgr* mgr);

	private:
		friend class hkpConvexListAgent;
			/// Constructor, called by the createGskConvexConvexAgent() function.
		hkpGskBaseAgent(hkpContactMgr* mgr): hkpIterativeLinearCastAgent(mgr){}

	protected:
		HK_FORCE_INLINE hkBool _getClosestPoint( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpExtendedGskOut& output);

		static void HK_CALL calcSeparatingNormal( const hkpCdBody& bodyA, const hkpCdBody& bodyB, hkReal earlyOutTolerance, hkpGsk& gsk, hkVector4& separatingNormalOut );

			/// get the closest point
		hkBool getClosestPoint( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpExtendedGskOut& output );

		static HK_FORCE_INLINE hkBool staticGetClosestPoint(  const hkpCdBody& bodyA,	const hkpCdBody& bodyB, const hkTransform& aTb, const hkpCollisionInput& input, hkpGskCache& cache, struct hkpExtendedGskOut& output);

			// hkpCollisionAgent interface implementation.
		virtual void invalidateTim( const hkpCollisionInput& input);

			// hkpCollisionAgent interface implementation.
		virtual void warpTime(hkTime oldTime, hkTime newTime, const hkpCollisionInput& input);

		/// Destructor, called by cleanup().
		~hkpGskBaseAgent(){}

	protected:

		hkpGskCache  m_cache;
		hkTime      m_timeOfSeparatingNormal;
		hkReal	    m_allowedPenetration;	// only used by the hkpPredGskfAgent
		hkVector4	m_separatingNormal;
		
			// not used in this class, but otherwise data would be padded and lost
};


#endif // HK_COLLIDE2_GSK_BASE_AGENT_H

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
