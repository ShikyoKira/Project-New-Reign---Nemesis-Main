/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>

#ifndef HK_CPU_KD_TREE_RAYCAST_JOB_H
#define HK_CPU_KD_TREE_RAYCAST_JOB_H

#include <Common/Internal/KdTree/QueryUtils/hkKdTreeCastCollector.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayBundleCastInput.h>


class hkpCollisionFilter;
struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
struct hkpWorldRayCastCommand;

struct hkpProcessCollisionInput;

struct hkKdTreeRayCastBundleCommand;


class hkKdTreeCpuBroadphaseBundleCastCollector : public hkKdTreeBundleCastCollector
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkKdTreeCpuBroadphaseBundleCastCollector );
	hkKdTreeCpuBroadphaseBundleCastCollector();

	virtual	void addMidPhaseHit( hkPrimitiveId primId, hkVector4ComparisonParameter mask );


	//virtual hkBool hasHit() {return m_hit; }
	virtual void getHitFractions(hkVector4& hitFractions) const;

	const hkpCollisionFilter*		m_filter;

	// command-dependent data
	//hkReal						m_hitFraction;
	//hkBool						m_hit;

	const hkpWorldRayCastInput*	m_originalInput;
	//hkpWorldRayCastOutputPpu*	m_result;
	//hkpWorldRayCastOutputPpu*	m_nextFreeResult;
	hkKdTreeRayCastBundleCommand*		m_command;
	hkpShapeRayBundleCastInput	m_workInput;
	int							m_numHits[4];
};

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
