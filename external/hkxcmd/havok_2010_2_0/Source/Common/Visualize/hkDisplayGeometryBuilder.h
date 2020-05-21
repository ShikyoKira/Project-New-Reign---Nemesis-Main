/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DISPLAY_GEOMETRY_BUILDER
#define HK_DISPLAY_GEOMETRY_BUILDER

class hkDisplayGeometry;
#include <Common/Base/Container/Array/hkArray.h>

/// An interface converting objects which can be visualized as display geometries.
class hkDisplayGeometryBuilder : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

			/// Creates an array of display geometries from a given source object.
			/// \param source the source object from which the geometries are to be built.
			/// \param displayGeometries the output geometries.
		virtual void buildDisplayGeometries( const hkReferencedObject* source, hkArray<hkDisplayGeometry*>& displayGeometries ) = 0;

			/// Build some geometry data, but "yield" when a certain amount has been built.
			/// The geometries built by this process are likely inefficient, since vertex
			/// data cannot be shared between triangles. The default implementation builds
			/// the whole geometry and decrements numSimpleShapes by 1.
			///
			/// \param source the source object from which the geometries are to be built.
			/// \param numSimpleShapes an in/out parameter stating the maximum number of "simple"
			///                        (typically convex, but implementation dependent) shapes which should be
			///                        processed before returning. It is reduced by the number of simple shapes
			///                        processed.
			/// \param continueData in/out parameter used to continue an unfinished build.
			/// \param displayGeometries the output geometries.
			/// \return True if the building has finished.
		virtual hkBool buildPartialDisplayGeometries( const hkReferencedObject* source, int& numSimpleShapes, hkReferencedObject* continueData, hkArray<hkDisplayGeometry*>& displayGeometries ) 
		{
			HK_ASSERT2( 0x3e421e73, numSimpleShapes > 0, "Cannot build a shape display for 0 numSimpleShapes." );
			buildDisplayGeometries( source, displayGeometries );
			--numSimpleShapes;
			return true;
		}

			/// Get the continue data to use when the process hasn't started yet.
			/// The continueData will have a single reference which should be automatically cleaned
			/// up on the final buildPartialDisplayGeometries.
			/// \param source the source object from which the geometries are to be built.
			/// \param continueData in/out parameter used to continue an unfinished build.
		virtual hkReferencedObject* getInitialContinueData( const hkReferencedObject* source ) 
		{ 
			return HK_NULL;
		}
};

#endif // HK_DISPLAY_GEOMETRY_BUILDER

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
