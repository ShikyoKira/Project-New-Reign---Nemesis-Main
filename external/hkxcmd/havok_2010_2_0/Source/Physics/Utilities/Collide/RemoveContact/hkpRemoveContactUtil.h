/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_REMOVE_CONTACT_UTIL__H
#define HK_REMOVE_CONTACT_UTIL__H

#include <Common/Base/hkBase.h>
#include <Physics/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>
#include <Physics/Utilities/Dynamics/SaveContactPoints/hkpSerializedAgentNnEntry.h>

class hkpWorld;

/// A utility that removes contact point IDs from collision agents and the associated contact manager
/// 
/// Removing contacts from only the contact manager is insufficient since agents still maintain
/// contact point IDs. It is necessary to traverse the hierarchy (in the case of shape collections)
/// and call the removePoint func on all agents until one containing the given contact point ID is found.
///
/// This is potentially costly, since it requires iterating over many agents.
class hkpRemoveContactUtil
{
	public:
			/// Removes a contact point from the given hkpAgentNnEntry
		static void HK_CALL removeContactPoint( hkpAgentNnEntry* entry, const hkpWorld* world, hkContactPointId idToRemove );

			/// Removes a contact point from the given hkpAgentNnEntry
		static void HK_CALL removeContactPoint( hkpAgentNnEntry* entry, const hkpProcessCollisionInput* collisionInput, hkContactPointId idToRemove );

			/// Removes a contact point from the given hkpAgentNnEntry
		static hkResult HK_CALL removeCollisionEntry( hkpAgentNnEntry* entry, const hkpProcessCollisionInput* collisionInput, hkContactPointId idToRemove );

			/// Recursively searches agent hierarchy for idToRemove, then calls agent's removePoint function
		static hkResult HK_CALL removeEntry( hkpAgentEntry* entry, hkBool isNnEntry, const hkpProcessCollisionInput* input, const hkContactPointId& idToRemove, int& sizeOfThisEntryOut );

			/// Removes all contact points where at least one entry in the list is involved in the collision
		static void HK_CALL removeContactPoints( const hkpEntity** entities, int numEntities );

	private:

			/// Agent type
		enum AgentType
		{
			INVALID_AGENT_TYPE,
			BOX_BOX_AGENT3,
			CAPSULE_TRIANGLE_AGENT3,
			PRED_GSK_AGENT3,
			PRED_GSK_CYLINDER_AGENT3,
			CONVEX_LIST_AGENT3,
			LIST_AGENT3,
			BV_TREE_AGENT3,
			COLLECTION_COLLECTION_AGENT3,
			COLLECTION_AGENT3
		};

			/// If the agent is a shape collection, process sub tracks
		static hkResult HK_CALL processSubTracks( const hkArray<hkpAgent1nSector*>& sectorsIn, const hkpProcessCollisionInput* input, const hkContactPointId& idToRemove );

			/// Look up agent type by comparing process functions
		static AgentType HK_CALL getAgentType(hkAgent3::ProcessFunc func);
};

#endif // HK_REMOVE_CONTACT_UTIL

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
