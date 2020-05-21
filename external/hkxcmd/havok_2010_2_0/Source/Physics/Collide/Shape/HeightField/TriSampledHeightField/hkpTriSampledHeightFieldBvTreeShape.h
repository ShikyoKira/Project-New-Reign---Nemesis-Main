/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TRI_SAMPLED_HEIGHTFIELD_BVTREE_SHAPE
#define HK_TRI_SAMPLED_HEIGHTFIELD_BVTREE_SHAPE

#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>

extern const hkClass hkpTriSampledHeightFieldBvTreeShapeClass;

class hkpTriSampledHeightFieldCollection;

/// This shape must be used in conjunction with the hkpTriSampledHeightFieldCollection. Please see the documentation for
/// the hkpTriSampledHeightFieldCollection for details on what the classes do.
class hkpTriSampledHeightFieldBvTreeShape : public hkpBvTreeShape
{
	//+hk.ReflectedFile("hkpHeightField")
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// The constructor takes a pointer to hkpTriSampledHeightFieldCollection.
			/// This class is designed to work together with the hkpTriSampledHeightFieldCollection.
			/// The value "doAabbRejection" can be used to turn off the AABB rejection test.
			/// The queryAabb() method (which will be used by agents/user queries using
			/// the hkpBvTreeShape interface) uses its projection onto the heightfield grid
			/// space to determine what triangles to return.
			/// To ensure that the resulting triangles are at least close to the
			/// AABB in the direction of projection (that the AABB is not hovering completely
			/// above or below all points in that area of heightfield), this test checks that it
			/// at least overlaps the single AABB bounding all those triangles.
			/// By default it is true. Setting it to false disables the test,
			/// which may give a small speedup for flat heightfields.
		hkpTriSampledHeightFieldBvTreeShape( const hkpTriSampledHeightFieldCollection* c, hkBool doAabbRejection = true );

		hkpTriSampledHeightFieldBvTreeShape( hkFinishLoadedObjectFlag flag ) : hkpBvTreeShape(flag), m_childContainer(flag)
		{
			if( flag.m_finishing )
			{
				m_type = HK_SHAPE_BV_TREE;
				m_bvTreeType = BVTREE_TRISAMPLED_HEIGHTFIELD;
			}
		}

			/// Returns a struct of function pointers needed by the SPU
		static void HK_CALL HK_INIT_FUNCTION( registerSimulationFunctions )( ShapeFuncs& sf );
		static void HK_CALL registerCollideQueryFunctions( ShapeFuncs& sf );

		//
		// hkpBvTreeShape interface
		//

			/// Returns the hkpShapeKey for all shapes in the hkpShapeCollection that intersect with the OBB (defined by obbTransform and obbExtent).
			/// This implementation simply bounds the OBB by an AABB and uses the queryAabb() method. Thus it is not as
			/// accurate as it could be if using a more sophisticated algorithm, and can return triangles completely outside the OBB.
		virtual void queryObb( const hkTransform& obbTransform, const hkVector4& obbExtent, hkReal tolerance, hkArray< hkpShapeKey >& hits ) const;

			/// Returns the hkpShapeKey for all shapes in the hkpShapeCollection that intersect with the AABB.
		virtual void queryAabb( const hkAabb& aabb, hkArray<hkpShapeKey>& hits ) const;

			// hkpBvTreeShape interface implementation.
		HKP_SHAPE_VIRTUAL hkUint32 queryAabbImpl(HKP_SHAPE_VIRTUAL_THIS const hkAabb& aabb, hkpShapeKey* hits, int maxNumKeys ) HKP_SHAPE_VIRTUAL_CONST;
			
		//
		// hkpShape interface
		//

			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL hkBool castRayImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output) HKP_SHAPE_VIRTUAL_CONST;

			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void castRayWithCollectorImpl( HKP_SHAPE_VIRTUAL_THIS const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) HKP_SHAPE_VIRTUAL_CONST;

			// Retrieves the shape collection, DMAing from PPU if necessary
		inline const hkpTriSampledHeightFieldCollection* getShapeCollection() const;

			// hkpBvTreeShape interface implementation.
		inline const hkpShapeContainer* getContainer() const;

			// hkpShape interface implementation.
		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;

		HK_FORCE_INLINE hkBool getWantAabbRejectionTest() const { return m_wantAabbRejectionTest; }
		HK_FORCE_INLINE void   setWantAabbRejectionTest(hkBool newVal) { m_wantAabbRejectionTest = newVal; }

	protected:
		
		static hkBool HK_CALL getExtentsForQueryAabb(hkAabb& aabb, const hkpTriSampledHeightFieldCollection* collection, hkUint32& minX, hkUint32& maxX, hkUint32& minZ, hkUint32& maxZ);

		class hkpSingleShapeContainer m_childContainer;
		mutable int	m_childSize;	//+nosave
		
		hkBool m_wantAabbRejectionTest;
		
		// Need to pad to a multiple of 16 on SPU
		hkUint8 m_padding[12];
};

#include <Physics/Collide/Shape/HeightField/TriSampledHeightField/hkpTriSampledHeightFieldBvTreeShape.inl>

#endif // HK_TRI_SAMPLED_HEIGHTFIELD_BVTREE_SHAPE

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
