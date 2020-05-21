/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SYMMETRIC_AGENT_LINEAR_CAST_H
#define HK_COLLIDE2_SYMMETRIC_AGENT_LINEAR_CAST_H

#include <Physics/Collide/Agent/hkpProcessCollisionOutput.h>
#include <Physics/Collide/Agent/Query/hkpCdPointCollector.h>
#include <Physics/Collide/Agent/Query/hkpCdBodyPairCollector.h>

class hkpCdBody;
struct hkpCollisionInput;
class hkpContactMgr;


	/// A special symmetric agent, which should be used instead of the hkpSymmetricAgent if you derive
	/// the agent from the hkpIterativeLinearCastAgent
template<typename AGENT>
class hkpSymmetricAgentLinearCast : public AGENT
{
	public:

		HK_FORCE_INLINE hkpSymmetricAgentLinearCast( hkpContactMgr* mgr ): AGENT(mgr){}

			/// Called by the appropriate createAgent() function.
		HK_FORCE_INLINE hkpSymmetricAgentLinearCast( const hkpCdBody& A, const hkpCdBody& B, const hkpCollisionInput& input, hkpContactMgr* mgr );

			// hkpCollisionAgent interface implementation.
		virtual void getPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetPenetrations( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpCdBodyPairCollector& collector );

			// hkpCollisionAgent interface implementation.
		virtual void getClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  ) ;


			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticGetClosestPoints( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, class hkpCdPointCollector& collector  ) ;

			// hkpCollisionAgent interface implementation.
		static void HK_CALL staticLinearCast( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpLinearCastCollisionInput& input, hkpCdPointCollector& collector, hkpCdPointCollector* startCollector ); 

			// hkpCollisionAgent interface forward call.
		virtual void processCollision(	const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& result);

			// hkpCollisionAgent interface forward call.
		virtual void updateShapeCollectionFilter( const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkCollisionConstraintOwner& constraintOwner );

};

/// a private class to hkpSymmetricAgent, declared outside, as this class is not templated
class hkpSymmetricAgentFlipCollector: public hkpCdPointCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpCdPointCollector);
		hkpCdPointCollector& m_collector;
		hkpSymmetricAgentFlipCollector( hkpCdPointCollector& c): m_collector(c){}
		virtual void addCdPoint( const hkpCdPoint& point );
};

/// a private class to hkpSymmetricAgent, declared outside, as this class is not templated
class hkpSymmetricAgentFlipCastCollector: public hkpCdPointCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpSymmetricAgentFlipCastCollector);
		hkVector4 m_path;
		hkpCdPointCollector& m_collector;

		hkpSymmetricAgentFlipCastCollector( const hkVector4& path, hkpCdPointCollector& c): m_path(path), m_collector(c){}
		virtual void addCdPoint( const hkpCdPoint& point );
};

/// a private class to hkpSymmetricAgent, declared outside, as this class is not templated
class hkpSymmetricAgentFlipBodyCollector: public hkpCdBodyPairCollector
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpSymmetricAgentFlipBodyCollector);
		hkpCdBodyPairCollector& m_collector;
		hkpSymmetricAgentFlipBodyCollector( hkpCdBodyPairCollector& c): m_collector(c){}
		virtual void addCdBodyPair( const hkpCdBody& bodyA, const hkpCdBody& bodyB);
};


#include <Physics/Collide/Agent/Util/Symmetric/hkpSymmetricAgentLinearCast.inl>

#endif // HK_COLLIDE2_SYMMETRIC_AGENT_LINEAR_CAST_H

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
