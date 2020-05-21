/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILS_SHAPE_SCALING_UTILITY__H
#define HK_UTILS_SHAPE_SCALING_UTILITY__H

#include <Common/Base/hkBase.h>

class hkpShape;

/// This utility class uniformly scales physics shapes.
/// Note that this is not meant to be used at runtime, but in the tool chain and preprocess stages.
class hkpShapeScalingUtility
{
public:
HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpShapeScalingUtility);
	struct ShapePair
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpShapeScalingUtility::ShapePair );

		/// Old shape.
		hkpShape* originalShape; 

		/// New shape. Null if not new.
		hkpShape* newShape;
	};
	
	/// This will try to uniformly scale the given shape by the given amount.
	/// If you are doing multiple scale calls and you have shared shapes,
	/// you might want to provide an array to persist between calls
	/// to stop the shapes from being shrunk more than once.
	/// This function is recursive.
	static hkpShape* HK_CALL scaleShape( hkpShape* shape, hkReal scale, hkArray<ShapePair>* doneShapes = HK_NULL );
};


#endif // HK_UTILS_SHAPE_SCALING_UTILITY__H

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
