/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_PROCESS_COLLISION_INPUT_H
#define HK_COLLIDE2_PROCESS_COLLISION_INPUT_H

#include <Common/Base/Types/Physics/hkStepInfo.h>
#include <Physics/Collide/Agent/hkpCollisionInput.h>
#include <Physics/Collide/Agent3/Machine/1n/hkpAgent1nTrack.h>


struct hkpAgent1nSector;
struct hkpCollisionQualityInfo;

struct hkpAgent1nSector;

	/// This structure is used for all process collision calls queries.
struct hkpProcessCollisionInput : public hkpCollisionInput
{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpProcessCollisionInput);

	public:
		hkpProcessCollisionInput(): m_spareAgentSector(HK_NULL) {}

		inline ~hkpProcessCollisionInput(); 

		hkpProcessCollisionInput& operator=(const hkpProcessCollisionInput& other)
		{
			// copy everything but m_spareAgentSector
			(hkpCollisionInput&)*this = (hkpCollisionInput&)other;
			m_stepInfo = other.m_stepInfo;
			m_collisionQualityInfo = other.m_collisionQualityInfo;
			m_dynamicsInfo = other.m_dynamicsInfo;
			m_enableDeprecatedWelding = other.m_enableDeprecatedWelding;
			m_allowToSkipConfirmedCallbacks = other.m_allowToSkipConfirmedCallbacks;
			m_config = other.m_config;
			return *this;
		}

		hkpProcessCollisionInput( const hkpProcessCollisionInput& other )
		:	hkpCollisionInput(other),
			m_spareAgentSector(HK_NULL)
		{
			*this = other;
		}


	public:
			/// time step information
		hkStepInfo m_stepInfo;

			/// A pointer to the collision quality information. See hkpCollisionDispatcher for more details
		mutable hkPadSpu<hkpCollisionQualityInfo*> m_collisionQualityInfo;

			/// An extra spare sector to optimize memory for the 1n Machine
		mutable hkpAgent1nSector*	m_spareAgentSector; 

			/// a pointer to hkpWorldDynamicsStepInfo if you use the hkDynamics lib, otherwise this can be used as a user pointer
		void*	m_dynamicsInfo;

		hkBool m_enableDeprecatedWelding;

			/// On PlayStation(R)3 contact point confirmed callbacks can be omitted when bodies have zero restitution.
			/// Leave this set to false, to ensure all confirmed callbacks are consistent.
		hkBool m_allowToSkipConfirmedCallbacks;

			/// A pointer to a structure containing internal collision tolerances etc.
		hkpCollisionAgentConfig* m_config;

};

hkpProcessCollisionInput::~hkpProcessCollisionInput()
{ 
	if (m_spareAgentSector) 
	{	
		// Can't do as it causes compile error.. so this replaces it
		//delete m_spareAgentSector; 

		HK_MEMORY_TRACKER_DELETE_OBJECT(m_spareAgentSector)
			hkMemoryRouter::getInstance().heap().blockFree(m_spareAgentSector, hkpAgent1nSector::SECTOR_SIZE);
	}
}

#endif // HK_COLLIDE2_PROCESS_COLLISION_INPUT_H

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
