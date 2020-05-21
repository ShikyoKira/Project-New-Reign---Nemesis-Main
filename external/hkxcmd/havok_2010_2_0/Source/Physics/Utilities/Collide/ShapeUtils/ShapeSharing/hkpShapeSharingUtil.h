/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_UTILITIES_SHAPE_SHARING_UTIL_H
#define INC_UTILITIES_SHAPE_SHARING_UTIL_H

#include <Common/Base/hkBase.h>

class hkpRigidBody;
class hkpShape;

/// This utility class will detect identical shape structures in one or multiple rigid bodies and will change them
/// so shapes are shared. It replaces "cloned" shapes or shapes structures with "instanced" ones.
class hkpShapeSharingUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpShapeSharingUtil );

		/*
		** SHAPE SHARING
		*/

			/// These options are used during shape comparison to decided whether two shape are equal or not.
		struct Options
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpShapeSharingUtil::Options );

				/// Used as a threshold for comparing float and vector values
				/// Defaults to 0.
				/// For a sphere shape this value applies to the radius of 2 spheres
				/// For a box shape this value applies to the extents
			float m_equalityThreshold;

				/// If true (default), the utility will check for permutations of vertices/triangles/planes when comparing hkpSimpleMeshShape or
				/// hkpConvexVerticesShape objects. This will possibly detect more instanced shapes but due to the increased amount of comparisons
				/// it will take longer time. When the option is off, vertices, triangles and planes are compared using the same order.
			hkBool m_detectPermutedComponents;

				/// Constructor; sets default values.
			Options();
		};

			/// Information regarding the results of the algorithm.
		struct Results
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpShapeSharingUtil::Results );

				/// Number of shapes originally duplicated and now reused.
				/// This is equal to the number of shapes that have been dereferenced by the shareShapes() call
			int m_numSharedShapes;

				/// Constructor; resets values.
			Results();
				
				/// Using the HK_REPORT macro, it reports these results
			void report();

				/// Initializes the results to 0.
			void reset();
		};

			/// Given a set of rigid bodies, it will try to change their shape hierarchies so cloned shapes (identical shapes) are
			/// shared (instanced) rather than duplicated.
		static hkResult HK_CALL shareShapes (hkArray<hkpRigidBody*>& rigidBodies, const Options& options, Results& resultsOut);

			/// Given a rigid body, it will try to change its shape hierarchy so cloned shapes (identical shapes) are
			/// shared (instanced) rather than duplicated.
		static hkResult HK_CALL shareShapes (hkpRigidBody* rigidBody, const Options& options, Results& resultsOut);


	private:

		static hkResult findIdenticalShapes (const Options& options, class _ShapeReplacementData& sharedShapeData, Results& resultsOut);

		static const hkpShape* replaceShapesRecursively (const hkpShape* shape, class _ShapeReplacementData& shapeReplacementData);
};


#endif //INC_UTILITIES_SHAPE_SHARING_UTIL_H

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
