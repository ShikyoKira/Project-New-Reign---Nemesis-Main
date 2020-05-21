/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_COLLIDE2_SHAPE_COLLECTION_FILTER_H
#define HK_COLLIDE2_SHAPE_COLLECTION_FILTER_H

#include <Physics/Collide/Shape/hkpShape.h>

extern const class hkClass hkpShapeCollectionFilterClass;

struct hkpCollisionInput;
class hkpBvTreeShape;

	/// A filter which allows for filtering shapes against subshapes
	/// It is called when a shape encounters a shape collection. For each sub-shape of the shape collection that needs to be tested against the shape
	/// the filter is called.
class hkpShapeCollectionFilter
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_REFLECTION();

			/// See whether body A collides with a subshape of shapeCollection B
			/// "input" is the collision input used for the initial collide query
			/// "bodyA" is the cd body for the single shape
			/// "collectionBodyB" is the cd body for the bvTree shape *or* shape collection (if there's no bvTree shape)
			/// "containerShapeB" is (the container of) the shape collection of B
			/// "keyB" is the key needed to access the subshape of containerShapeB.
			/// You can get the child shape itself by calling containerShapeB->getChildShape( bKey, buffer );
			/// If you return true from this function, the system goes on to perform a collision check between "a" and the
			/// child shape of "collectionBodyB" given by "keyB".
			/// N.B. There are two possible 'gotchas' associated with implementing this method, both related to the logic
			/// of what should happen when a collection collides against another collection: Please read the comments in hkpGroupFilter
			/// very carefully to see how it deals with these problems.
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& bodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyB  ) const = 0;

			/// See whether a subshape of bodyA collides with a subshape of bodyB.
			/// This method is analogical to
			/// \code virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& bodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyB ) const; \endcode
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const = 0;

			/// This callback is triggered when a hkpBvTreeShape's AABB query returns more hkpShapeKey hits
			/// than can be fit into the default hits buffer (default: 4K).
			/// This function is meant to allow you to filter the returned keys, or perform a new aabbQuery
			/// and choose hkpShapeKeys hit from scratch.
			///
			/// \param bvTreeShapeB The MOPP BV tree shape. This shape must be the shape of bodyB.
			/// \param aabb The hkAabb used with the original AABB query.
			/// \param shapeKeysInOut Buffer containing the original keys, that should be overwritten with newly
			///                       selected keys.
			/// \param shapeKeysCapacity Number of keys in the shapeKeysInOut buffer. It is equal to the number
			///                          of hits returned by the original AABB query clipped to the buffer size.
			/// \return Number of hkpShapeKey hits to be used from the output shapeKeysInOut buffer.
		virtual int numShapeKeyHitsLimitBreached( const hkpCollisionInput& input, 
															const hkpCdBody& bodyA, const hkpCdBody& bodyB, 
															const hkpBvTreeShape* bvTreeShapeB, hkAabb& aabb,
															hkpShapeKey* shapeKeysInOut,
															int shapeKeysCapacity) const = 0;

		/// Virtual destructor for derived objects
		virtual ~hkpShapeCollectionFilter() { }
};


#endif //HK_COLLIDE2_SHAPE_COLLECTION_FILTER_H

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
