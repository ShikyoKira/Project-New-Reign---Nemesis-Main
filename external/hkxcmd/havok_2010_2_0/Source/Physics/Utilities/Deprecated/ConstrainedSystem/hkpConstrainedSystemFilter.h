/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKCONSTRAINEDSYSTEML_FILTER
#define INC_HKCONSTRAINEDSYSTEML_FILTER

#include <Physics/Collide/Filter/hkpCollisionFilter.h>
#include <Physics/Dynamics/Constraint/hkpConstraintListener.h>

extern const hkClass hkpConstrainedSystemFilterClass;

	/// DEPRECATED. This collision filter will be removed in future releases of Havok Physics.
	///
	/// This filter disables collisions between two rigid bodies if they are connected by a constraint (other than a contact constraint);
	/// otherwise, it forwards to another (optional) filter.
	///
	/// This filter is also a world constraint listener, and it removes agents immediately
	/// upon addition of constraints. Therefore it is not necessary to call
	/// updateCollisionFilter explicitly. You will need to call updateCollisionFilter explicitly
	/// when removing a constraint and expecting collision detection to be reenabled between linked bodies.
class hkpConstrainedSystemFilter : public hkpCollisionFilter, public hkpConstraintListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Constructor
		hkpConstrainedSystemFilter (const hkpCollisionFilter* otherFilter = HK_NULL);

			// Destructor
		~hkpConstrainedSystemFilter();

			// Checks two collidables
		virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const;

		virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey bKey  ) const;

		// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const;

		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& shape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const;

		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const;


		//
		// Implementation of the hkpConstraintListener interface
		//

			// Called when a constraint is added to the world.
		virtual void constraintAddedCallback( hkpConstraintInstance* constraint );

			// Called when a constraint is removed from the world.
		virtual void constraintRemovedCallback( hkpConstraintInstance* constraint );

			// hkReferencedObject Implementation
		virtual const hkClass* getClassType() const { return &hkpConstrainedSystemFilterClass; }

	public:

		hkpConstrainedSystemFilter(hkFinishLoadedObjectFlag f) : hkpCollisionFilter(f) {}

	private:

		const hkpCollisionFilter* m_otherFilter;
};


#endif //INC_HKCONSTRAINEDSYSTEML_FILTER

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
