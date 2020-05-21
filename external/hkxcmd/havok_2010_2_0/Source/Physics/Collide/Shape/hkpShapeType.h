/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SHAPE_TYPES_H
#define HK_COLLIDE2_SHAPE_TYPES_H


/// All shape types. The dispatcher has only to implement at least the types that can be used as secondary types
enum hkpShapeType
{
		//
		// Shape types from HK_SHAPE_INVALID to HK_SHAPE_MAX_ID_SPU are supported on the SPU.
		//

	HK_SHAPE_INVALID = 0,
		
		// 
		//	The abstract base shapes
		//

		//
		// Special convex shapes, which get their private agents for better performance on
		// PPU only.  All use predictive GSK agent on SPU.
		//

		/// hkpSphereShape type.
	HK_SHAPE_SPHERE,
		/// The first real shape.
	HK_FIRST_SHAPE_TYPE = HK_SHAPE_SPHERE,		
		/// hkpCylinderShape type.
	HK_SHAPE_CYLINDER, 
		/// hkpTriangleShape type.
	HK_SHAPE_TRIANGLE,
		/// hkpBoxShape type.
	HK_SHAPE_BOX,
		/// hkpCapsuleShape type.
	HK_SHAPE_CAPSULE,
		/// hkpConvexVerticesShape type.
	HK_SHAPE_CONVEX_VERTICES,

		//
		// Special shape collections that are solved on the SPU.
		//

		/// All shapes which inherit from hkpShapeCollection have this as an alternate type.
	HK_SHAPE_COLLECTION,

		/// All shapes which inherit from hkpBvTreeShape have this as an alternate type.
	HK_SHAPE_BV_TREE,

		/// hkpListShape type.
	HK_SHAPE_LIST,

		/// hkpMoppBvTreeShape type.
	HK_SHAPE_MOPP,

		/// hkpConvexTranslateShape type.
	HK_SHAPE_CONVEX_TRANSLATE,

		/// hkpConvexTransformShape type.
	HK_SHAPE_CONVEX_TRANSFORM,

		/// hkpSampledHeightFieldShape type.
	HK_SHAPE_SAMPLED_HEIGHT_FIELD,		

		/// hkpExtendedMeshShape type.
	HK_SHAPE_EXTENDED_MESH,

		/// hkpTransformShape type.
	HK_SHAPE_TRANSFORM,

		/// hkpCompressedMeshShape type.
	HK_SHAPE_COMPRESSED_MESH,

		/// hkpCompoundShape
	HK_SHAPE_COMPOUND,

		//
		// Shape types from HK_SHAPE_MAX_ID_SPU to HK_SHAPE_MAX_ID are NOT supported on the SPU.
		//

		// Last SPU support shape type
	HK_SHAPE_MAX_ID_SPU = HK_SHAPE_COMPOUND + 1,

		/// All shapes which inherit from hkpConvexShape have this as an alternate type.
	HK_SHAPE_CONVEX,
	
	//HK_SHAPE_PACKED_CONVEX_VERTICES,

		/// DEPRECATED - hkpMoppEmbeddedShape type.
	HK_SHAPE_MOPP_EMBEDDED,

		/// DEPRECATED - hkpConvexPieceShape type.
	HK_SHAPE_CONVEX_PIECE,

		//
		//	hkpShapeCollection implementations
		//
		
		/// DEPRECATED - hkpMultiSphereShape type.
	HK_SHAPE_MULTI_SPHERE,

		/// DEPRECATED - hkpConvexListShape.
	HK_SHAPE_CONVEX_LIST,

		/// A shape collection which only returns triangles as child shapes, e.g., hkpMeshShape.
	HK_SHAPE_TRIANGLE_COLLECTION,

		// 
		// Special shapes
		// 

		/// hkpMultiRayShape type.
	HK_SHAPE_MULTI_RAY,
		/// hkpHeightFieldShape type.
	HK_SHAPE_HEIGHT_FIELD,
		/// hkpSphereRepShape type.
	HK_SHAPE_SPHERE_REP, 
		/// hkpBvShape type.
	HK_SHAPE_BV,
		/// hkpPlaneShape type.
	HK_SHAPE_PLANE,

		//
		//	Single shapes which are processed by unary agents.
		//
		
		/// hkpPhantomCallbackShape type.
	HK_SHAPE_PHANTOM_CALLBACK,


		//
		//	user shapes
		//

	HK_SHAPE_USER0,
	HK_SHAPE_USER1,
	HK_SHAPE_USER2,

		///	The end of the shape type list.
	HK_SHAPE_MAX_ID,

		/// All shape flag, used by the hkpCollisionDispatcher.
	HK_SHAPE_ALL = -1

};

	/// A utility function to return a useful name for a given shape type
const char* HK_CALL hkGetShapeTypeName( hkpShapeType type );


class hkpCollisionDispatcher;

	/// Register all Havok specific shapeTypes
	/// This needs to be called at setup time to tell the dispatcher which
	/// shapes inherit from other shapes
void HK_CALL hkRegisterAlternateShapeTypes( hkpCollisionDispatcher* dis );


#endif // HK_COLLIDE2_SHAPE_TYPES_H

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
