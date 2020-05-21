/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_DYNAMICS_CONTACT_MGR_H
#define HK_DYNAMICS2_DYNAMICS_CONTACT_MGR_H

#include <Physics/Collide/Agent/ContactMgr/hkpContactMgr.h>

class hkContactPoint;
class hkpContactPointProperties;
class hkVector4;


///An interface to access contact point information.
class hkpDynamicsContactMgr: public hkpContactMgr
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpDynamicsContactMgr( Type type ): hkpContactMgr( type ) {}

			/// Gets the properties attached to the contact point (if available)
		virtual hkpContactPointProperties* getContactPointProperties( hkContactPointId id ){ return HK_NULL;}

			/// Gets the dynamics contact point (if available)
		virtual hkContactPoint* getContactPoint( hkContactPointId id ){ return HK_NULL;}

			/// Gets the ids of all contact points in this collision
		virtual void getAllContactPointIds( hkArray<hkContactPointId>& contactPointIds ) const {}

		virtual Type getType() const = 0;

			/// Virtual function which is called just before a simple collision response is executed
			/// in a TOI. The default implementation will forward this to the hkpSimpleContactConstraintData::collisionResponseBeginCallback.
			/// This is used for the mass changer and moving surface utility
		virtual void toiCollisionResponseBeginCallback( const hkContactPoint& cp, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB){}

			/// Virtual function which is called just after a simple collision response is executed
			/// The default implementation will forward this to the hkpSimpleContactConstraintData::collisionResponseEndCallback.
			/// This is used for the mass changer and moving surface utility
		virtual void toiCollisionResponseEndCallback( const hkContactPoint& cp, hkReal impulseApplied, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB){}

			/// Return the constraint if there is one
		virtual class hkpConstraintInstance* getConstraintInstance() { return HK_NULL; } 

			/// Fire contactPointCallbacks when the earliest TOI callback has been identified.
			/// \return True if the TOI should be processed.
		virtual hkBool fireCallbacksForEarliestToi( struct hkpToiEvent& event, hkReal& rotateNormal ) { return false; }

			/// Apply custom TOI-collision handling before localized solving; this method actually belongs to hkpDynamicsContactMgr interface.
		virtual void confirmToi( struct hkpToiEvent& event, hkReal rotateNormal, class hkArray<class hkpEntity*>& outToBeActivated ){}

	protected:
		class hkpWorld*					m_world;
};

#endif // HK_DYNAMICS2_DYNAMICS_CONTACT_MGR_H

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
