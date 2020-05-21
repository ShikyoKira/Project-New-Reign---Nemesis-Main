/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_AABB_TREE_WORLD_LINEAR_CASTER_H
#define HK_AABB_TREE_WORLD_LINEAR_CASTER_H

#include <Common/Internal/Collide/Tree/AabbTree/QueryUtils/hkAabbTreeQueryUtils.h>
#include <Physics/Collide/Shape/hkpShapeType.h>
#include <Physics/Collide/Agent/Query/hkpLinearCastCollisionInput.h>

struct hkpCollisionInput;
struct hkpLinearCastInput;
struct hkpCollisionAgentConfig;
class hkpCdPointCollector;
class hkpCollisionFilter;
class hkpCollidableCollidableFilter;
class hkpCollidable;


template<typename BOUNDING_VOLUME>
class hkpAabbTreeWorldLinearCaster : public hkAabbTreeQueryUtils<hkAabbTreeData::hkAabbTreeAabb, hkUint16>::Collector,
	public hkAabbTreeQueryUtils<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Collector
{
	public:
		hkpAabbTreeWorldLinearCaster(){}
		~hkpAabbTreeWorldLinearCaster(){}

		void linearCast( const hkAabbTree<BOUNDING_VOLUME>* tree, const hkpCollidable* collA, 
			const hkpLinearCastInput& input, const hkpCollidableCollidableFilter* filter,
			const hkpCollisionInput& collInput, hkpCollisionAgentConfig* config,
			hkpCdPointCollector& castCollector, hkpCdPointCollector* startPointCollector );

	protected:
		virtual hkReal addLeaf( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb, hkUint16>::Node* leaf );
		virtual hkReal addLeaf( const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>::Node* leaf );


		void testCollidable(const hkpCollidable* collB);

	protected:

		const hkpLinearCastInput* m_input;
		const hkpCollidableCollidableFilter* m_filter;
		hkpCdPointCollector*			 m_castCollector;
		hkpCdPointCollector*			 m_startPointCollector;
		const hkpCollidable*			 m_collidableA;
		enum hkpShapeType					 m_typeA;

		// used as a temporary storage
		hkpLinearCastCollisionInput	m_shapeInput;
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
