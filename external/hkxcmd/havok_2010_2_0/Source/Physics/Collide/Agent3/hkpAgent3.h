/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLISION_AGENT3_H
#define HK_COLLIDE2_COLLISION_AGENT3_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>

struct hkpProcessCollisionInput;
class hkpContactMgr;
struct hkpProcessCollisionOutput;
struct hkpCollisionQualityInfo;
class hkpLinkedCollidable;
class hkpCollisionDispatcher;
#ifndef hkCollisionConstraintOwner
class hkpConstraintOwner;
#	define hkCollisionConstraintOwner hkpConstraintOwner
#endif
struct hkpCollisionInput;
class hkpCdBody;
class hkpShapeKeyTrackWriter;
class hkpShapeKeyTrackConsumer;

// this is a copied version from the hkpProcessCollisionOutput.h file
#	define HK_1N_MACHINE_SUPPORTS_WELDING 


class hkpCollisionAgent;

	/// the base memory stream
struct hkpAgentEntry
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpAgentEntry );

		/// see hkAgent3::StreamCommand
	hkUchar     m_streamCommand;	

		/// the type of the agent (to be used with the hkpCollisionDispatcher)
	hkUchar     m_agentType;		

		/// The number of contact points, if you don't use this, set it to 1
	hkUchar     m_numContactPoints;			

		/// the size of the agent in bytes
	hkUchar     m_size;										

};


typedef void hkpAgentData;

	/// The input structure needed to call an implementation of an agent3
struct hkpAgent3Input
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpAgent3Input );

	hkpAgent3Input() : m_overrideBodyA(HK_NULL) {}

		/// BodyA
	hkPadSpu<const hkpCdBody*> m_bodyA;

		/// BodyB
	hkPadSpu<const hkpCdBody*> m_bodyB;

		/// Original BodyA for the hkpCdBody hierarchy.
		/// This is only used when the hkpCollectionAgent3 calls routines from hkpCollectionCollectionAgent3 and then the NmMachine
	hkPadSpu<const hkpCdBody*> m_overrideBodyA;

		/// Pointer to hkpProcessCollisionInput
	hkPadSpu<const hkpProcessCollisionInput*> m_input;

		/// Pointer to hkpContactMgr
	hkPadSpu<hkpContactMgr*>	m_contactMgr;

		/// a transform converting from b to a space
	hkTransform		m_aTb;
};

struct hkpAgent3ProcessInput : public hkpAgent3Input
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpAgent3ProcessInput );

		/// for GSK convex agents using tim
	hkPadSpu<hkReal>          m_distAtT1;

		/// for agents using tim
	hkVector4       m_linearTimInfo;
};


namespace hkAgent3
{
		/// An id, which can be used with the hkpCollisionDispatcher to get the right function
	typedef int AgentType;

	enum {
		MIN_TOTAL_SIZE = 16,		// 16 = size of end command
		MAX_MEMORY_INCREASE = 80,	// <todo: set back to 48 the maximum amount of memory an agent is allowed to increase every frame
		MAX_SIZE   = 128
	};

	enum StreamCommand
	{
		STREAM_NULL = 0,		 // not defined
		STREAM_END  = 1,		 // end of last sectors
		STREAM_CALL = 2,		 // call the function declared in the dispatcher using the m_agentType
		STREAM_CALL_FLIPPED = 3, // call the function declared in the dispatcher using the m_agentType with flipped input variables
		STREAM_CALL_WITH_TIM = 4, // same but check tims before
		STREAM_CALL_WITH_TIM_FLIPPED = 5,	// same as STREAM_CALL_WITH_TIM but using flipped input variables
		STREAM_CALL_AGENT = 6,	// special mode for compatibility with old style agents.
		
		TRANSFORM_FLAG = 0x08, // when this flag is set in a command, then at least one of the hkpCdBodies in the agent entry is a hkpTransformShape.
		
		STREAM_CALL_WITH_TRANSFORM					= TRANSFORM_FLAG | STREAM_CALL,
		STREAM_CALL_FLIPPED_WITH_TRANSFORM			= TRANSFORM_FLAG | STREAM_CALL_FLIPPED,
		STREAM_CALL_WITH_TIM_WITH_TRANSFORM			= TRANSFORM_FLAG | STREAM_CALL_WITH_TIM,
		STREAM_CALL_WITH_TIM_FLIPPED_WITH_TRANSFORM = TRANSFORM_FLAG | STREAM_CALL_WITH_TIM_FLIPPED,
		STREAM_CALL_AGENT_WITH_TRANSFORM			= TRANSFORM_FLAG | STREAM_CALL_AGENT,
	};

	//
	//	Functions needed to create a full agent
	//

		/// Create an agent and return a pointer just after the agent (but 16byte aligned)
	typedef hkpAgentData* (HK_CALL* CreateFunc)( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* agentData );

		/// Destroy the agent
	typedef void  (HK_CALL* DestroyFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner, hkpCollisionDispatcher* dispatcher );

		/// call to process collision, which should produce a nice manifold
		///
		/// Notes:
		///     - separatingNormalOut is set to HK_NULL if SepNormalFunc was set to HK_NULL
		///     - You should set the entry->m_numContatPoints if possible, else set it to 1
	typedef hkpAgentData* (HK_CALL* ProcessFunc)( const hkpAgent3ProcessInput& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4* separatingNormalOut, hkpProcessCollisionOutput& result);

		/// Called by midphase handling to obtain shape keys.
	typedef hkpAgentData* (HK_CALL* ProcessMidphaseFunc) ( const hkpAgent3ProcessInput& input, hkpAgentData* agentData, hkpShapeKeyTrackWriter& shapeKeyTrackWriter );

		/// Called by post-midphase handling to consumer the shape keys and process the appropriate narrowphase agents.
	typedef hkpAgentData* (HK_CALL* ProcessPostMidphaseFunc) ( const hkpAgent3ProcessInput& input, hkpAgentData* agentData, hkpShapeKeyTrackConsumer& shapeKeyTrackConsumer, hkpProcessCollisionOutput& output );

	enum Symmetric
	{
		IS_SYMMETRIC,
		IS_NOT_SYMMETRIC,
		IS_NOT_SYMMETRIC_AND_FLIPPED
	};

	//
	//	If an agent uses tim technology it needs to implement:
	//
		/// An optional function, which you need to implement to enable tims.
		/// It calculates a valid separating plane (.w component is distance)
	typedef void  (HK_CALL* SepNormalFunc)( const hkpAgent3Input& input, hkpAgentEntry* entry, hkpAgentData* agentData, hkVector4& separatingNormalOut );

		/// An optional function, which you need to implement to enable tims
		/// This function removes all contact points held by the hkAgent3. You only need to implement
		/// this if SepNormalFunc is defined
	typedef hkpAgentData* (HK_CALL* CleanupFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner );



	//
	// If an agent places contact points in the hkpProcessCollisionOutput.m_potentialContacts, 
	// it needs to implement the following functions
	//

		/// Remove one contact point (including potential ones). This function should not call the contact mgr!
	typedef void (HK_CALL* RemovePointFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idToRemove );

		/// Commit a potential contact point (one which does not have an id yet. This function should not call the contact mgr!
	typedef void (HK_CALL* CommitPotentialFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId newId );

		/// Flag a contact point to die at the next process call
		/// This is used when the contact point id is still valid and we need the contact point this very frame
	typedef void (HK_CALL* CreateZombieFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, hkContactPointId idTobecomeZombie );


	//
	//	Other functions
	//
		/// Update child shapes with collision filter
	typedef void (HK_CALL* UpdateFilterFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, const hkpCdBody& bodyA, const hkpCdBody& bodyB, const hkpCollisionInput& input, hkpContactMgr* mgr, hkCollisionConstraintOwner& constraintOwner );

		/// Calculate statistics for all child agents
	typedef void (HK_CALL* CalcStatisticsFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, const hkpCollisionInput& input, hkStatisticsCollector* collector );

		/// Update child shapes with collision filter
	typedef void (HK_CALL* InvalidateTimFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, const hkpCollisionInput& input );

		/// Update child shapes with collision filter
	typedef void (HK_CALL* WarpTimeFunc)( hkpAgentEntry* entry, hkpAgentData* agentData, hkTime oldTime, hkTime newTime, const hkpCollisionInput& input );
}


#endif // HK_COLLIDE2_COLLISION_AGENT3_H

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
