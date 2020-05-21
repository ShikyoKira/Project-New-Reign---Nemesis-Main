/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SHAPE_GENERATOR
#define HK_SHAPE_GENERATOR

#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>

class hkPseudoRandomGenerator;

///This class can be used to create random hkConvexVerticesShapes.
class hkpShapeGenerator
{
	public:
		enum Flags { NONE, NO_PLANE_EQUATIONS };

		static hkpConvexVerticesShape* HK_CALL createRandomConvexVerticesShape(	const hkVector4& minbox, 
																				const hkVector4& maxbox, 
																				int numvert, 
																				hkPseudoRandomGenerator *generator,
																				Flags flags = NONE );

		static hkpConvexVerticesShape* HK_CALL createRandomConvexVerticesShapeWithThinTriangles(	const hkVector4& minbox, 
																								const hkVector4& maxbox, 
																								int numvert, 
																								float minEdgeLen, 
																								hkPseudoRandomGenerator *generator,
																								Flags flags = NONE );

		enum ShapeType
		{
				RANDOM,
				SPHERE,
				CAPSULE,
				BOX,
				TRIANGLE,
				//THIN_TRIANGLE,
				CONVEX_VERTICES,
				CONVEX_VERTICES_BOX,
				SHAPE_MAX
		};

		static hkpConvexShape* HK_CALL createConvexShape( const hkVector4& extents, ShapeType type, hkPseudoRandomGenerator *generator );

			/// Create a convexVerticesShape which is like a box with the given
			/// halfExtents and convexRadius.
		static hkpConvexVerticesShape* HK_CALL createConvexVerticesBox( const hkVector4& halfExtents, hkReal convexRadius = hkConvexShapeDefaultRadius );

		static const char* HK_CALL getShapeTypeName( ShapeType type );
};

#endif //HK_SHAPE_GENERATOR

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
