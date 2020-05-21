/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

inline hkpCollidable::hkpCollidable(const hkpShape* shape, const hkMotionState* ms, int type)
:	hkpCdBody(shape, ms), m_ownerOffset(0), 
		m_broadPhaseHandle(type),
		m_allowedPenetrationDepth(-1.0f)
{
	m_broadPhaseHandle.setOwner(this); // 'this' valid here.
	m_forceCollideOntoPpu = hkpCollidable::FORCE_PPU_SHAPE_UNCHECKED;
	m_shapeSizeOnSpu = 0;
#ifdef HK_DEBUG
	checkPerformance();
#endif
}

inline hkpCollidable::hkpCollidable(const hkpShape* shape, const hkTransform* t, int type)
:	hkpCdBody(shape, t) , m_ownerOffset(0),
	m_broadPhaseHandle(type),
	m_allowedPenetrationDepth(-1.0f)
{
	m_broadPhaseHandle.setOwner(this); // 'this' valid here.
	m_forceCollideOntoPpu = hkpCollidable::FORCE_PPU_SHAPE_UNCHECKED;
	m_shapeSizeOnSpu = 0;
#ifdef HK_DEBUG
	//checkPerformance();	// this will produce 10 million warnings in the filter pipeline
#endif
}

inline hkpCollidable::hkpCollidable( class hkFinishLoadedObjectFlag flag )
	: m_broadPhaseHandle(flag), m_boundingVolumeData(flag)
{
	if( flag.m_finishing )
	{
		m_broadPhaseHandle.setOwner(this);
	}	
}

inline hkpCollidable::BoundingVolumeData::BoundingVolumeData( class hkFinishLoadedObjectFlag flag )
{
	if( flag.m_finishing )
	{
		if (m_numChildShapeAabbs != 0 || m_capacityChildShapeAabbs != 0 || m_childShapeAabbs != HK_NULL || m_childShapeKeys != HK_NULL)
		{
			HK_WARN_ONCE(0xad836432, "Cached AABBs were saved in a packfile!?");
		}
		m_numChildShapeAabbs = 0;
		m_capacityChildShapeAabbs = 0;
		m_childShapeAabbs = HK_NULL;
		m_childShapeKeys = HK_NULL;
		invalidate();
	}	
}

HK_FORCE_INLINE bool hkpCollidable::BoundingVolumeData::hasAllocations() const 
{
	HK_ASSERT2(0xad808121, ((m_childShapeAabbs != HK_NULL) == (m_childShapeKeys != HK_NULL)) && ((m_childShapeAabbs != HK_NULL) == (m_capacityChildShapeAabbs != 0)), "BoundingVolumeData sub-allocations incorrect");
	return HK_NULL != m_childShapeAabbs;
}


HK_FORCE_INLINE hkpCollidable::~hkpCollidable()
{
}

HK_FORCE_INLINE void hkpCollidable::setShape(const hkpShape* shape)
{ 
	m_shape			 = shape;

	// Just duplicating the constructor behavior, checking if the new m_shape might cause
	// performance loss.
#if defined HK_DEBUG && !defined(HK_PLATFORM_HAS_SPU)
	checkPerformance();
#endif
}

HK_FORCE_INLINE const hkpShape* hkpCollidable::getShape() const
{ 
	return m_shape; 
}

// HK_FORCE_INLINE void* hkpCollidable::getOwner() const
// { 
// 	return const_cast<char*>( reinterpret_cast<const char*>(this) + m_ownerOffset ); 
// }

HK_FORCE_INLINE void hkpCollidable::setOwner( void* owner )
{
	int ownerOffset = hkGetByteOffsetInt( this, owner ); // should be within int8 range even on 64 bit as a member
	HK_ASSERT( 0xf0232323, ownerOffset > -128 && ownerOffset<127);
	m_ownerOffset = hkInt8(ownerOffset);
}

inline const hkpTypedBroadPhaseHandle* hkpCollidable::getBroadPhaseHandle() const
{
	return &m_broadPhaseHandle;
}

inline hkpTypedBroadPhaseHandle* hkpCollidable::getBroadPhaseHandle()
{
	return &m_broadPhaseHandle;
}

int hkpCollidable::getType() const
{
	return m_broadPhaseHandle.getType();
}

HK_FORCE_INLINE hkpCollidableQualityType hkpCollidable::getQualityType() const
{
	return hkpCollidableQualityType( m_broadPhaseHandle.m_objectQualityType );
}

HK_FORCE_INLINE void hkpCollidable::setQualityType(hkpCollidableQualityType type)
{
	m_broadPhaseHandle.m_objectQualityType = hkInt8(type);
}

hkUint32 hkpCollidable::getCollisionFilterInfo() const
{
	return m_broadPhaseHandle.getCollisionFilterInfo();
}

void hkpCollidable::setCollisionFilterInfo( hkUint32 info )
{
	m_broadPhaseHandle.setCollisionFilterInfo(info);
}


HK_FORCE_INLINE hkReal hkpCollidable::getAllowedPenetrationDepth() const
{
	return m_allowedPenetrationDepth;
}

HK_FORCE_INLINE void hkpCollidable::setAllowedPenetrationDepth( hkReal val )
{
	HK_ASSERT2(0xad45bd3d, val > HK_REAL_EPSILON, "Must use a non-zero ( > epsilon ) value when setting allowed penetration depth of bodies.");
	m_allowedPenetrationDepth = val;
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
