/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_RAY_SHAPE_COLLECTION_FILTER
#define HK_RAY_SHAPE_COLLECTION_FILTER

struct hkpShapeRayCastInput;
class hkpShapeCollection;
class hkpShape;

extern const hkClass hkpRayShapeCollectionFilterClass;

	/// This filter is called for every new child-shape of a hkpShapeCollection to be considered for ray-casting against a given ray.
class hkpRayShapeCollectionFilter
{
	//+hk.MemoryTracker(ignore=True)
	public:
		HK_DECLARE_REFLECTION();
        
			/// Return true if you want the child shape to be considered for ray-casting
			/// Return false if the ray should not hit the child shape
			/// 
			/// "aInput" is the initial input for the ray cast
			/// "bCollection" is the shape collection
			/// "bKey" is the key for the child shape being considered for filtering.
			/// You can get the child shape itself by calling bCollection->getChildShape( bKey, buffer );
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& bShape, const HK_SHAPE_CONTAINER& hContainer, hkpShapeKey bKey ) const = 0;


			/// Virtual destructor for derived objects
		virtual ~hkpRayShapeCollectionFilter() { }

};

#endif //HK_RAY_SHAPE_COLLECTION_FILTER

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
