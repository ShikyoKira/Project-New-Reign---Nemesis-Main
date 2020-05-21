/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_AGENT3_COLLECTION_MACHINE_H
#define HK_COLLIDE2_AGENT3_COLLECTION_MACHINE_H

#include <Physics/Collide/Agent3/hkpAgent3.h>
#include <Physics/Collide/Dispatch/Agent3Bridge/hkpAgent3Bridge.h>
#include <Physics/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>

class hkpLinkedCollidable;
class hkpShapeKeyTrackWriter;
class hkpShapeKeyTrackConsumer;


struct hkpAgentNnMachinePaddedEntry: hkpAgentNnEntry
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpAgentNnMachinePaddedEntry );

#if HK_POINTER_SIZE == 4
	hkUint32 m_padOutTo16BytesAlignment[2];
#else // HK_POINTER_SIZE == 8
	hkUint32 m_padOutTo16BytesAlignment[2];
#endif
};

struct hkpAgentNnMachineTimEntry: hkpAgentNnEntry
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpAgentNnMachineTimEntry );

	hkTime     m_timeOfSeparatingNormal;		// only used if tims are enabled
	hkVector4  m_separatingNormal;
};
#	define HK_FOR_ALL_AGENT_ENTRIES_BEGIN( TRACK, ENTRY )											\
{																									\
	for (int HKLOOP_sectorIndex = 0; HKLOOP_sectorIndex < TRACK.m_sectors.getSize(); )				\
    {																								\
	    hkpAgentNnSector* HKLOOP_currentSector = TRACK.m_sectors[HKLOOP_sectorIndex];				\
	    hkpAgentNnEntry* ENTRY = HKLOOP_currentSector->getBegin();									\
	    HKLOOP_sectorIndex++;																		\
	    hkpAgentNnEntry* HKLOOP_endEntry =  (HKLOOP_sectorIndex == TRACK.m_sectors.getSize()) ?		\
		    hkAddByteOffset(ENTRY, TRACK.m_bytesUsedInLastSector) : HKLOOP_currentSector->getEnd();	\
	    for( ; ENTRY < HKLOOP_endEntry; ENTRY = hkAddByteOffset( ENTRY, ENTRY->m_size ) )			\
    	{

#define HK_FOR_ALL_AGENT_ENTRIES_END } } }


extern "C"
{
	void HK_CALL hkAgentNnMachine_DestroyTrack( hkpAgentNnTrack& track, hkpCollisionDispatcher* dispatch );

	void HK_CALL hkAgentNnMachine_GetAgentType( const hkpCdBody* cdBodyA, const hkpCdBody* cdBodyB, const hkpProcessCollisionInput& input, int& agentTypeOut, int& isFlippedOut );

	hkpAgentNnEntry* HK_CALL hkAgentNnMachine_CreateAgent( hkpAgentNnTrack& track, hkpLinkedCollidable* collA, const hkpCdBody* firstNonTransformBodyA, hkpLinkedCollidable* collB, const hkpCdBody* firstNonTransformBodyB, hkUchar cdBodyHasTransformFlag, int agentType, const hkpProcessCollisionInput& input, hkpContactMgr* mgr );

	void HK_CALL hkAgentNnMachine_DestroyAgent( hkpAgentNnTrack& track, hkpAgentNnEntry* entry, hkpCollisionDispatcher* dispatch, hkCollisionConstraintOwner& constraintOwner );


	void HK_CALL hkAgentNnMachine_AppendTrack( hkpAgentNnTrack& track, hkpAgentNnTrack& appendee);


	void HK_CALL hkAgentNnMachine_UpdateShapeCollectionFilter( hkpAgentNnEntry* entry, const hkpCollisionInput& input, hkCollisionConstraintOwner& constraintOwner );

	//
	// processing
	//

	void HK_CALL hkAgentNnMachine_ProcessAgent( hkpAgentNnEntry* entry,	const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkpContactMgr* mgr );
	void HK_CALL hkAgentNnMachine_ProcessAgent_midphase( hkpAgentNnEntry* entry, const hkpProcessCollisionInput& input, hkpShapeKeyTrackWriter& shapeKeyTrackWriter );
	void HK_CALL hkAgentNnMachine_ProcessAgent_postMidphase( hkpAgentNnEntry* entry,	const hkpProcessCollisionInput& input, hkpShapeKeyTrackConsumer& shapeKeyTrackConsumer, hkpProcessCollisionOutput& output, hkpContactMgr* mgr );


		// process a track, stops if out of memory
	void HK_CALL hkAgentNnMachine_ProcessTrack( class hkpConstraintOwner* owner, hkpAgentNnTrack& track,	const hkpProcessCollisionInput& input );

	bool HK_CALL hkAgentNnMachine_IsEntryOnTrack(hkpAgentNnTrack& track, hkpAgentNnEntry* entry);

	void HK_CALL hkAgentNnMachine_InternalDeallocateEntry(hkpAgentNnTrack& track, hkpAgentNnEntry* entry);

	// ALSO FOR EXTERNAL USE

		/// Makes a copy of the entry in the destTrack and redirects all pointers to entry to the new copy.
		/// Does not touch the entry. In the end, no pointers will point to the original entry
	hkpAgentNnEntry* HK_CALL hkAgentNnMachine_CopyAndRelinkAgentEntry( hkpAgentNnTrack& destTrack, hkpAgentNnEntry* entry );

	hkpAgentNnEntry* HK_CALL hkAgentNnMachine_FindAgent( const hkpLinkedCollidable* collA, const hkpLinkedCollidable* collB );

		// activation/deactiation/setPositionOnEntity
	void HK_CALL hkAgentNnMachine_InvalidateTimInAgent( hkpAgentNnEntry* entry, const hkpCollisionInput& input );

	void HK_CALL hkAgentNnMachine_WarpTimeInAgent( hkpAgentNnEntry* entry, hkTime oldTime, hkTime newTime, const hkpCollisionInput& input );


	//
	// Debugging
	//

	void HK_CALL hkAgentNnMachine_AssertTrackValidity( hkpAgentNnTrack& track );

	//
	// UNDONE
	//

	void HK_CALL hkAgentNnMachine_TouchAgent( hkpAgentEntry* entry,	const hkpProcessCollisionInput& input );

	const hkpCdBody* HK_CALL hkAgentMachine_processTransformedShapes(const hkpCdBody* cdBody, hkpCdBody* newCdBodies, class hkMotionState* newMotionStates, int numSlots, hkPadSpu<hkUchar>& cdBodyHasTransformFlag);

}





#endif // HK_COLLIDE2_AGENT3_COLLECTION_MACHINE_H

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
