/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLISION_EVENTS_H
#define HK_COLLISION_EVENTS_H

#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>
#include <Physics/Dynamics/Collide/hkpDynamicsContactMgr.h>
#include <Physics/ConstraintSolver/Constraint/Contact/hkpContactPointProperties.h>
#include <Physics/Collide/Agent/ContactMgr/hkpContactMgr.h>
#include <Physics/Collide/Agent/hkpProcessCollisionOutput.h>

#include <Physics/Dynamics/Collide/hkpSimpleConstraintContactMgr.h>

class hkpCollidable;
struct hkpCollisionInput;
struct hkpProcessCollisionOutput;
class hkpEntity;
class hkpDynamicsContactMgr;

/// Accept or reject contact points.
enum hkpContactPointAccept
{
		/// Accept.
	HK_CONTACT_POINT_ACCEPT = 0,
		/// Reject.
	HK_CONTACT_POINT_REJECT = 1
};


struct hkpToiPointAddedEvent;
struct hkpManifoldPointAddedEvent;
struct hkpSimpleContactConstraintAtom;

	/// This collision event is passed to a hkpCollisionListener's contactPointAddedCallback() just when
	/// a new contact point is created by the engine
struct hkpContactPointAddedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactPointAddedEvent );

		/// This is the hkpCdBody that was used (for entity A) to generate the contact point. If the entity's collision
		/// detection representation is a single shape (for example one hkpConvexVerticesShape) this will actually be
		/// a reference to the entity's hkpCollidable. However if the entity has a more complicated shape hierarchy
		/// (such as a hkpMeshShape) then this will be a stack allocated hkpCdBody object, which may also reference a
		/// stack allocated hkpShape. Do not store a pointer to this hkpCdBody, or the shape it references for this
		/// reason, as the objects may be temporary. You can call getRootCollidable() on the hkpCdBody, which will
		/// return a reference to the (permanent) heap created hkpCollidable owned by the entity.
		/// The shape this points to is always the "leaf" shape, e.g., the actual triangle
		/// that created the contact point.
	HK_PAD_ON_SPU(const hkpCdBody*) m_bodyA;
	
		/// This is the hkpCdBody that was used (for entity B) to generate the contact point. See the comments for m_bodyA
		/// for further details.
	HK_PAD_ON_SPU(const hkpCdBody*) m_bodyB;

	enum Type { TYPE_TOI, TYPE_MANIFOLD };
	const hkEnum<Type, hkInt32> m_type;

		/// The entity this collision listener was added to
		/// (or HK_NULL if the collision listener was added to the world)
	HK_PAD_ON_SPU(hkpEntity*) m_callbackFiredFrom;

		/// The contact point. This cannot be changed by the user - it needs to be done in process callback.
	HK_PAD_ON_SPU(const hkContactPoint*) m_contactPoint;
	
		/// Optional information about the feature type of that contact.
		/// This provides detailed information about the type of contact, e.g.
		/// point-point, point-edge .., edge-edge or point-triangle.
		/// Only valid if the GSK algorithm is used. However if hkpShapeCollection::m_disableWelding
		/// is set to false (default), this pointer might be HK_NULL.
	HK_PAD_ON_SPU(const hkpGskCache*) m_gskCache;
	
		/// The contact point properties, including friction and restitution.
		/// This is changeable (and is the best place to change it).
		/// This is set to HK_NULL if you're using hkpReportContactMgr.
	HK_PAD_ON_SPU(hkpContactPointProperties*)  m_contactPointProperties; 

		/// gets the body index to be used by getNumExtendedUserDatas and getExtendedUserData
	inline int getEntityIndex( const hkpEntity* entity ) const;

		/// gets the contact atom.
	inline const hkpSimpleContactConstraintAtom* getAtom() const;

		/// Gets number of extended user datas for that entity.
	inline int getNumExtendedUserDatas(const hkpEntity* entity) const;

		/// Gets all user datas into the 'datasOut' buffer. The buffer's size must match the size of extended datas.
	inline void getExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const;

		/// Writes all data from 'datasIn' buffer into the extended user data block. The buffer's size must match the size of extended datas.
	inline void setExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const;

		/// The relative velocity of the objects at the point of contact projected onto the collision normal.
		/// You can change its value, but it only has an effect for TOI contact points.
		/// \note This value is negative
	HK_PAD_ON_SPU(hkReal) m_projectedVelocity;        


		/// Whether the contact point will actually be added to the constraint solver or not.
		/// By default this is HK_CONTACT_POINT_ACCEPT. If you set this to HK_CONTACT_POINT_REJECT
		/// the contact point will be removed and a remove callback will be triggered immediately.
	HK_PAD_ON_SPU(hkpContactPointAccept) m_status;

	inline hkBool isToi() const;

		/// if the underlying contact point is a TOI contact point, this will give you a safe access
		/// to the additional properties.
	inline hkpToiPointAddedEvent& asToiEvent();

		/// if the underlying contact point is a normal collision point, this will give you a safe access
		/// to the additional properties.
	inline hkpManifoldPointAddedEvent& asManifoldEvent();

		//
		//	internal section
		//
		// The contact manager.
	HK_PAD_ON_SPU(hkpDynamicsContactMgr*) m_internalContactMgr;

	HK_PAD_ON_SPU(const struct hkpProcessCollisionInput*) m_collisionInput;

	HK_PAD_ON_SPU(struct hkpProcessCollisionOutput*) m_collisionOutput;
	

		// Creates a new hkpContactPointAddedEvent.
	inline hkpContactPointAddedEvent(	hkpDynamicsContactMgr* contactMgr,	
										const hkpProcessCollisionInput* m_collisionInput,
										hkpProcessCollisionOutput* m_collisionOutput,

										const hkpCdBody* a,    const hkpCdBody* b, 
										hkContactPoint* cp,   const hkpGskCache* gskCache, hkpContactPointProperties* cpp, 
										hkReal projectedVelocity, Type pointType );

};

	/// Additional properties, in case hkpContactPointAddedEvent is a TOI event.
struct hkpToiPointAddedEvent: public hkpContactPointAddedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpToiPointAddedEvent );

		/// The time of impact, you can modify this value
	HK_PAD_ON_SPU(hkTime) m_toi;

		/// Creates a new hkpContactPointAddedEvent.
	inline hkpToiPointAddedEvent(	hkpDynamicsContactMgr* contactMgr,	
									const hkpProcessCollisionInput* collisionInput,
									hkpProcessCollisionOutput* collisionOutput,
									const hkpCdBody* a,    const hkpCdBody* b, 
									hkContactPoint* cp,   const hkpGskCache* gskCache, hkpContactPointProperties* cpp, 
									hkTime toi, hkReal projectedVelocity );

};

	/// Additional properties if the added event was fired by the normal discrete collision detection.
struct hkpManifoldPointAddedEvent : public hkpContactPointAddedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpManifoldPointAddedEvent );

		/// Identifies the contact point in the contact mgr.
	HK_PAD_ON_SPU(hkContactPointId) m_contactPointId;

		/// This lets you modify the delay to the next process callback.
	mutable HK_PAD_ON_SPU(hkUint16)	m_nextProcessCallbackDelay;

	inline hkpManifoldPointAddedEvent(	hkContactPointId contactPointId, 
										hkpDynamicsContactMgr* contactMgr,	
										const hkpProcessCollisionInput* collisionInput,
										hkpProcessCollisionOutput* collisionOutput,
										const hkpCdBody* a,    const hkpCdBody* b, 
										hkContactPoint* cp,   const hkpGskCache* gskCache, hkpContactPointProperties* cpp, 
										hkReal projectedVelocity);

};


	/// This event is fired once for every new contact point exactly when
	/// its information is used for the very first time.
	/// You should use this:
	///     - to trigger high quality game events
	///
	/// \warning Due to the way this function is implemented, you cannot use it to
	/// apply forces or impulses at rigid bodies. The reason is that
	/// this callback is fired by the solver and at this stage the rigid
	/// body velocities are copied to internal solver data already.
struct hkpContactPointConfirmedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactPointConfirmedEvent );

		/// The first entity's hkpCollidable. Note that this is not the "leaf" hkpCdBody (as is the case for the hkpContactPointAddedEvent).
		/// This is because these events are fired after collision detection has been performed, and the hkpCdBody objects needed
		/// to create the contact points may no longer exist.
	const hkpCollidable* m_collidableA;
	
		/// The second entity's hkpCollidable. Note that this is not the "leaf" hkpCdBody (as is the case for the hkpContactPointAddedEvent).
		/// This is because these events are fired after collision detection has been performed, and the hkpCdBody objects needed
		/// to create the contact points may no longer exist.
	const hkpCollidable* m_collidableB;

		/// The entity this collision listener was added to
		/// (or HK_NULL if the collision listener was added to the world).
	hkpEntity* m_callbackFiredFrom; 

		/// returns true if the contact point is a TOI contact point.
	inline hkBool isToi() const;

		//
		// Changeable attributes:
		//

		/// The contact point.
	hkContactPoint* m_contactPoint;

		/// The contact point properties, including friction and restitution.
		/// This is set to HK_NULL if you're using hkpReportContactMgr.
	hkpContactPointProperties*  m_contactPointProperties;

		/// gets the body index to be used by getNumExtendedUserDatas and getExtendedUserData.
	inline int getEntityIndex( const hkpEntity* entity ) const;

		/// gets the contact atom.
	inline const hkpSimpleContactConstraintAtom* getAtom() const;

		/// Gets number of extended user datas for that entity.
	inline int getNumExtendedUserDatas(const hkpEntity* entity) const;

		/// Gets all user datas into the 'datasOut' buffer. The buffer's size must match the size of extended datas.
	inline void getExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const;

		/// Writes all data from 'datasIn' buffer into the extended user data block. The buffer's size must match the size of extended datas.
	inline void setExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const;

		/// Specifies how much the contact normal should be rotated in TOI events. This is only relevant for TOI points.
		/// When set to non-zero value, this parameter reduces number of TOI collisions, artificially adding separating velocity to colliding bodies.
		/// The higher the relative velocity of the bodies in the contact's tangent plane, the higher the extra impulse.
		/// \note This causes serious artifacts for collisions of e.g., a car riding along a wall and then crashing/sliding into the wall. Similarly,
		///       it may cause artifacts, when a car's chassis collides with landscape. It is advised to zero this parameter in a callback in such cases.
	hkReal m_rotateNormal;

		/// The relative velocity of the objects at the point of contact projected onto the collision normal.
		/// Changing this value influences the restitution. If this event is a TOI event, you should
		/// only decrease its value, otherwise objects can sink in at the expense of tons of extra CPU.
	hkReal m_projectedVelocity;     

	const hkEnum<hkpContactPointAddedEvent::Type, hkInt32> m_type;


		/// Gets the contact point id of this point (or -1 for TOIs).
	hkContactPointId getContactPointId() const;

		/// Gets the DynamicsContactMgr: Warning: This function only works if you are not implementing your own
		/// hkpDynamicsContactMgr.
		// 
	const class hkpDynamicsContactMgr* getContactMgr() const;

		// Internal section
		// 

		/// Pointer to the contact constraint data that triggered the callback. This can be used to access the contact manager and
		/// extract the id of the contact point. This is only valid for in-manifold contact points, and is set to null for TOI contact points.
	const class hkpSimpleContactConstraintData* m_contactData;



		// Creates a new hkpContactPointAddedEvent.
	inline hkpContactPointConfirmedEvent(	hkpContactPointAddedEvent::Type type,
											const hkpCollidable* a, const hkpCollidable* b, 
											const hkpSimpleContactConstraintData* data,
											hkContactPoint* cp,	hkpContactPointProperties* cpp, 
											hkReal rotateNormal,
											hkReal projectedVelocity);



};




	/// This collision event is passed to a hkpCollisionListener's contactPointRemovedCallback() before a contact point is removed.
struct hkpContactPointRemovedEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactPointRemovedEvent );

		/// gets the body index to be used by getNumExtendedUserDatas and getExtendedUserData
	inline int getEntityIndex( const hkpEntity* entity ) const;

		/// gets the contact atom.
	inline const hkpSimpleContactConstraintAtom* getAtom() const;

		/// Gets number of extended user datas for that entity.
	inline int getNumExtendedUserDatas(const hkpEntity* entity) const;

		/// Gets all user datas into the 'datasOut' buffer. The buffer's size must match the size of extended datas.
	inline void getExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const;

		/// Writes all data from 'datasIn' buffer into the extended user data block. The buffer's size must match the size of extended datas.
	inline void setExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const;


		/// Identifies the contact point. Is set to HK_INVALID_CONTACT_POINT if this contact point was a TOI.
	HK_PAD_ON_SPU(hkContactPointId) m_contactPointId;
			
		/// The properties of the contact point.
	HK_PAD_ON_SPU(hkpContactPointProperties*) m_contactPointProperties;

		/// The first entity.
	HK_PAD_ON_SPU(hkpEntity*) m_entityA;

		/// The second entity.
	HK_PAD_ON_SPU(hkpEntity*) m_entityB;
	
		/// The entity this collision listener was added to
		/// (or HK_NULL if the collision listener was added to the world).
	HK_PAD_ON_SPU(hkpEntity*) m_callbackFiredFrom;

		/// returns true if the contact point is a TOI contact point.
	inline hkBool isToi();

		//
		//	Internal section	
		//

		// Creates a new hkpContactPointRemovedEvent.
	inline hkpContactPointRemovedEvent( hkContactPointId contactPointId, hkpDynamicsContactMgr* contactMgr,	class hkpConstraintOwner* constraintOwner, hkpContactPointProperties* prop, hkpEntity* a,	hkpEntity* b);

		// The contact manager.
	HK_PAD_ON_SPU(hkpDynamicsContactMgr*) m_internalContactMgr;

	HK_PAD_ON_SPU(hkpConstraintOwner*) m_constraintOwner;
};

	/// This collision event is passed to a hkpCollisionListener's contactProcessCallback() just after
	/// all collision agents between a pair of bodies have been executed.
struct hkpContactProcessEvent
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactProcessEvent );

	const hkpContactProcessEvent& operator=(const hkpContactProcessEvent&);

		/// gets the body index to be used by getNumExtendedUserDatas and getExtendedUserData
	inline int getEntityIndex( const hkpEntity* entity ) const;

		/// gets the contact atom.
	inline const hkpSimpleContactConstraintAtom* getAtom() const;

		/// Gets number of extended user datas for that entity.
	inline int getNumExtendedUserDatas(const hkpEntity* entity) const;

		/// Gets all user datas into the 'datasOut' buffer. The buffer's size must match the size of extended datas.
	inline void getExtendedUserDatas(int contactPointIndex, const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const;

		/// Writes all data from 'datasIn' buffer into the extended user data block. The buffer's size must match the size of extended datas.
	inline void setExtendedUserDatas(int contactPointIndex, const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const;

		/// Gets all user datas into the 'datasOut' buffer. The buffer's size must match the size of extended datas.
	inline void getToiExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const;

		/// Writes all data from 'datasIn' buffer into the extended user data block. The buffer's size must match the size of extended datas.
	inline void setToiExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const;

		/// The first entity's hkpCollidable. Note that this is not the "leaf" hkpCdBody (as is the case for the hkpContactPointAddedEvent).
		/// This is because these events are fired after collision detection has been performed, and the hkpCdBody objects needed
		/// to create the contact points may no longer exist.
	HK_PAD_ON_SPU(const hkpCollidable*) m_collidableA;
	
		/// The second entity's hkpCollidable. Note that this is not the "leaf" hkpCdBody (as is the case for the hkpContactPointAddedEvent).
		/// This is because these events are fired after collision detection has been performed, and the hkpCdBody objects needed
		/// to create the contact points may no longer exist.
	HK_PAD_ON_SPU(const hkpCollidable*) m_collidableB;
	
		/// The entity this collision listener was added to
		/// (or HK_NULL if the collision listener was added to the world).
	HK_PAD_ON_SPU(hkpEntity*) m_callbackFiredFrom;

		/// All contact points including the TOI contact point.
	HK_PAD_ON_SPU(hkpProcessCollisionData*) m_collisionData;
    
		/// An array of additional properties, like friction, one pointer for each contact point in
		/// the m_collisionData->m_contactPoints array
		/// This is only available on PPU. On SPU use m_internalContactMgr->getContactPointProperty().
	hkpContactPointProperties* m_contactPointProperties[ HK_MAX_CONTACT_POINT ];

		//
		//	Internal section
		//

		// Access to the contact manager
	HK_PAD_ON_SPU(hkpDynamicsContactMgr*) m_internalContactMgr;

		/// Creates a new hkpContactProcessEvent.
	inline hkpContactProcessEvent( 	hkpDynamicsContactMgr* contactMgr,	const hkpCollidable* a,	const hkpCollidable* b,	hkpProcessCollisionData* cr);
};

#include <Physics/Dynamics/Collide/Deprecated/hkpCollisionEvents.inl>


#endif // HK_COLLISION_EVENTS_H

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
