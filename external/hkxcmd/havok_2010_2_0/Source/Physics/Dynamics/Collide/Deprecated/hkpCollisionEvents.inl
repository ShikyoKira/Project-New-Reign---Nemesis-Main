/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkpContactPointAddedEvent::hkpContactPointAddedEvent(	hkpDynamicsContactMgr* contactMgr,
															const hkpProcessCollisionInput* collisionInput,
															hkpProcessCollisionOutput* collisionOutput,
															const hkpCdBody* a,	const hkpCdBody* b,
															hkContactPoint* ccp, 
															const hkpGskCache* gskCache,
															hkpContactPointProperties* dcp, 
															hkReal projectedVelocity,
															Type pointType )

:	m_bodyA( a ),
	m_bodyB( b ),
	m_type( pointType ),
	m_contactPoint( ccp ),
	m_gskCache(gskCache),
	m_contactPointProperties( dcp ),
	m_projectedVelocity( projectedVelocity ),
	m_internalContactMgr( contactMgr ),
	m_collisionInput( collisionInput ),
	m_collisionOutput( collisionOutput )
{
	m_status = HK_CONTACT_POINT_ACCEPT;
}

inline hkpToiPointAddedEvent::hkpToiPointAddedEvent(	hkpDynamicsContactMgr* contactMgr,	
													const hkpProcessCollisionInput* collisionInput,
													hkpProcessCollisionOutput* collisionOutput,
													const hkpCdBody* a,	const hkpCdBody* b, 
													hkContactPoint* cp,  const hkpGskCache* gskCache, hkpContactPointProperties* cpp, 
													hkTime toi, hkReal projectedVelocity)
	:   hkpContactPointAddedEvent(  contactMgr, collisionInput, collisionOutput, a, b, cp, gskCache, cpp, projectedVelocity, TYPE_TOI), m_toi(toi)
{
}

inline hkpManifoldPointAddedEvent::hkpManifoldPointAddedEvent(	hkContactPointId contactPointId, 
																hkpDynamicsContactMgr* contactMgr,	
																const hkpProcessCollisionInput* collisionInput,
																hkpProcessCollisionOutput* collisionOutput,
																const hkpCdBody* a,	const hkpCdBody* b, 
																hkContactPoint* cp,   const hkpGskCache* gskCache, hkpContactPointProperties* cpp, 
																hkReal projectedVelocity)
	:   hkpContactPointAddedEvent(  contactMgr, collisionInput, collisionOutput, a, b, cp, gskCache, cpp, projectedVelocity, TYPE_MANIFOLD ), m_contactPointId( contactPointId )
{
	if ( contactMgr->m_type == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR )
	{
		m_nextProcessCallbackDelay = static_cast<hkpSimpleConstraintContactMgr*>( contactMgr )->m_contactPointCallbackDelay;
	}
	else
	{
		m_nextProcessCallbackDelay = 0;
	}
}

inline hkpContactPointConfirmedEvent::hkpContactPointConfirmedEvent(	hkpContactPointAddedEvent::Type type,
																	const hkpCollidable* a, const hkpCollidable* b, 
																	const hkpSimpleContactConstraintData* data,
																	hkContactPoint* cp,	hkpContactPointProperties* cpp, 
																	hkReal rotateNormal,
																	hkReal projectedVelocity)
																	:   m_collidableA(a), m_collidableB(b),
																	m_contactPoint(cp),	m_contactPointProperties( cpp ),
																	m_rotateNormal(rotateNormal),
																	m_projectedVelocity( projectedVelocity ),
																	m_type(type), m_contactData(data)
{
}

hkpContactPointRemovedEvent::hkpContactPointRemovedEvent(	hkContactPointId contactPointId, 
														hkpDynamicsContactMgr* contactMgr,	
														hkpConstraintOwner* constraintOwner,
														hkpContactPointProperties* prop, 
														hkpEntity* ca,	
														hkpEntity* cb)
	:	m_contactPointId(contactPointId),
		m_contactPointProperties(prop),
		m_entityA(ca),
		m_entityB(cb),
		m_internalContactMgr(contactMgr),
		m_constraintOwner( constraintOwner )
{
}

hkpContactProcessEvent::hkpContactProcessEvent( hkpDynamicsContactMgr* contactMgr, const hkpCollidable* ca, const hkpCollidable* cb, hkpProcessCollisionData* data)
	:	m_collidableA(ca),
		m_collidableB(cb),
		m_collisionData( data ),
		m_internalContactMgr(contactMgr)
{
}

inline hkBool hkpContactPointRemovedEvent::isToi()
{
	return m_contactPointId == HK_INVALID_CONTACT_POINT;
}

inline hkpToiPointAddedEvent& hkpContactPointAddedEvent::asToiEvent()
{
	HK_ASSERT2( 0xf043dea2, m_type == this->TYPE_TOI, "Invalid upcast from hkpContactPointAddedEvent to hkpToiPointAddedEvent" );
	return static_cast<hkpToiPointAddedEvent&>(*this);
}

inline hkBool hkpContactPointAddedEvent::isToi() const
{
	return m_type == this->TYPE_TOI;

}


inline hkpManifoldPointAddedEvent& hkpContactPointAddedEvent::asManifoldEvent()
{
	HK_ASSERT2( 0xf043dea2, m_type == this->TYPE_MANIFOLD, "Invalid upcast from hkpContactPointAddedEvent to hkpManifoldPointAddedEvent" );
	return static_cast<hkpManifoldPointAddedEvent&>(*this);
}

inline hkBool hkpContactPointConfirmedEvent::isToi() const 
{
	return m_type == hkpContactPointAddedEvent::TYPE_TOI;
}


#define getEntityIndex_BODY(contactMgr) \
	HK_ASSERT2(0xad875caa, contactMgr->getType() == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR, "Unsupported manager type.");\
	const hkpSimpleConstraintContactMgr* mgr = static_cast<const hkpSimpleConstraintContactMgr*>(contactMgr);\
	return ( mgr->m_constraint.getEntityA() == entity ) ? 0 : 1;

int hkpContactPointAddedEvent    ::getEntityIndex( const hkpEntity* entity ) const { getEntityIndex_BODY(m_internalContactMgr) }
int hkpContactPointConfirmedEvent::getEntityIndex( const hkpEntity* entity ) const { getEntityIndex_BODY(getContactMgr()) }
int hkpContactProcessEvent       ::getEntityIndex( const hkpEntity* entity ) const { getEntityIndex_BODY(m_internalContactMgr) }
int hkpContactPointRemovedEvent  ::getEntityIndex( const hkpEntity* entity ) const { getEntityIndex_BODY(m_internalContactMgr) }
#undef getEntityIndex_BODY

#define getAtom_BODY(contactMgr) \
	HK_ASSERT2(0xad875caa, contactMgr->getType() == hkpContactMgr::TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR, "Unsupported manager type.");\
	return static_cast<const hkpSimpleConstraintContactMgr*>(contactMgr)->getAtom();

const hkpSimpleContactConstraintAtom* hkpContactPointAddedEvent    ::getAtom() const { getAtom_BODY(m_internalContactMgr) }
const hkpSimpleContactConstraintAtom* hkpContactPointConfirmedEvent::getAtom() const { getAtom_BODY(getContactMgr()) }
const hkpSimpleContactConstraintAtom* hkpContactProcessEvent       ::getAtom() const { getAtom_BODY(m_internalContactMgr) }
const hkpSimpleContactConstraintAtom* hkpContactPointRemovedEvent  ::getAtom() const { getAtom_BODY(m_internalContactMgr) }
#undef getAtom_BODY

#define getNumExtendedUserDatas_BODY \
	int bodyIndex = getEntityIndex(entity);\
	const hkpSimpleContactConstraintAtom* atom = getAtom();\
	return hkpContactPointProperties::getNumExtendedUserDatas(atom, bodyIndex);

int hkpContactPointAddedEvent    ::getNumExtendedUserDatas(const hkpEntity* entity) const { getNumExtendedUserDatas_BODY }
int hkpContactPointConfirmedEvent::getNumExtendedUserDatas(const hkpEntity* entity) const { getNumExtendedUserDatas_BODY }
int hkpContactProcessEvent::getNumExtendedUserDatas(const hkpEntity* entity) const { getNumExtendedUserDatas_BODY }
int hkpContactPointRemovedEvent  ::getNumExtendedUserDatas(const hkpEntity* entity) const { getNumExtendedUserDatas_BODY }
#undef getNumExtendedUserDatas_BODY


#define getExtendedUserDatas_BODY \
	int bodyIndex = getEntityIndex(entity);\
	const hkpSimpleContactConstraintAtom* atom = getAtom();\
	HK_ASSERT2(0xad67343a, numDatas == m_contactPointProperties->getNumExtendedUserDatas(atom, bodyIndex), "Buffer size doesn't match extended data size.");\
	for (int i = 0; i < numDatas; i++)\
	{\
		*datasOut = m_contactPointProperties->getExtendedUserData(atom, bodyIndex, i);\
		datasOut++;\
	}

void hkpContactPointAddedEvent    ::getExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const { getExtendedUserDatas_BODY }
void hkpContactPointConfirmedEvent::getExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const { getExtendedUserDatas_BODY }
void hkpContactPointRemovedEvent  ::getExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const { getExtendedUserDatas_BODY }
#undef getExtendedUserDatas_BODY

#define setExtendedUserDatas_BODY \
	int bodyIndex = getEntityIndex(entity);\
	const hkpSimpleContactConstraintAtom* atom = getAtom();\
	HK_ASSERT2(0xad67343a, numDatas == m_contactPointProperties->getNumExtendedUserDatas(atom, bodyIndex), "Buffer size doesn't match extended data size.");\
	for (int i = 0; i < numDatas; i++)\
	{\
		m_contactPointProperties->getExtendedUserData(atom, bodyIndex, i) = *datasIn;\
		datasIn++;\
	}

void hkpContactPointAddedEvent::setExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const { setExtendedUserDatas_BODY }
void hkpContactPointConfirmedEvent::setExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const { setExtendedUserDatas_BODY }
void hkpContactPointRemovedEvent::setExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const { setExtendedUserDatas_BODY }
#undef setExtendedUserDatas_BODY


void hkpContactProcessEvent::getExtendedUserDatas(int contactPointIndex, const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const
{
	int bodyIndex = getEntityIndex(entity);
	const hkpSimpleContactConstraintAtom* atom = getAtom();
	HK_ASSERT2(0xad67343a, numDatas == m_contactPointProperties[contactPointIndex]->getNumExtendedUserDatas(atom, bodyIndex), "Buffer size doesn't match extended data size.");
	for (int i = 0; i < numDatas; i++)
	{
		*datasOut = m_contactPointProperties[contactPointIndex]->getExtendedUserData(atom, bodyIndex, i);
		datasOut++;
	}
}

void hkpContactProcessEvent::setExtendedUserDatas(int contactPointIndex, const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const
{
	int bodyIndex = getEntityIndex(entity);
	const hkpSimpleContactConstraintAtom* atom = getAtom();
	HK_ASSERT2(0xad67343a, numDatas == m_contactPointProperties[contactPointIndex]->getNumExtendedUserDatas(atom, bodyIndex), "Buffer size doesn't match extended data size.");
	for (int i = 0; i < numDatas; i++)
	{
		m_contactPointProperties[contactPointIndex]->getExtendedUserData(atom, bodyIndex, i) = *datasIn;
		datasIn++;
	}
}

void hkpContactProcessEvent::getToiExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasOut, int numDatas) const
{
	int bodyIndex = getEntityIndex(entity);
	const hkpSimpleContactConstraintAtom* atom = getAtom();
	HK_ASSERT2(0xad67343a, numDatas == m_collisionData->getToiProperties().getNumExtendedUserDatas(atom, bodyIndex), "Buffer size doesn't match extended data size.");
	for (int i = 0; i < numDatas; i++)
	{
		*datasOut = m_collisionData->getToiProperties().getExtendedUserData(atom, bodyIndex, i);
		datasOut++;
	}
}

void hkpContactProcessEvent::setToiExtendedUserDatas(const hkpEntity* entity, hkpContactPointProperties::UserData* datasIn, int numDatas) const
{
	int bodyIndex = getEntityIndex(entity);
	const hkpSimpleContactConstraintAtom* atom = getAtom();
	HK_ASSERT2(0xad67343a, numDatas == m_collisionData->getToiProperties().getNumExtendedUserDatas(atom, bodyIndex), "Buffer size doesn't match extended data size.");
	for (int i = 0; i < numDatas; i++)
	{
		m_collisionData->getToiProperties().getExtendedUserData(atom, bodyIndex, i) = *datasIn;
		datasIn++;
	}
}

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
