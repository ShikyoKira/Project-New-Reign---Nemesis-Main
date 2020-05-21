/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE_CONVEX_RADIUS_BUILDER_H
#define HK_COLLIDE_CONVEX_RADIUS_BUILDER_H

#include <Common/Base/hkBase.h>
#include <Common/Visualize/hkDisplayGeometryBuilder.h>

#include <Physics/Collide/Shape/hkpShapeType.h>

class hkpCollidable;
class hkDisplayGeometry;
class hkpShape;
class hkpCylinderShape;
class hkpBoxShape;
class hkpTriangleShape;
class hkpConvexVerticesShape;
class hkpConvexPieceShape;
class hkpShapeContinueData;

	/// A utility class that creates hkDisplayGeometrys from hkCollidables and hkShapes.
	/// This class is used by the hkpConvexRadiusViewer and automatically expands any object
	/// with convex radius by that amount. Any object that does not use a convex radius
	/// will be ignored and if the radius is below m_minVisibleRadius it will be ignored too.
class hkpConvexRadiusBuilder: public hkDisplayGeometryBuilder
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		struct hkpConvexRadiusBuilderEnvironment
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpConvexRadiusBuilder::hkpConvexRadiusBuilderEnvironment );

			hkpConvexRadiusBuilderEnvironment();
			float m_minVisibleRadius; // below this no display is created (== the normal body display anyway so not much use)
		};

		hkpConvexRadiusBuilder(const hkpConvexRadiusBuilderEnvironment& env);

			/// Creates an array of display geometries from a given collidable.
		void buildDisplayGeometries(		const hkpShape* shape, 
											hkArray<hkDisplayGeometry*>& displayGeometries);

			/// Creates an array of display geometries from a given shape and a transform.
		void buildShapeDisplay(				const hkpShape* shape, 
											const hkTransform& transform, 
											hkArray<hkDisplayGeometry*>& displayGeometries);

			/// Clears the current raw geometry. A temporary geometry is used to
			/// create display geometries triangle by triangle from a MOPP for example.
		void resetCurrentRawGeometry();

		// Hack: internal method used by Destruction in the Preview Tool. Search for [0xaf5511e1] or see COM-1312 for more details.
		void setMinimumVisibleRadius(hkReal radius) { m_environment.m_minVisibleRadius = radius; }

	public:
		// hkDisplayGeometryBuilder interface.
		virtual void buildDisplayGeometries( const hkReferencedObject* source, hkArray<hkDisplayGeometry*>& displayGeometries );
		virtual hkBool buildPartialDisplayGeometries( const hkReferencedObject* source, int& numSimpleShapes, hkReferencedObject* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );
		virtual hkReferencedObject* getInitialContinueData( const hkReferencedObject* source );

	public:
		// Convex shapes
		void buildShapeDisplay_Cylinder( const hkpCylinderShape* s, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Box( const hkpBoxShape* boxShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_Triangle( const hkpTriangleShape* triangleShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );
		void buildShapeDisplay_ConvexVertices( const hkpConvexVerticesShape* convexVerticesShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Compound shapes
		void buildShapeDisplay_ShapeContainer( const hkpShapeContainer* shapeContainer, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// User shapes
		hkBool buildShapeDisplay_UserShapes( const hkpShape* shape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

		// Deprecated
		void buildShapeDisplay_ConvexPiece( const hkpConvexPieceShape* triangulatedConvexShape, const hkTransform& transform, hkArray<hkDisplayGeometry*>& displayGeometries );

	protected:

		hkDisplayGeometry* getCurrentRawGeometry(hkArray<hkDisplayGeometry*>& displayGeometries);

	public:
		//
		// Partial building:
		//

		hkBool buildPartialShapeDisplay( const hkpShape* shape, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );
		hkBool buildPartialShapeDisplay_ShapeContainer( const hkpShapeContainer* shapeContainer, const hkTransform& transform, int branchDepth, int& numSimpleShapes, hkpShapeContinueData* continueData, hkArray<hkDisplayGeometry*>& displayGeometries );

	protected:

		hkpConvexRadiusBuilderEnvironment m_environment;
		hkDisplayGeometry* m_currentGeometry;


};

class hkpUserConvexRadiusBuilder : public hkReferencedObject, public hkSingleton< hkpUserConvexRadiusBuilder >
{
	//+vtable(true)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpUserConvexRadiusBuilder() {}

			/// A function to build display geometries for user shapes. The function is expected to add hkDisplayGeometry objects
			/// to the displayGeometries list. It may call back the hkpConvexRadiusBuilder::buildDisplayGeometries() method on the
			/// builder object passed in to achieve this.
		typedef void (HK_CALL *ConvexRadiusBuilderFunction)(	const hkpShape* shape,
														const hkTransform& transform,
														hkArray<hkDisplayGeometry*>& displayGeometries,
														hkpConvexRadiusBuilder* builder );

			/// You can register functions to build display for your own user types with the shape display builder using this method
		void registerUserConvexRadiusDisplayBuilder( ConvexRadiusBuilderFunction f, hkpShapeType type );

	public:

		struct UserShapeBuilder
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VDB, hkpUserConvexRadiusBuilder::UserShapeBuilder );

			ConvexRadiusBuilderFunction f;
			hkpShapeType type;
		};
		hkArray< UserShapeBuilder > m_userConvexRadiusBuilders;


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
