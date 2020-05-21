/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkpMotion* hkpEntity::getMotion()
{
	return &m_motion;
}

inline hkpSimulationIsland* hkpEntity::getSimulationIsland() const
{	
	return m_simulationIsland;
}

hkpAction* hkpEntity::getAction(int i)
{
	return m_actions[i];
}

int hkpEntity::getNumActions() const
{
	return m_actions.getSize();
}


inline const hkSmallArray<hkpEntityListener*>& hkpEntity::getEntityListeners() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadOnly();
#endif
	if ( !m_extendedListeners )
	{
		m_extendedListeners = new ExtendedListeners;
	}
	return m_extendedListeners->m_entityListeners;
}

inline const hkSmallArray<hkpEntityActivationListener*>& hkpEntity::getEntityActivationListeners() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadOnly();
#endif
	if ( !m_extendedListeners )
	{
		m_extendedListeners = new ExtendedListeners;
	}
	return m_extendedListeners->m_activationListeners;
}


inline const hkSmallArray<hkpContactListener*>& hkpEntity::getContactListeners() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkReadOnly();
#endif
	return m_contactListeners;
}

inline bool hkpEntity::areContactListenersAdded() const
{
	return m_contactListeners.getSize() > 0;
}

inline hkUint16 hkpEntity::getContactPointCallbackDelay() const
{
	return m_contactPointCallbackDelay;
}

inline void hkpEntity::setContactPointCallbackDelay( hkUint16 delay )
{
	m_contactPointCallbackDelay = delay;
}

inline hkpMaterial& hkpEntity::getMaterial()
{
	return m_material;
}

inline const hkpMaterial& hkpEntity::getMaterial() const
{
	return m_material;
}

inline hkdBreakableBody* hkpEntity::getBreakableBody() const
{
	return reinterpret_cast<hkdBreakableBody*>(m_breakableBody);
}


inline hkBool hkpEntity::isFixed() const
{
	return m_motion.m_type == hkpMotion::MOTION_FIXED;
}

hkBool hkpEntity::isFixedOrKeyframed() const
{
	return (m_motion.m_type == hkpMotion::MOTION_FIXED) || (m_motion.m_type == hkpMotion::MOTION_KEYFRAMED);
}

inline hkUint32 hkpEntity::getUid() const
{
	return m_uid;
}

const hkSmallArray<struct hkConstraintInternal>&  hkpEntity::getConstraintMasters() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	return getConstraintMastersImpl();
#else
	return m_constraintsMaster;
#endif
}

hkSmallArray<struct hkConstraintInternal>&  hkpEntity::getConstraintMastersRw()
{
#ifdef HK_DEBUG_MULTI_THREADING
	return getConstraintMastersRwImpl();
#else
	return m_constraintsMaster;
#endif
}

const hkArray<class hkpConstraintInstance*>&  hkpEntity::getConstraintSlaves() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	return getConstraintSlavesImpl();
#else
	return m_constraintsSlave;
#endif
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
