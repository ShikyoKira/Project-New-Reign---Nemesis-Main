/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_SAMPLED_HEIGHT_FIELD_SHAPE_H
#define HK_COLLIDE2_SAMPLED_HEIGHT_FIELD_SHAPE_H

#include <Physics/Collide/Shape/HeightField/hkpHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Common/Base/Types/Geometry/Sphere/hkSphere.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>

extern const hkClass hkpSampledHeightFieldShapeClass;

/// This class implements some default behaviour for the hkpSampledHeightFieldShape.
/// It uses the y coordinate for the up axis
/// This is a template class for 2d sampled heightfields.
/// The x and z coordinates are used to lookup a height information
/// If you want to use this class, you need to subclass it and
/// implement the following functions:
/// \code HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const \endcode
/// Which returns the height at a given x,z
/// \code HK_FORCE_INLINE hkBool getTriangleFlipImpl() \endcode
/// This should return true if two triangles share the edge p(x,z) - p(x+1,z+1).
/// This should return false if the triangles share the edge p(x,z+1) - p(x+1,z)
/// \code collideSpheres \endcode
/// This should forward to collideSpheresImplementation. This inlines your
/// getHeightAtImpl and getTriangleFlipImpl.
///
/// Example:
/// \verbatim
/// class MySampledHeightFieldShape : public hkpSampledHeightFieldShape
/// {
///     HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const{ return x + y; }
///     HK_FORCE_INLINE hkBool getTriangleFlipImpl(){ return false; }
///     virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
///     {
///         return collideSpheresImplementation(*this, input, outputArray);
///     }
/// };
/// \endverbatim
class hkpSampledHeightFieldShape : public hkpHeightFieldShape
{
	//+hk.ReflectedFile("hkpHeightField")
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		enum HeightFieldType
		{
			/// hkpStorageSampledHeightFieldShape, stores values as hkReals
			HEIGHTFIELD_STORAGE = 0,

			/// hkpCompressedSampledHeightFieldShape, stores values as hkUint16s
			HEIGHTFIELD_COMPRESSED,

			/// All other heightfields use this type
			/// It's possible to run a custom heightfield on the SPU by setting s_heightFieldFunctions[HEIGHTFIELD_USER_SPU]
			/// to point to the appropriate functions in registerHeightFieldFunctions.
			/// This is recommended for experts only.
			HEIGHTFIELD_USER, 

			HEIGHTFIELD_MAX_ID
		};


		hkpSampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci, HeightFieldType type = HEIGHTFIELD_USER);
		virtual ~hkpSampledHeightFieldShape();

			/// Get the construction info for this current instance. Note that this info will reflect the current state of this object, not its original construction info.
		void getCinfo( hkpSampledHeightFieldBaseCinfo& cinfo ) const;

			///	hkpShape Interface
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

			/// hkpHeightFieldShape interface implementation
		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const = 0;

			/// hkpHeightFieldShape interface implementation
		virtual void castSphere( const hkpSphereCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;


			/// get the height at x,z. Note x,z are already clipped
			/// On CPU/PPU, forwards to the virtual getHeightAtImpl method
			/// On SPU, calls the static s_getHeightAtImplPtr function
		HK_FORCE_INLINE hkReal getHeightAt( int x, int z ) const;

			/// Return true if two triangles share the edge p(x,z) - p(x+1,z+1)
			/// Return false if the triangles sphere the edge p(x,z+1) - p(x+1,z);
			/// On CPU/PPU, forwards to the virtual getTriangleFlipImpl method
			/// On SPU, calls the static s_getTriangleFlipImplPtr method
		HK_FORCE_INLINE hkBool getTriangleFlip() const;

		

		virtual hkReal getHeightAtImpl( int x, int z ) const = 0;
		virtual hkBool getTriangleFlipImpl() const = 0;

			/// Returns a struct of function pointers needed by the SPU for
		static void HK_CALL HK_INIT_FUNCTION( registerSimulationFunctions )( ShapeFuncs& sf );			
		static void HK_CALL HK_INIT_FUNCTION( registerCollideQueryFunctions )( ShapeFuncs& sf );
		static void HK_CALL registerRayCastFunctions( ShapeFuncs& sf );

			/// Set up the static table for heightfield functions
		static void HK_CALL HK_INIT_FUNCTION( registerHeightFieldFunctions )();

			/// hkpShape interface implementation
			///     - output.m_extraInfo = hitX << 1 + (hitZ<<16). Lowest bit is used for triangle index.
			///
			/// Notes on the implementation:
			///     -  The ray can never tunnel through two neighbouring triangles.
			///     -  The ray is not epsilon proof when it just touches the landscape.
			///     -  The ray algorithm uses a fast walking algorithm over the landscape,
			///        that means very long rays over very big height fields cost quite some CPU
			///     -  The code is somehow fast, however quite big. Try to combine many ray queries together to minimize instruction cache hits
			///     -  Rays that are nearly vertical to the heightfield get a different optimized treatment.
			///     -  There are no restrictions on the scale, it can be anything, even negative
			///     -  If the ray starts or ends outside the heightfield, no problem, it gets clipped correctly
			///     -  the size of the heightfield is limited to 16k * 16k
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output ) HKP_SHAPE_VIRTUAL_CONST ;

			/// hkpShape interface implementation
		HKP_SHAPE_VIRTUAL void castRayWithCollectorImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) HKP_SHAPE_VIRTUAL_CONST;

	private:
		void castRayInternal( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkBool reportPenetratingStartPosition, hkReal maxExtraPenetration, hkpRayHitCollector& collector ) const;

		// implementation in .cpp - inlined several times
		HK_FORCE_INLINE void _getHeightAndNormalAt( int xPos, int zPos, hkReal subX, hkReal subZ, hkVector4& normalOut, hkReal& heightOut, int& triangleIndexOut ) const;

	public:
		void getHeightAndNormalAt( int xPos, int zPos, hkReal subX, hkReal subZ, hkVector4& normalOut, hkReal& heightOut, int& triangleIndexOut ) const;

	public:
		
		int m_xRes;
		int m_zRes;

		hkReal m_heightCenter;

			/// The method used to calculated the height. See hkpSampledHeightFieldBaseCinfo::m_useProjectionBasedHeight for details.
		hkBool m_useProjectionBasedHeight;

			/// Heightfield type. This is used to determine which getHeightAt method to call on the SPU.
		hkEnum<HeightFieldType, hkUint8> m_heightfieldType; //+default(hkpSampledHeightFieldShape::HEIGHTFIELD_USER)

		hkVector4 m_intToFloatScale;
		hkVector4 m_floatToIntScale;
		hkVector4 m_floatToIntOffsetFloorCorrected;
		hkVector4 m_extents;

	public:

		hkpSampledHeightFieldShape( ) : hkpHeightFieldShape( HK_SHAPE_SAMPLED_HEIGHT_FIELD ) {}
		hkpSampledHeightFieldShape( hkFinishLoadedObjectFlag flag ) : hkpHeightFieldShape(flag)
		{
			if (flag.m_finishing)
			{
				m_type = HK_SHAPE_SAMPLED_HEIGHT_FIELD;
			}
		}
};

// Work around vc6 member templates bug. Should be hkpSampledHeightFieldShape::collideSpheres.
template<typename IMPL>
HK_FORCE_INLINE void HK_CALL hkSampledHeightFieldShape_collideSpheres(
	const IMPL& shape,
	const hkpHeightFieldShape::CollideSpheresInput& input,
	hkpHeightFieldShape::SphereCollisionOutput* outputArray);


#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.inl>

#endif // HK_COLLIDE2_SAMPLED_HEIGHT_FIELD_SHAPE_H

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
