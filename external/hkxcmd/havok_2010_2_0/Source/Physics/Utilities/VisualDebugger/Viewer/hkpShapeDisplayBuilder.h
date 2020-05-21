/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SHAPE_DISPLAY_BUILDER_H
#define HK_COLLIDE2_SHAPE_DISPLAY_BUILDER_H

#include <Common/Base/hkBase.h>
#include <Common/Visualize/hkDisplayGeometryBuilder.h>

#include <Physics/Collide/Shape/hkpShapeType.h>
#include <Physics/Collide/Shape/hkpShapeContainer.h>

class hkpCollidable;
class hkDisplayGeometry;
class hkpShape;
class hkpSphereShape;
class hkpCapsuleShape;
class hkpCylinderShape;
class hkpBoxShape;
class hkpTriangleShape;
class hkpConvexVerticesShape;
class hkpListShape;
class hkpMultiSphereShape;
class hkpPlaneShape;
class hkpMultiRayShape;
class hkpConvexPieceShape;
class hkpExtendedMeshShape;
class hkpStorageExtendedMeshShape;
class hkpCompressedMeshShape;
class hkpSampledHeightFieldShape;
class hkpShapeContinueData;

	/// A utility class that creates hkDisplayGeometrys from hkCollidables and hkShapes.
	/// This class is used by the hkpShapeDisplayViewer.
class hkpShapeDisplayBuilder: public hkDisplayGeometryBuilder
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		struct hkpShapeDisplayBuilderEnvironment
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment );

			hkpShapeDisplayBuilderEnvironment();
			int m_spherePhiRes;
			int m_sphereThetaRes;
		};

		hkpShapeDisplayBuilder(const hkpShapeDisplayBuilderEnvironment& env);

			/// Creates an array of display geometries from a given shape.
		void buildDisplayGeometries( const hkpShape* shape, hkArray<hkDisplayGeometry*>& displayGeometries );

			/// Convert a geometry and send it to the hkDebugDisplay
		static void HK_CALL addObjectToDebugDisplay( const hkpShape* shape, hkTransform& t, hkUlong id );

	public:
			/// Creates an array of display geometries from a given shape and a transform.
		void buildShapeDisplay(	const hkpShape* shape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

			/// Clears the current raw geometry. A temporary geometry is used to
			/// create display geometries triangle by triangle from a MOPP for example.
		void resetCurrentRawGeometry();

	public:

		// hkDisplayGeometryBuilder interface.
		virtual void buildDisplayGeometries( const hkReferencedObject* source, hkArray<hkDisplayGeometry*>& displayGeometries );
		virtual hkBool buildPartialDisplayGeometries( const hkReferencedObject* source, int& numSimpleShapes, hkReferencedObject* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );
		virtual hkReferencedObject* getInitialContinueData( const hkReferencedObject* source );

	public:
		// Convex shapes
		void buildShapeDisplay_Sphere( const hkpSphereShape* sphereShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Capsule( const hkpCapsuleShape* s, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Cylinder( const hkpCylinderShape* s, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Box( const hkpBoxShape* boxShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Triangle( const hkpTriangleShape* triangleShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_ConvexVertices( const hkpConvexVerticesShape* shape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Compound shapes
		void buildShapeDisplay_ListShape( const hkpListShape* listShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_CompressedMeshShape( const hkpCompressedMeshShape* compressedMeshShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_ExtendedMeshShape( const hkpExtendedMeshShape* extendedMeshShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_SampledHeightField( const hkpSampledHeightFieldShape* heightField, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_ShapeContainer( const hkpShapeContainer* shapeContainer, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Other
		void buildShapeDisplay_MultiSphere( const hkpMultiSphereShape* s, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Plane( const hkpPlaneShape* planeShape, const hkTransform& tranform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_MultiRay( const hkpMultiRayShape* s, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// User shapes
			/// \return True if the shape was handled by a user shape display builder.
		hkBool buildShapeDisplay_UserShapes( const hkpShape* shape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Deprecated
		void buildShapeDisplay_ConvexPiece( const hkpConvexPieceShape* triangulatedConvexShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
	protected:
			/// Build the display geometry from an extended mesh shape with external storage.
		void buildShapeDisplayTriangleSubPartsCompress( const hkpExtendedMeshShape* extendedMeshShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

			/// Build the display geometry from a storage extended mesh shape.
		void buildShapeDisplayTriangleSubPartsStorage( const hkpStorageExtendedMeshShape* extendedMeshShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

			///  Build the display geometry for the shape subparts of an extended mesh shape.
		void buildShapeDisplayShapeSubParts( const hkpExtendedMeshShape* extendedMeshShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
//	protected:

public:
		hkDisplayGeometry* getCurrentRawGeometry( hkArray<hkDisplayGeometry*>& displayGeometries );

	public:
		//
		// Partial building:
		//

		hkBool buildPartialShapeDisplay( const hkpShape* shape, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Partial for compound shapes
		hkBool buildPartialShapeDisplay_ShapeContainer( const hkpShapeContainer* shapeContainer, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Partial for other shapes
		hkBool buildPartialShapeDisplay_MultiSphere( const hkpMultiSphereShape* shape, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );
		hkBool buildPartialShapeDisplay_SampledHeightField( const hkpSampledHeightFieldShape* heightField, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );
		hkBool buildPartialShapeDisplay_MultiRay( const hkpMultiRayShape* s, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );

protected:

		hkpShapeDisplayBuilderEnvironment m_environment;
		hkDisplayGeometry* m_currentGeometry;
};

class hkpUserShapeDisplayBuilder : public hkReferencedObject, public hkSingleton< hkpUserShapeDisplayBuilder >
{
	//+vtable(true)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpUserShapeDisplayBuilder() {}

			/// A function to build display geometries for user shapes. The function is expected to add hkDisplayGeometry objects
			/// to the displayGeometries list. It may call back the hkpShapeDisplayBuilder::buildDisplayGeometries() method on the
			/// builder object passed in to achieve this.
		typedef void (HK_CALL *ShapeBuilderFunction)(	const hkpShape* shape,
														const hkTransform& transform,
														hkArray<hkDisplayGeometry*>& displayGeometries,
														hkpShapeDisplayBuilder* builder );

			/// You can register functions to build display for your own user types with the shape display builder using this method
		void registerUserShapeDisplayBuilder( ShapeBuilderFunction f, hkpShapeType type );

	public:

		struct UserShapeBuilder
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpUserShapeDisplayBuilder::UserShapeBuilder );

			ShapeBuilderFunction f;
			hkpShapeType type;
		};
		hkArray< UserShapeBuilder > m_userShapeBuilders;
};


#endif // HK_COLLIDE2_SHAPE_DISPLAY_BUILDER_H

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
