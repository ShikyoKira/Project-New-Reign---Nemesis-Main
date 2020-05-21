/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_PROCESS_COLLISION_OUTPUT_H
#define HK_COLLIDE2_PROCESS_COLLISION_OUTPUT_H

#include <Physics/Collide/Agent/hkpProcessCollisionData.h>
class hkpContactMgr;

	// this is a copied version from the hkAgent3.h file
#	define HK_1N_MACHINE_SUPPORTS_WELDING 

	/// A structure used by the hkCollisionAgents to output their result
struct hkpProcessCollisionOutput: public hkpProcessCollisionData
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpProcessCollisionOutput);
		
		inline hkpProcessCollisionOutput(hkCollisionConstraintOwner* owner): hkpProcessCollisionData(owner){}

			/// before calling a collision agent we have to reset all values
		inline void reset()
		{
			m_firstFreeContactPoint = &m_contactPoints[0];
			m_toi.m_time = HK_REAL_MAX;
#if defined(HK_1N_MACHINE_SUPPORTS_WELDING)
			m_potentialContacts = HK_NULL;
#endif
		}

			/// Reserve N contact points in this structure
		inline hkpProcessCdPoint* reserveContactPoints( int numberOfPoints=1 )
		{
			return m_firstFreeContactPoint;
		}
		
			/// If we want to make N reserved contact points permanent, we have to call
			/// commitContactPoints(N)
		inline void commitContactPoints( int numberOfPoints=1 )
		{
			m_firstFreeContactPoint= m_firstFreeContactPoint + numberOfPoints;
			HK_ASSERT2( 0xf0100101, m_firstFreeContactPoint < &m_contactPoints[HK_MAX_CONTACT_POINT], "ContactPoint Overflow in hkpProcessCollisionOutput");
		}

			/// Discards N previously committed points.
		inline void uncommitContactPoints( int numberOfPoints=1 )
		{
			m_firstFreeContactPoint= m_firstFreeContactPoint - numberOfPoints;
			HK_ASSERT2( 0xad100102, m_firstFreeContactPoint >= &m_contactPoints[0], "ContactPoint Underflow in hkpProcessCollisionOutput");
		}
	
			/// If we want to discard N reserved contact points we have to call abortContactPoints(N)
		inline void abortContactPoints( int numberOfPoints=1 )
		{
		}


		//
		//	For hkAgent3 welding technology
		//
	public:
		struct ContactRef 
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpProcessCollisionOutput::ContactRef );

			hkpProcessCdPoint*	m_contactPoint;
			hkpAgentEntry*		m_agentEntry;
			void*			    m_agentData; // see hkAgent3.h
		};


		/// Internal welding data:
		/// 
		/// All agents have to report all existing and new point in the m_contactPoints array.
		/// If a contact point is new, an entry should be added to the m_potentialContacts array
		/// (and m_firstFreePotentialContact incremented).
		/// 
		/// If a contact point might to be useful as a reference point for welding, a pointer
		/// to this contact point should be added to the m_representativeContacts array
		/// (and m_firstFreeRepresentativeContact incremented).
		struct PotentialInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpProcessCollisionOutput::PotentialInfo );

			void reset()
			{ 
				m_firstFreePotentialContact      = &m_potentialContacts[0];
				m_firstFreeRepresentativeContact = &m_representativeContacts[0];
			}

				/// A pointer pointing just past the last used element in m_potentialContacts
			ContactRef*        m_firstFreePotentialContact;

				/// A pointer pointing just past the last used element in m_representativeContacts
			hkpProcessCdPoint** m_firstFreeRepresentativeContact;

				/// A list of contact points which can be used as a reference contact points
			hkpProcessCdPoint*  m_representativeContacts[ HK_MAX_CONTACT_POINT ];

				/// A list of potential or new contacts, used for hkAgent3 welding technology
			ContactRef         m_potentialContacts[ HK_MAX_CONTACT_POINT ];
		};
		
	public:
#if defined(HK_1N_MACHINE_SUPPORTS_WELDING)
			/// An optional pointer to a PotentialInfo welding information
		hkPadSpu<PotentialInfo*>    m_potentialContacts;
#endif

};



#endif // HK_COLLIDE2_PROCESS_COLLISION_OUTPUT_H

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
