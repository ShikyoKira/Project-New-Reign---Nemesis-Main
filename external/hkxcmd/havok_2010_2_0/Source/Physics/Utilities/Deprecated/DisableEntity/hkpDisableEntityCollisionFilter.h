/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_DISABLE_ALL_COLLISION_FILTER_H
#define HK_UTILITIES2_DISABLE_ALL_COLLISION_FILTER_H

#include <Physics/Collide/Filter/hkpCollisionFilter.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

extern const hkClass hkpDisableEntityCollisionFilterClass;

	/// This class is subclassed to implement special code for disabling or enabling collisions between objects.
	/// Note: this code is deprecated, please do not use it
class hkpDisableEntityCollisionFilter : public hkpCollisionFilter, private hkpEntityListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			///
		hkpDisableEntityCollisionFilter();

			///
		~hkpDisableEntityCollisionFilter();

			///Adds entity if the pointer is valid and not already in list and returns true, else returns false.
		hkBool addEntityToFilter( hkpEntity* new_entity);

			/// Removes the entity from the stored array if it exists and returns true, if the entity is not in the array it returns false.
		hkBool removeEntityFromFilter( hkpEntity* new_entity);

			/// hkpEntityListener implementation - removes any references to removed entities.
		virtual void entityRemovedCallback(hkpEntity* entity);

			///
		virtual void entityDeletedCallback(hkpEntity* entity);

			///
		int getNumStoredEntities() {return m_disabledEntities.getSize();}

			/// Returns true if the specified objects can collide, overrides from hkpCollisionFilter
		virtual hkBool isCollisionEnabled(const hkpCollidable& a, const hkpCollidable& b ) const;

			// hkpShapeCollectionFilter interface forwarding
		virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bCollection, hkpShapeKey bKey  ) const;

		// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const;

			// hkpRayShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& shape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const;

			// hkpRayCollidableFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const;

	public:

		hkArray<hkpEntity*> m_disabledEntities;

	public:
	
		hkpDisableEntityCollisionFilter(hkFinishLoadedObjectFlag f) : hkpCollisionFilter(f), m_disabledEntities(f) {}
};


#endif // HK_EXPORT2_DISABLE_ALL_COLLISION_FILTER_H

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
