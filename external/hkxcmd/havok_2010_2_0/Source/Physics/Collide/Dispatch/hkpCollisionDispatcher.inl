/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkpContactMgrFactory* hkpCollisionDispatcher::getContactMgrFactory(int responseA, int responseB) const
{
	HK_ASSERT2(0x34f4a8bd,  unsigned(responseA) < HK_MAX_RESPONSE_TYPE, "Response Type A is outside [ 0 .. " << HK_MAX_RESPONSE_TYPE-1 << "]" );
	HK_ASSERT2(0x28956f94,  unsigned(responseB) < HK_MAX_RESPONSE_TYPE, "Response Type B is outside [ 0 .. " << HK_MAX_RESPONSE_TYPE-1 << "]" );
	return m_contactMgrFactory[ responseA ] [ responseB ];
}


hkpCollisionDispatcher::CreateFunc hkpCollisionDispatcher::getCollisionAgentCreationFunction( hkpShapeType typeA, hkpShapeType typeB, hkpCollisionDispatcher::IsAgentPredictive predictive ) const
{
	HK_ASSERT2(0x632d17d0,  unsigned(typeA) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x40304d2d,  unsigned(typeB) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	int idx = (predictive ? m_agent2TypesPred : m_agent2Types)[typeA][typeB];
	return m_agent2Func[idx].m_createFunc;
}


hkpCollisionDispatcher::GetPenetrationsFunc hkpCollisionDispatcher::getGetPenetrationsFunc( hkpShapeType typeA, hkpShapeType typeB ) const
{
	HK_ASSERT2(0x3bb42402,  unsigned(typeA) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x5f31c19f,  unsigned(typeB) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	int idx = m_agent2Types[typeA][typeB];
	return m_agent2Func[idx].m_getPenetrationsFunc;
}

hkpCollisionDispatcher::GetClosestPointsFunc hkpCollisionDispatcher::getGetClosestPointsFunc( hkpShapeType typeA, hkpShapeType typeB ) const
{
	HK_ASSERT2(0x18e676ad,  unsigned(typeA) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x5e9be6aa,  unsigned(typeB) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	int idx = m_agent2Types[typeA][typeB];
	return m_agent2Func[idx].m_getClosestPointFunc;
}

hkpCollisionDispatcher::LinearCastFunc hkpCollisionDispatcher::getLinearCastFunc( hkpShapeType typeA, hkpShapeType typeB ) const
{
	HK_ASSERT2(0x76474747,  unsigned(typeA) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x1921f43f,  unsigned(typeB) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	int idx = m_agent2Types[typeA][typeB];
	return m_agent2Func[idx].m_linearCastFunc;
}

hkBool hkpCollisionDispatcher::getIsFlipped( hkpShapeType typeA, hkpShapeType typeB ) const
{
	HK_ASSERT2(0x76474747,  unsigned(typeA) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x1921f43f,  unsigned(typeB) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	int idx = m_agent2Types[typeA][typeB];
	return m_agent2Func[idx].m_isFlipped;
}


hkAgent3::CreateFunc   hkpCollisionDispatcher::getAgent3CreateFunc  ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_createFunc;
}

hkAgent3::DestroyFunc  hkpCollisionDispatcher::getAgent3DestroyFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_destroyFunc;
}

hkAgent3::CleanupFunc  hkpCollisionDispatcher::getAgent3CleanupFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_cleanupFunc;
}

hkAgent3::RemovePointFunc  hkpCollisionDispatcher::getAgent3RemovePointFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_removePointFunc;
}

hkAgent3::CommitPotentialFunc  hkpCollisionDispatcher::getAgent3CommitPotentialFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_commitPotentialFunc;
}

hkAgent3::CreateZombieFunc  hkpCollisionDispatcher::getAgent3CreateZombieFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_createZombieFunc;
}

hkAgent3::ProcessFunc  hkpCollisionDispatcher::getAgent3ProcessFunc ( hkAgent3::AgentType type )
{
	HK_ASSERT2(0xad873433, type >= 0 && type < HK_MAX_AGENT3_TYPES, "Invalid agent type");
	return m_agent3Func[type ].m_processFunc;
}

hkAgent3::ProcessMidphaseFunc hkpCollisionDispatcher::getAgent3ProcessMidphaseFunc ( hkAgent3::AgentType type )
{
	HK_ASSERT2(0xad873433, type >= 0 && type < HK_MAX_AGENT3_TYPES, "Invalid agent type");
	return m_agent3Func[type ].m_processMidphaseFunc;
}

hkAgent3::ProcessPostMidphaseFunc hkpCollisionDispatcher::getAgent3ProcessPostMidphaseFunc ( hkAgent3::AgentType type )
{
	HK_ASSERT2(0xad873433, type >= 0 && type < HK_MAX_AGENT3_TYPES, "Invalid agent type");
	return m_agent3Func[type ].m_processPostMidphaseFunc;
}

hkAgent3::SepNormalFunc hkpCollisionDispatcher::getAgent3SepNormalFunc( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_sepNormalFunc;
}

hkAgent3::Symmetric     hkpCollisionDispatcher::getAgent3Symmetric( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_symmetric;
}

hkAgent3::AgentType hkpCollisionDispatcher::getAgent3Type( hkpShapeType typeA, hkpShapeType typeB, hkBool32 predictive ) const
{
	HK_ASSERT(0x614aca85, (int) typeA >= 0 &&  (int) typeA < HK_MAX_SHAPE_TYPE );
	HK_ASSERT(0x614aca85, (int) typeB >= 0 &&  (int) typeB < HK_MAX_SHAPE_TYPE );

	if ( predictive )
	{
		return m_agent3TypesPred[typeA] [ typeB ];
	}
	return m_agent3Types[typeA] [ typeB ];
}

hkAgent3::UpdateFilterFunc  hkpCollisionDispatcher::getAgent3UpdateFilterFunc ( hkAgent3::AgentType type ) const
{
	return m_agent3Func[type ].m_updateFilterFunc;
}

hkAgent3::CalcStatisticsFunc  hkpCollisionDispatcher::getAgent3CalcStatisticsFunc ( hkAgent3::AgentType type ) const
{
	return m_agent3Func[type ].m_calcStatisticsFunc;
}

hkAgent3::InvalidateTimFunc  hkpCollisionDispatcher::getAgent3InvalidateTimFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_invalidateTimFunc;
}

hkAgent3::WarpTimeFunc  hkpCollisionDispatcher::getAgent3WarpTimeFunc ( hkAgent3::AgentType type )
{
	return m_agent3Func[type ].m_warpTimeFunc;
}

hkpCollisionAgent* hkpCollisionDispatcher::getNewCollisionAgent(const hkpCdBody& collA,  const hkpCdBody& collB,
															  const hkpCollisionInput& environment, hkpContactMgr* mgr) const
{
	const hkpShapeType typeA = collA.getShape()->getType();
	const hkpShapeType typeB = collB.getShape()->getType();

	HK_ASSERT2(0x7f1a216c,  unsigned(typeA) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x6ecac429,  unsigned(typeB) < HK_MAX_SHAPE_TYPE, "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");

	//CreateFunc f = m_agentCreationTable[ typeA ] [ typeB ];
	hkpCollisionDispatcher::IsAgentPredictive predictive = static_cast<hkpCollisionDispatcher::IsAgentPredictive>((int)environment.m_createPredictiveAgents.val());
	CreateFunc f = getCollisionAgentCreationFunction(typeA, typeB, predictive);

	return f(collA, collB, environment, mgr);
}




hkBool32 hkpCollisionDispatcher::hasAlternateType( hkpShapeType type, hkpShapeType alternateType )
{
	HK_ASSERT2(0x4fa8c66f,  unsigned(type) < sizeof(m_hasAlternateType)/sizeof(m_hasAlternateType[0]), "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");
	HK_ASSERT2(0x135739e6,  unsigned(alternateType) < sizeof(m_hasAlternateType)/sizeof(m_hasAlternateType[0]), "You can only access types between [0.." << HK_MAX_SHAPE_TYPE-1 << "]");

	return ( m_hasAlternateType[type] & (1<<alternateType) );
}

hkpCollisionDispatcher::CollisionQualityIndex hkpCollisionDispatcher::getCollisionQualityIndex( hkpCollidableQualityType a, hkpCollidableQualityType b)
{
	HK_ASSERT2(0xf056aef3,  unsigned(a) < HK_COLLIDABLE_QUALITY_MAX, "You can only use types between [0.." << HK_COLLIDABLE_QUALITY_MAX-1 << "]");
	HK_ASSERT2(0xf056aef4,  unsigned(b) < HK_COLLIDABLE_QUALITY_MAX, "You can only use types between [0.." << HK_COLLIDABLE_QUALITY_MAX-1 << "]");
	return m_collisionQualityTable[a][b];
}

hkpCollisionQualityInfo* hkpCollisionDispatcher::getCollisionQualityInfo( CollisionQualityIndex index)
{
	HK_ASSERT2(0xf056aef4,  unsigned(index) < HK_MAX_COLLISION_QUALITIES, "You can only use types between [0.." << HK_MAX_COLLISION_QUALITIES-1 << "]");
	return &m_collisionQualityInfo[index];
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
