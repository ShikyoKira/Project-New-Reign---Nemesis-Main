/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

const hkTransform& hkpCdBody::getTransform() const
{
	HK_ASSERT2(0x733aae9d, m_motion, "You cannot call getTransform() on a hkpCdBody with null m_motion (eg, a hkpAabbPhantom)");
	return *static_cast<const hkTransform*>(m_motion);
}

const hkMotionState* hkpCdBody::getMotionState() const
{
	return static_cast<const hkMotionState*>(m_motion);
}

void hkpCdBody::setMotionState( const hkMotionState* state )
{
	m_motion = state;
}

void hkpCdBody::setTransform( const hkTransform* t )
{
	m_motion = t;
}


const hkpShape* hkpCdBody::getShape() const
{
	return m_shape;
}

const hkpCollidable* hkpCdBody::getRootCollidable() const
{	
	const hkpCdBody* body = this;
	while( body->m_parent )
	{
		body = body->m_parent;
	}
	return reinterpret_cast<const hkpCollidable*>(body);
}

const hkpCdBody* hkpCdBody::getParent() const
{
	return m_parent;
}

hkpCdBody::hkpCdBody( const hkpShape* shape, const hkMotionState* motionState )
{
	m_shape = shape;
	m_motion = motionState;
	m_parent = HK_NULL;
	m_shapeKey = HK_INVALID_SHAPE_KEY;
}

hkpCdBody::hkpCdBody( const hkpShape* shape, const hkTransform* t )
{
	m_shape = shape;
	m_motion = t;
	m_parent = HK_NULL;
	m_shapeKey = HK_INVALID_SHAPE_KEY;
}


hkpCdBody::hkpCdBody( const hkpCdBody* parent )
{
	m_parent = parent;
	m_motion = parent->m_motion;
}

hkpCdBody::hkpCdBody( const hkpCdBody* parent, const hkMotionState* ms )
{
	m_parent = parent;
	m_motion = ms;
}

hkpCdBody::hkpCdBody( const hkpCdBody* parent, const hkTransform* t )
{
	m_parent       = parent;
	m_motion = t;
}


hkpShapeKey hkpCdBody::getShapeKey() const
{
	return m_shapeKey;
}

hkpShapeKey hkpCdBody::getTopLevelShapeKey() const
{
	const hkpCdBody* root = this;
	const hkpCdBody* topChild = root;
	while ( root->m_parent ){ topChild = root; root = root->m_parent; }
	return topChild->m_shapeKey;
}

void hkpCdBody::setShape( const hkpShape* shape, hkpShapeKey key )
{
	m_shape = shape;
	m_shapeKey = key;
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
