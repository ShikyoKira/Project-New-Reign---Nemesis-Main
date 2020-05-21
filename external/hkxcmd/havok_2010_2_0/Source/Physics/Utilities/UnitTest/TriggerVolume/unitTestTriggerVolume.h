/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/UnitTest/hkUnitTest.h>

#include <Common/Base/hkBase.h>

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Utilities/Collide/TriggerVolume/hkpTriggerVolume.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>


class UnitTestTriggerVolume : public hkpTriggerVolume
{
public:
	UnitTestTriggerVolume( hkpRigidBody* triggerBody, int numExpectedEvents, hkpTriggerVolume::EventType* expectedEvents ) 
		: hkpTriggerVolume( triggerBody ), m_numExpectedEvents( numExpectedEvents ), m_expectedEvents( expectedEvents ), m_eventCount( 0 ) 
	{

	}

	using hkpTriggerVolume::triggerEventCallback;

	virtual void triggerEventCallback( hkpRigidBody* body, EventType type )
	{
		if ( m_eventCount < m_numExpectedEvents )
		{
			HK_TEST( m_expectedEvents[m_eventCount] == type );
		}
		++m_eventCount;
	}

	~UnitTestTriggerVolume()
	{
		HK_TEST( m_eventCount == m_numExpectedEvents );
	}

public:
	int m_numExpectedEvents;
	hkpTriggerVolume::EventType* m_expectedEvents;
	int m_eventCount;
};

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
