/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkpEntity* hkpConstraintInstance::getEntityA() const
{
	return m_entities[0];
}

inline hkpEntity* hkpConstraintInstance::getEntity( int index ) const
{
	return m_entities[index];
}

inline hkpEntity* hkpConstraintInstance::getEntityB() const
{
	return m_entities[1];
}


inline hkpRigidBody* hkpConstraintInstance::getRigidBodyA() const
{
	return reinterpret_cast<hkpRigidBody*>(m_entities[0]);
}

inline hkpRigidBody* hkpConstraintInstance::getRigidBodyB() const
{
	return reinterpret_cast<hkpRigidBody*>(m_entities[1]);
}



inline hkpEntity* hkConstraintInternal::getMasterEntity() const
{
	return m_entities[m_whoIsMaster];
}

inline hkpEntity* hkConstraintInternal::getSlaveEntity() const
{
	return m_entities[1-m_whoIsMaster];
}

inline hkpEntity* hkpConstraintInstance::getMasterEntity() const
{
	HK_ASSERT2( 0xf056d145, m_internal, "You cannot access the master entity, this constraint is not added to the world yet" );
	return m_internal->getMasterEntity();
}

inline hkpEntity* hkpConstraintInstance::getSlaveEntity() const
{
	HK_ASSERT2( 0xf056d145, m_internal, "You cannot access the slave entity, this constraint is not added to the world yet" );
	return m_internal->getSlaveEntity();
}

inline void hkpConstraintInstance::setWantRuntime( hkBool b )
{
	HK_ASSERT2( 0xf03de567, HK_NULL == m_owner, "You cannot call setWantRuntime after you have added the constraint to the world" );
	m_wantRuntime = b;
}

inline hkBool hkpConstraintInstance::getWantRuntime() const
{
	return m_wantRuntime;
}

inline hkpEntity* hkConstraintInternal::getOtherEntity( const hkpEntity* entity ) const
{
	hkUlong a = hkUlong( m_entities[0] );
	hkUlong b = hkUlong( m_entities[1] );
	hkUlong c = hkUlong( entity );
	return reinterpret_cast<hkpEntity*>( a^b^c );
}

inline hkpConstraintOwner* hkpConstraintInstance::getOwner() const
{
	return m_owner;
}

void hkpConstraintInstance::setOwner( hkpConstraintOwner* owner )
{
	m_owner = owner;
}

inline const hkpConstraintData* hkpConstraintInstance::getData() const
{
	return m_data;
}

inline hkpConstraintData* hkpConstraintInstance::getDataRw() const
{
	return m_data;
}

inline hkConstraintInternal* hkpConstraintInstance::getInternal()
{
	HK_ASSERT2( 0xf056d145, m_internal, "You cannot access internal, this constraint is not added to the world yet" );
	return m_internal;
}

inline hkpConstraintRuntime* hkpConstraintInstance::getRuntime() const
{
	HK_ASSERT2( 0xf056d145, m_internal, "You cannot access internal, this constraint is not added to the world yet" );
	return m_internal->m_runtime;
}


inline hkUlong hkpConstraintInstance::getUserData() const
{
	return m_userData;
}

inline void hkpConstraintInstance::setUserData( hkUlong data )
{
	//HK_ACCESS_CHECK_WITH_PARENT( m_entities[0]->getWorld(), HK_ACCESS_IGNORE, this, HK_ACCESS_RW );
	m_userData = data;
}

inline const char* hkpConstraintInstance::getName() const
{
	return m_name;
}

inline void hkpConstraintInstance::setName( const char* name )
{
	m_name = name;
}

inline hkpConstraintInstance::ConstraintPriority hkpConstraintInstance::getPriority() const
{
	return m_priority;
}

inline hkpEntity* hkpConstraintInstance::getOtherEntity( const hkpEntity* entity ) const
{
	return hkSelectOther( const_cast<hkpEntity*>(entity), m_entities[0], m_entities[1]);
}

void hkConstraintInternal::getConstraintInfo( hkpConstraintInfo& info ) const
{
	info.m_maxSizeOfSchema     = m_sizeOfSchemas;
	info.m_sizeOfSchemas       = m_sizeOfSchemas;
	info.m_numSolverResults    = m_numSolverResults;
	info.m_numSolverElemTemps  = m_numSolverElemTemps;
}

void hkConstraintInternal::clearConstraintInfo( )
{
	this->m_numSolverResults = 0;
	this->m_numSolverElemTemps = 0;
	this->m_sizeOfSchemas    = 0;
}

void hkConstraintInternal::addConstraintInfo( const hkpConstraintInfo& delta)
{
	const hkUint32 numSolverResults   = this->m_numSolverResults   + delta.m_numSolverResults;
	const hkUint32 numSolverElemTemps = this->m_numSolverElemTemps + delta.m_numSolverElemTemps;
	const hkUint32 sizeOfSchemas      = this->m_sizeOfSchemas      + delta.m_sizeOfSchemas;

	HK_ASSERT2( 0xf0ff3244, numSolverResults < 0x10000, "Constraint too big for system" );
	HK_ASSERT2( 0xf0ff3244, numSolverElemTemps < 0x10000, "Constraint too big for system" );
	HK_ASSERT2( 0xf0ff3246, sizeOfSchemas    < 0x10000, "Constraint too big for system" );

	this->m_numSolverResults   = hkUint16(numSolverResults);
	this->m_numSolverElemTemps = hkUint16(numSolverElemTemps);
	this->m_sizeOfSchemas      = hkUint16(sizeOfSchemas);
}

void hkConstraintInternal::subConstraintInfo( const hkpConstraintInfo& delta)
{
	const hkUint32 numSolverResults   = this->m_numSolverResults   - delta.m_numSolverResults;
	const hkUint32 numSolverElemTemps = this->m_numSolverElemTemps - delta.m_numSolverElemTemps;
	const hkUint32 sizeOfSchemas      = this->m_sizeOfSchemas      - delta.m_sizeOfSchemas;

	HK_ASSERT2( 0xf0fe3244, numSolverResults < 0x10000, "Constraint internal inconsistency" );
	HK_ASSERT2( 0xf0fe3244, numSolverElemTemps < 0x10000, "Constraint internal inconsistency" );
	HK_ASSERT2( 0xf0fe3246, sizeOfSchemas    < 0x10000, "Constraint internal inconsistency" );

	this->m_numSolverResults = hkUint16(numSolverResults);
	this->m_numSolverElemTemps = hkUint16(numSolverElemTemps);
	this->m_sizeOfSchemas    = hkUint16(sizeOfSchemas);
}

hkpModifierConstraintAtom* hkpConstraintInstance::getConstraintModifiers() const
{
	return m_constraintModifiers;
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
