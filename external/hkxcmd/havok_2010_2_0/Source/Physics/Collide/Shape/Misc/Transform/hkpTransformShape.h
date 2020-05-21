/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_TRANSFORM_SHAPE_H
#define HK_COLLIDE2_TRANSFORM_SHAPE_H

#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Shape/hkpShapeContainer.h>

extern const hkClass hkpTransformShapeClass;


	/// An hkpTransformShape contains an hkpShape and an additional transform for that shape.
	/// This is useful, for instance, if you
	/// want to position child shapes correctly when constructing a compound shape.
	/// You should avoid using a transform shape as the root shape of a rigid body.
class hkpTransformShape : public hkpShape
{
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

			/// Constructs a new transform shape.
			/// This adds a reference to the child shape.
		hkpTransformShape( const hkpShape* childShape, const hkTransform& transform );

		hkpTransformShape( hkFinishLoadedObjectFlag flag ) : hkpShape(flag), m_childShape(flag) { m_type = HK_SHAPE_TRANSFORM; }

			/// Get the child shape.
		inline const hkpShape* getChildShape() const;

			/// Gets the transform from the child shape's space to this transform shape's local space.
		inline const hkTransform& getTransform() const;

			/// Gets the rotation part of the transform as a quaternion
		inline const hkQuaternion& getRotation() const;

			/// Sets the current transform.
			/// Don't do this once the shape is added to a world
			/// as the agents may have cached data dependant on it.
		void setTransform(const hkTransform& transform);

		//
		// hkpShape implementation
		//
			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

			//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) HKP_SHAPE_VIRTUAL_CONST;

			//	hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void castRayWithCollectorImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) HKP_SHAPE_VIRTUAL_CONST;

			/// Support for MOPP. hkpShape interface implementation.
		virtual hkReal getMaximumProjection( const hkVector4& direction ) const;

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;


			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerSimulationFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerCollideQueryFunctions )( ShapeFuncs& sf );

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerGetAabbFunction )( ShapeFuncs& sf );

		static void HK_CALL HK_INIT_FUNCTION( registerRayCastFunctions )( ShapeFuncs& sf );

			// Inherited.
		virtual const hkpShapeContainer* getContainer() const;

		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;

		void getChildShapeFromPpu() const;

	protected:

		class hkpSingleShapeContainer m_childShape;
			// 0 if the child shape is following this hkpConvexTransformShape consecutively in memory, the size of the child shape otherwise
		mutable int m_childShapeSize; // +nosave

		hkQuaternion m_rotation;
		hkTransform m_transform;
};

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.inl>

#endif // HK_COLLIDE2_TRANSFORM_SHAPE_H

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
