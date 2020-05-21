/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline const hkpProcessCollisionInput* hkpWorld::getCollisionInput() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_collisionInput;
}

inline hkpSolverInfo* hkpWorld::getSolverInfo()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	return &m_dynamicsStepInfo.m_solverInfo;
}


inline hkpProcessCollisionInput* hkpWorld::getCollisionInputRw()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	return m_collisionInput;
}


inline hkpBroadPhase* hkpWorld::getBroadPhase() 
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	return m_broadPhase;
}

inline const hkpBroadPhase* hkpWorld::getBroadPhase() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_broadPhase;
}


inline hkpCollisionDispatcher* hkpWorld::getCollisionDispatcher() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_collisionDispatcher;
}

inline const hkVector4& hkpWorld::getGravity() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_gravity;
}


hkpRigidBody* hkpWorld::getFixedRigidBody()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	return m_fixedRigidBody;
}

const hkpRigidBody* hkpWorld::getFixedRigidBody() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_fixedRigidBody;
}

inline const hkpSimulationIsland* hkpWorld::getFixedIsland() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_fixedIsland;
}


inline const hkArray<hkpSimulationIsland*>& hkpWorld::getInactiveSimulationIslands() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_inactiveSimulationIslands;
}

const hkArray<hkpSimulationIsland*>& hkpWorld::getActiveSimulationIslands() const
{
#ifdef HK_DEBUG_MULTI_THREADING
	checkAccessGetActiveSimulationIslands();
#endif
	return m_activeSimulationIslands;
}

inline const hkArray<hkpPhantom*>& hkpWorld::getPhantoms() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_phantoms;
}

inline const hkpCollisionFilter* hkpWorld::getCollisionFilter() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_collisionFilter;
}

inline const hkArray<hkpContactListener*>& hkpWorld::getContactListeners() const
{
	return m_contactListeners;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Locking the world, and delaying worldOperations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void hkpWorld::lockCriticalOperations()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	m_criticalOperationsLockCount++;
	HK_ASSERT2(0xad000203, m_criticalOperationsLockCount <= 100, "Internal Error: m_criticalOperationsLockCount corrupted (count went above 100)");
}

void hkpWorld::unlockCriticalOperations()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	m_criticalOperationsLockCount--;
	HK_ASSERT2(0xad000202, m_criticalOperationsLockCount >= 0, "Internal Error: m_criticalOperationsLockCount corrupted (count went below 0)");
}

int hkpWorld::areCriticalOperationsLocked( ) const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	HK_ASSERT2(0xad000200, m_criticalOperationsAllowed, "Warning: areCriticalOperationsLocked() queried while critical operations are disallowed (... and this query is (very probably) only performed when executing critical operations)");
	return m_criticalOperationsLockCount;
}

int hkpWorld::areCriticalOperationsLockedUnchecked( ) const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	return m_criticalOperationsLockCount;
}


void hkpWorld::lockCriticalOperationsForPhantoms()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	m_criticalOperationsLockCountForPhantoms++;
	HK_ASSERT2(0x7cd945a0, m_criticalOperationsLockCountForPhantoms == -1 || m_criticalOperationsLockCountForPhantoms == 0, "Internal Error: m_criticalOperationsLockCountForPhantoms corrupted.");
}

void hkpWorld::unlockCriticalOperationsForPhantoms()
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	m_criticalOperationsLockCountForPhantoms--;
	HK_ASSERT2(0x72141b94, m_criticalOperationsLockCountForPhantoms == -1 || m_criticalOperationsLockCountForPhantoms == 0, "Internal Error: m_criticalOperationsLockCountForPhantoms corrupted.");
}

int hkpWorld::areCriticalOperationsLockedForPhantoms() const
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RO );
	HK_ASSERT2(0xad000200, m_criticalOperationsAllowed, "Warning: areCriticalOperationsLocked() queried while critical operations are disalloed (... and this query is (very probably) only performed when executing critical operations)");
	HK_ASSERT(0x31a7f4ac, m_criticalOperationsLockCount + m_criticalOperationsLockCountForPhantoms >= 0);
	return m_criticalOperationsLockCount + m_criticalOperationsLockCountForPhantoms;
}


void hkpWorld::blockExecutingPendingOperations(hkBool block)
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	HK_ASSERT2(0xad000400, m_blockExecutingPendingOperations ^ block, "Internal Error: blocking/unblocking executing pending operations performed multiple times...");
	m_blockExecutingPendingOperations = block;
}

void hkpWorld::attemptToExecutePendingOperations( bool executeBodyOperations )
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	if (/*!areCriticalOperationsLocked()*/ !m_criticalOperationsLockCount && !m_blockExecutingPendingOperations )
	{
		if ( m_pendingOperationsCount )
	    {
		    internal_executePendingOperations();
	    }
		// only perform execution of body operations after the criticalOperation *recursive* queue is cleared completely !!
		if (m_pendingOperationQueueCount == 1 && executeBodyOperations )
		{
			if ( m_pendingBodyOperationsCount )
			{
				internal_executePendingBodyOperations();
			}
		}
	}
}

void hkpWorld::unlockAndAttemptToExecutePendingOperations()
{
	unlockCriticalOperations();
	attemptToExecutePendingOperations();
}

//
// Debugging utility: monitoring of critical operations executions
//
void hkpWorld::allowCriticalOperations(hkBool allow)
{
	HK_ACCESS_CHECK_OBJECT( this, HK_ACCESS_RW );
	HK_ASSERT2(0xad000201, m_criticalOperationsAllowed ^ allow, "Internal Error: disabled/enabled critical operations multiple times...");
	m_criticalOperationsAllowed = allow;
}

#if !defined(HK_DEBUG_MULTI_THREADING)

inline void hkpWorld::markForRead( ) const
{
}

inline void hkpWorld::markForWrite( )
{
}

inline void hkpWorld::unmarkForRead( ) const
{
}

inline void hkpWorld::unmarkForWrite()
{
}

#endif

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
