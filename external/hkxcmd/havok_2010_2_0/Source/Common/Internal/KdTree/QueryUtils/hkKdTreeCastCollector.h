/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_CAST_COLLECTOR_H
#define HK_KDTREE_CAST_COLLECTOR_H

#include <Common/Internal/KdTree/hkKdTree.h>

class hkKdTreeCastCollector
{
	//+hk.MemoryTracker(ignore=True)
public:
	virtual ~hkKdTreeCastCollector() { }

	virtual	hkBool addMidPhaseHit( hkPrimitiveId primId ) = 0;
	virtual hkBool hasHit() = 0;
	virtual hkReal getHitFraction() = 0;

#ifdef NON_VIRTUAL_TEST
	hkReal getHitFractionNonVirtual(){ return m_nonVirtualHitFraction;}

public:
	hkReal m_nonVirtualHitFraction;
#endif
};


class hkKdTreeBundleCastCollector
{
	//+hk.MemoryTracker(ignore=True)
public:
	virtual ~hkKdTreeBundleCastCollector() { }
	virtual	void addMidPhaseHit( hkPrimitiveId primId, hkVector4ComparisonParameter mask  ) = 0;
	virtual void getHitFractions( hkVector4& hitFractions) const = 0;
};


#endif // HK_KDTREE_CAST_COLLECTOR_H

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
