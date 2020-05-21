/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ENTITY_AABB_UTIL_H
#define HK_ENTITY_AABB_UTIL_H


#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>


struct hkpCollisionInput;
class hkpEntity;


class hkValueIndexPair
{
public:
	hkUint32 m_value;
	hkUint32 m_index;

	HK_FORCE_INLINE bool operator< (const hkValueIndexPair& p1) const { return (m_value < p1.m_value); }
};


class hkpEntityAabbUtil
{
	public:
		static void HK_CALL entityBatchRecalcAabb(const hkpCollisionInput* collisionInput, hkpEntity*const* entityBatch, int numEntities);


		static void HK_CALL entityBatchInvalidateAabb(hkpEntity*const* entityBatch, int numEntities);

};


#endif // HK_ENTITY_AABB_UTIL_H

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
