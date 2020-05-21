/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_COLLIDE2_COLLISION_FILTER_SET_H
#define HK_COLLIDE2_COLLISION_FILTER_SET_H

#include <Physics/Collide/Filter/hkpCollisionFilter.h>

extern const hkClass hkpCollisionFilterListClass;

	/// This class allows you to create a collision filter that is composed of multiple other collision filters.
	/// For each of the isCollisionEnabled() methods, this filter iterates through its list of filters in reverse
	/// order. If any collision filter in the list returns false, then the list filter returns false. If all
	/// collision filters in the list return true, the list filter returns true.
class hkpCollisionFilterList : public hkpCollisionFilter
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
	
		hkpCollisionFilterList();

		hkpCollisionFilterList( const hkArray<const hkpCollisionFilter*>& collisionFilters );

			/// Destructor removes all references to collision filters
		~hkpCollisionFilterList();


			/// Get the list of collision filters
		const hkArray<const hkpCollisionFilter*>& getCollisionFilters() const;

			/// Adds a new collision filter: Attention: this does not start reevaluating the collidable pairs.
			/// You have to call hkpWorld::updateCollisionFilter to do this.
		void addCollisionFilter( const hkpCollisionFilter* filter);

			/// Adds a new collision filter: Attention: this does not start reevaluating the collidable pairs
			/// You have to call hkpWorld::updateCollisionFilter to do this.
		void removeCollisionFilter( const hkpCollisionFilter* filter);

			/// If any collision filter in the filter list returns false, then this function returns false. If all
			/// collision filters in the list return true, this function returns true.
		virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const;

			/// If any collision filter in the filter list returns false, then this function returns false. If all
			/// collision filters in the list return true, this function returns true.
		virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey bKey  ) const;

		// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const;

			/// If any collision filter in the filter list returns false, then this function returns false. If all
			/// collision filters in the list return true, this function returns true.
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& bShape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const;

			/// If any collision filter in the filter list returns false, then this function returns false. If all
			/// collision filters in the list return true, this function returns true.
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& aInput, const hkpCollidable& collidableB ) const;


	protected:

		//hkInplaceArray<const hkpCollisionFilter*,3> m_collisionFilters;
		hkArray<const hkpCollisionFilter*> m_collisionFilters;

	public:

		hkpCollisionFilterList( class hkFinishLoadedObjectFlag flag ) :
			hkpCollisionFilter(flag), m_collisionFilters(flag)
		{
			if( flag.m_finishing )
			{
				m_type = HK_FILTER_LIST;
			}
		}
};

#endif // HK_COLLIDE2_COLLISION_FILTER_SET_H

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
