/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEX_LIST_FILTER_H
#define HK_COLLIDE2_CONVEX_LIST_FILTER_H

#include <Common/Base/Object/hkReferencedObject.h>

extern const class hkClass hkpConvexListFilterClass;

class hkpCdBody;
struct hkpCollisionInput;

	/// This filter is used to determine how a convex list shape is treated in a collision.
	/// Please consult the user guide section on convex list shapes for details.
class hkpConvexListFilter : public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO );

			/// This specifies how a convex list shape should be treated in a particular collision.
		enum ConvexListCollisionType
		{
				/// Use the normal convex list collision detection, i.e., collide against the convex hull first, then
				/// check the children if necessary.
			TREAT_CONVEX_LIST_AS_NORMAL,

				/// Treat the convex list shape as a list, i.e., always check the children. This is only necessary for
				/// getting the best "welding" between convex list shapes and landscapes.
			TREAT_CONVEX_LIST_AS_LIST,

				/// Treat the convex list shape as a convex object.
			TREAT_CONVEX_LIST_AS_CONVEX
		};

			/// This callback is fired for all collisions with a convex list shape.
		virtual ConvexListCollisionType getConvexListCollisionType( const hkpCdBody& convexListBody, const hkpCdBody& otherBody, const hkpCollisionInput& input ) const = 0;

	public:

		hkpConvexListFilter(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag) {}

	protected:

		hkpConvexListFilter() {}
};

#endif // HK_COLLIDE2_CONVEX_LIST_FILTER_H

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
