/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEX_TRANSFORM_SHAPE_H
#define HK_COLLIDE2_CONVEX_TRANSFORM_SHAPE_H

#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>

extern const hkClass hkpConvexTransformShapeClass;

	/// An hkpConvexTransformShape contains an hkpShape and an additional transform for that shape.
	/// This is useful, for instance, if you
	/// want to position child shapes correctly when constructing a compound shape.
	/// The advantage of using hkpConvexTransformShape over hkpTransformShape is that
	/// it does not require additional agents to be created, as the hkpConvexTransformShape is
	/// a convex shape and directly works with GSK.
	/// However, if you use the hkpConvexTransformShape wrapping an hkpBoxShape, no hkpBoxBoxAgent will be
	/// created, but the hkpGskfAgent.
	///
	/// For detailed information on SPU handling for this shape, see hkpConvexTranslateShape.h
class hkpConvexTransformShape : public hkpConvexTransformShapeBase
{
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

			/// Constructs a new convex transform shape.
			/// The hkpShapeContainer::ReferencePolicy is only used for the constructor, the destructor will still always decrement the childs reference
		hkpConvexTransformShape(const hkpConvexShape* childShape, const hkTransform& transform, hkpShapeContainer::ReferencePolicy ref = hkpShapeContainer::REFERENCE_POLICY_INCREMENT);

		hkpConvexTransformShape( class hkFinishLoadedObjectFlag flag ) : hkpConvexTransformShapeBase(flag) { m_type = HK_SHAPE_CONVEX_TRANSFORM; }

			/// Sets the current transform.
			/// Don't do this once the shape is added to a world
			/// as the agents may have cached data dependent on it.
		void setTransform( const hkTransform& transform );


		//
		// hkpConvexShape implementation
		//


			// hkpConvexShape::getSupportingVertex() interface implementation.
		HKP_SHAPE_VIRTUAL void getSupportingVertexImpl( HKP_SHAPE_VIRTUAL_THIS hkVector4Parameter direction, hkpCdVertex& supportingVertexOut ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape interface implementation.
		HKP_SHAPE_VIRTUAL void convertVertexIdsToVerticesImpl( HKP_SHAPE_VIRTUAL_THIS const hkpVertexId* ids, int numIds, hkpCdVertex* verticesOut) HKP_SHAPE_VIRTUAL_CONST;

			// hkpConvexShape interface implementation.
		HKP_SHAPE_VIRTUAL void getCentreImpl( HKP_SHAPE_VIRTUAL_THIS hkVector4& centreOut ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// hkpSphereRepShape implementation
		//

			// hkpSphereRepShape interface implementation.
		HKP_SHAPE_VIRTUAL int getNumCollisionSpheresImpl( HKP_SHAPE_VIRTUAL_THIS2 ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpSphereRepShape interface implementation.
		HKP_SHAPE_VIRTUAL const hkSphere* getCollisionSpheresImpl( HKP_SHAPE_VIRTUAL_THIS hkSphere* sphereBuffer ) HKP_SHAPE_VIRTUAL_CONST;

		//
		// hkpShape implementation
		//
			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

			//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) HKP_SHAPE_VIRTUAL_CONST;

			//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void castRayWithCollectorImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) HKP_SHAPE_VIRTUAL_CONST;


			// hkpConvexShape interface implementation.
		virtual void getFirstVertex(hkVector4& v) const;



			/// Get the child shape.
		inline const hkpConvexShape* getChildShape() const;

			/// Gets the transform from the child shape's space to this transform shape's local space.
		inline hkTransform& getTransform();

			/// Gets the transform from the child shape's space to this transform shape's local space.
		inline const hkTransform& getTransform() const;


		//
		// hkpShape Implementation
		//

			//	hkpShape interface implementation.
		virtual hkReal getMaximumProjection( const hkVector4& direction ) const;


			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerSimulationFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerCollideFunctions_Narrowphase )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerCollideQueryFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerRayCastFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerGetAabbFunction )( ShapeFuncs& sf );

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

		virtual const hkpShapeContainer* getContainer() const;

		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;

			// For internal use only
		inline void initializeSpu( const hkpConvexShape* childShape, const hkTransform& transform, hkReal radius );

	protected:

		hkTransform m_transform;
};

#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.inl>

#endif // HK_COLLIDE2_CONVEX_TRANSFORM_SHAPE_H

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
