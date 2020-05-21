/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SERIALIZED_AGENT_NN_ENTRY_H
#define HK_UTILITIES2_SERIALIZED_AGENT_NN_ENTRY_H

#include <Common/Base/hkBase.h>

#include <Physics/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>
#include <Physics/Dynamics/Collide/hkpSimpleConstraintContactMgr.h>
#include <Physics/Dynamics/World/hkpSimulationIsland.h>

#include <Physics/Collide/Agent3/Machine/Nn/hkpAgentNnMachine.h>
#include <Physics/Collide/Agent3/Machine/1n/hkpAgent1nTrack.h>
#include <Physics/Collide/Agent3/Machine/1n/hkpAgent1nMachine.h>

#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

#include <Physics/ConstraintSolver/Constraint/Contact/hkpContactPointProperties.h>

extern const class hkClass hkpSerializedSubTrack1nInfoClass;

extern const class hkClass hkpSerializedTrack1nInfoClass;

extern const class hkClass hkpSerializedAgentNnEntryClass;


#define HK_SERIALIZED_AGENT_NN_ENTRY_VERSION 1


struct hkpSerializedSubTrack1nInfo;

struct hkpSerializedTrack1nInfo
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkpSerializedTrack1nInfo);

	hkpSerializedTrack1nInfo() {}
	~hkpSerializedTrack1nInfo();

	// sectors for this child track + info on further subtracks of this child track
	hkArray<struct hkpAgent1nSector*> m_sectors;
	hkArray<struct hkpSerializedSubTrack1nInfo*> m_subTracks; // have to use a block, or serialization gives up

	hkBool isEmpty() const { HK_ASSERT2(0xad8753dd, m_subTracks.isEmpty() || m_sectors.getSize(), "Subracks must must be empty when there are no sectors."); return m_sectors.isEmpty(); }

	hkpSerializedTrack1nInfo(hkFinishLoadedObjectFlag f) : m_sectors(f), m_subTracks(f) {}
};

struct hkpSerializedSubTrack1nInfo : public hkpSerializedTrack1nInfo
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkpSerializedSubTrack1nInfo);

	hkpSerializedSubTrack1nInfo() { m_sectorIndex = -1; }

	// location in parent track
	int m_sectorIndex;
	int m_offsetInSector; 

	hkpSerializedSubTrack1nInfo(hkFinishLoadedObjectFlag f) : hkpSerializedTrack1nInfo(f) {}
};

struct hkpSerializedAgentNnEntry : public hkReferencedObject 
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);

	inline hkUint32& endianCheckUint32() { return *reinterpret_cast<hkUint32*>(&m_endianCheckBuffer[0]); }

	hkpSerializedAgentNnEntry() : m_bodyA(HK_NULL), m_bodyB(HK_NULL), m_bodyAId(hkUlong(-1)), m_bodyBId(hkUlong(-1)), m_useEntityIds(false) 
	{ 
		endianCheckUint32() = ENDIAN_CHECK_VALUE; 
		m_version = HK_SERIALIZED_AGENT_NN_ENTRY_VERSION;
	}

	hkpSerializedAgentNnEntry(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_atom(f), m_propertiesStream(f), m_contactPoints(f), m_cpIdMgr(f), m_trackInfo(f) 
	{
		if(f.m_finishing)
		{
			HK_ASSERT2(0xad8755cc, m_version == HK_SERIALIZED_AGENT_NN_ENTRY_VERSION, "You're trying to load an older version of hkpSerializedAgentNnEntry, which may be not compatible with this build.");
		}
	}

	virtual ~hkpSerializedAgentNnEntry();

	enum SerializedAgentType
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

		// Entities linked by the agent entry
	hkpEntity* m_bodyA;
	hkpEntity* m_bodyB;

		// Custom id's identifying the entities linked by the agent entry.
	hkUlong m_bodyAId;
	hkUlong m_bodyBId;

		// Allows you to use custom id's to identify the entities, rather than hkpEntity pointers.
		// If set, you will need to provide an id-to-hkpEntity function when loading contact points.
	hkBool m_useEntityIds;

	hkEnum<SerializedAgentType, hkInt8> m_agentType;

	struct hkpSimpleContactConstraintAtom m_atom;
	hkArray<hkUint8> m_propertiesStream; // We're using a block, because our serialization doesn't support multiple inheritance !!
	hkArray<class hkContactPoint> m_contactPoints;
	hkArray<hkUint8> m_cpIdMgr; 

	hkUint8 m_nnEntryData[160/*largest HK_AGENT3_AGENT_SIZE on all platforms*/]; // used optionally

	struct hkpSerializedTrack1nInfo m_trackInfo;

	hkUint8 m_endianCheckBuffer[4];
	enum { ENDIAN_CHECK_VALUE = 259 };

	hkUint32 m_version;
};

#undef HK_SERIALIZED_AGENT_NN_ENTRY_VERSION


#endif // HK_UTILITIES2_SERIALIZED_AGENT_NN_ENTRY_H

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
