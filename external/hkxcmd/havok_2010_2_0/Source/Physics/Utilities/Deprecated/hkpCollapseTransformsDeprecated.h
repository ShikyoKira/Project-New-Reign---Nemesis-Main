/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_UTILITIES_COLLAPSE_TRANSFORMS_DEPRECATED_H
#define INC_UTILITIES_COLLAPSE_TRANSFORMS_DEPRECATED_H

#include <Common/Base/hkBase.h>

class hkpRigidBody;
class hkpTransformShape;
class hkpConvexTranslateShape;
class hkpShape;

/// [NOTE: This class is deprecated and left only for compatibility - check the hkpTransformCollapseUtil utility for more advanced functionality].
/// This utility class provides static methods that collapse transform shapes into their child shape, reducing the overhead
/// caused by having extra shapes in the hierarchy.
class hkpCollapseTransformsDeprecated
{
	public:
			/// Collapses as many transforms shapes as possible into the child shapes, therefore keeping the local pivot
			/// of the rigid body intact. Returns HK_SUCCESS if there was any collapsing, HK_FAILURE if none could be done
		static hkResult HK_CALL collapseAllTransformShapes (hkpRigidBody* rigidBody);

			/// Given a transform shape, it tries to collapse the transform into its child shape (possibly creating a new shape)
			/// The resulting shape is returned. Notice the resulting shape will have an increased reference count (or 1 if its newly created)
			/// No references are removed from the original transform shape either.
		static const hkpShape* HK_CALL collapseTransformShape (const hkpTransformShape* transformShape);

			/// Ditto, but for a given transform
		static const hkpShape* HK_CALL collapseTransformShape(const hkTransform& parentFromChild, const hkpShape* childShape);

			/// Ditto, but for a convex translate shape
		static const hkpShape* HK_CALL collapseConvexTranslate(const hkpConvexTranslateShape* tls);

};


#endif //INC_UTILITIES_COLLAPSE_TRANSFORMS_DEPRECATED_H

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
