/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEX_TRANSLATE_SHAPE_H
#define HK_COLLIDE2_CONVEX_TRANSLATE_SHAPE_H

#include <Physics/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics/Collide/Shape/hkpShapeContainer.h>

extern const hkClass hkpConvexTranslateShapeClass;

	/// An hkpConvexTranslateShape contains an hkpShape and an additional translation for that shape.
	/// This is useful, for instance, if you
	/// want to position child shapes correctly when constructing a compound shape.
	/// The advantage of using hkpConvexTranslateShape over hkpTransformShape is that
	/// it does not require additional agents to be created, as the hkpConvexTranslateShape is
	/// a convex shape and directly works with GSK.
	/// However, if you use the hkpConvexTranslateShape wrapping an hkpBoxShape, no hkpBoxBoxAgent will be
	/// created, but the hkpGskfAgent.
	///
	/// On PlayStation(R)3 this shape's collision detection will only be allowed to move onto an SPU if
	///     -# all its children are supported on SPU
	///     -# its total size including all its children will fit into one of the SPU's shape buffers
	///
	/// If this is the case a call to getChildShape() on the SPU will DMA in the child shape from
	/// main memory (and thus cause a code execution stall). To improve this we have added
	/// support for the situation where the child shape follows this hkpConvexTranslateShape consecutively in memory.
	/// In such a case both the hkpConvexTranslateShape and its child shape are brought in in one
	/// single DMA operation. Note that this properly takes care of cascades as well, i.e., several
	/// hkConvexTranslateShapes in a row are all brought in in one go.
	/// hkpConvexTransformShape supports the same mechanism and thus works as part of such a cascade as well.
class hkpConvexTranslateShape : public hkpConvexTransformShapeBase
{
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);


			/// Constructs a new convex translate shape.
			/// Note: The hkpShapeContainer::ReferencePolicy is only used for the constructor, the destructor will still always decrement the childs reference
		HK_FORCE_INLINE hkpConvexTranslateShape(const hkpConvexShape* childShape, const hkVector4& translation, hkpShapeContainer::ReferencePolicy ref = hkpShapeContainer::REFERENCE_POLICY_INCREMENT );

		hkpConvexTranslateShape( class hkFinishLoadedObjectFlag flag ) : hkpConvexTransformShapeBase(flag) { m_type = HK_SHAPE_CONVEX_TRANSLATE; }

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

			/// Gets the child shape position in local space
		inline hkVector4& getTranslation();

			/// Gets the child shape position in local space
		inline const hkVector4& getTranslation() const;


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
		inline void initializeSpu( const hkpConvexShape* childShape, const hkVector4& translation, hkReal radius );

	protected:

		hkVector4 m_translation;
};

#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.inl>

#endif // HK_COLLIDE2_CONVEX_TRANSLATE_SHAPE_H

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
