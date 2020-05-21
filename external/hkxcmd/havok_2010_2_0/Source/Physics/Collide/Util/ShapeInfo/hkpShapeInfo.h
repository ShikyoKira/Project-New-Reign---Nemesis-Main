/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SHAPE_INFO_H
#define HK_COLLIDE2_SHAPE_INFO_H

#include <Physics/Collide/Shape/hkpShape.h>

extern const hkClass hkpShapeInfoClass;

	/// helper class to store a shape in the filter pipeline.
	/// It is used, when a shape is exported without an owning rigid body
class hkpShapeInfo: public hkReferencedObject
{
	public:
			HK_DECLARE_REFLECTION();

			HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);
			hkpShapeInfo(){ m_transform.setIdentity(); m_isHierarchicalCompound = false; }
			hkpShapeInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag), m_shape(flag), m_childShapeNames(flag), m_childTransforms(flag) { }

			virtual ~hkpShapeInfo() {}

	public:
			hkRefPtr<const hkpShape> m_shape;	///< the shape without an owning rigid body
			hkBool m_isHierarchicalCompound;	///< set to true, if the compound shape is an physics hierarchical compound shape
			hkBool m_hkdShapesCollected;		///< set to true, if the compound shape includes only children which have an hkdShape attribute group
			hkArray<hkStringPtr> m_childShapeNames;	///< the names of the HKX nodes, which hold a graphics which represents this shape
			hkArray<hkTransform> m_childTransforms;	///< the transforms child shapes, which are needed to display the child transforms at the correct place
			hkTransform m_transform;
			
};


#endif // HK_COLLIDE2_SHAPE_INFO_H

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
