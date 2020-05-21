/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLIDE_TRIANGLEUTIL_H
#define HK_COLLIDE2_COLLIDE_TRIANGLEUTIL_H


	/// This class gives a description of a collision point in terms of the triangle's feature points.
	/// This can be used to weld the contact point normal.
struct hkpFeatureOutput
{
	typedef hkUint16 VertexId;

		/// The number of features involved.
	hkUint8 numFeatures;
		/// The featureIds of the triangle's vertices involved.
	VertexId featureIds[3];
};



/// Triangle utilities, including ray intersection, normal calculation, and point-in-triangle tests.
/// These methods are used internally by the engine. You should use the hkpShape, and hkpCollisionAgent interfaces instead
class hkpCollideTriangleUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpCollideTriangleUtil );
		/// calculate the barycentric coordinates of a point projected onto a triangle.
		/// Note: result 0 and 2 are always sign correct, result 1 is calculated as 1.0f - p0 - p2, this function is not epsilon safe.
		static void HK_CALL calcBarycentricCoordinates(hkVector4Parameter pos, hkVector4Parameter t0, hkVector4Parameter t1, hkVector4Parameter t2, hkPadSpu<hkReal> result[3]);


		//
		//	POINT TRIANGLE 
		//

		/// Fast way to repeatedly get the closest point between a triangle and a vertex
		struct ClosestPointTriangleCache
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpCollideTriangleUtil::ClosestPointTriangleCache );

			hkReal m_QQ;
			hkReal m_RR;
			hkReal m_QR;
			hkReal m_invTriNormal;
		};

		struct ClosestPointTriangleResult
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpCollideTriangleUtil::ClosestPointTriangleResult );

			hkVector4			hitDirection;
			hkPadSpu<hkReal>	distance;
			void calcClosestPoint( hkVector4Parameter point, hkVector4& closestPoint )
			{
				
				closestPoint.setAddMul4( point, hitDirection, hkSimdReal(distance.val()) );
			}
		};


		static void HK_CALL setupClosestPointTriangleCache( const hkVector4* triangle, ClosestPointTriangleCache& cache );

		enum ClosestPointTriangleStatus
		{
			HIT_TRIANGLE_FACE = 0,
			HIT_TRIANGLE_EDGE
		};

			/// \param featureOutput if non-null, then this contains the feature point ids which can be used for welding.
		static ClosestPointTriangleStatus HK_CALL closestPointTriangle( hkVector4Parameter position, const hkVector4* tri, const ClosestPointTriangleCache& cache, ClosestPointTriangleResult& result , hkpFeatureOutput* featureOutput = HK_NULL );


		//
		//	for fast triangle distance calculation
		//
		struct PointTriangleDistanceCache
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpCollideTriangleUtil::PointTriangleDistanceCache );

			hkReal m_invEdgeLen[3];
			hkReal m_invNormalLen;
			hkReal m_normalLen;
		};

		static void HK_CALL setupPointTriangleDistanceCache( const hkVector4* triangle, PointTriangleDistanceCache& cache );


			/// Calculate the distances to the triangle planes (edge0 = x ... edge2 = z, dist to triangle = w
		static void HK_CALL calcTrianglePlaneDirections( const hkVector4* tri, const PointTriangleDistanceCache& cache, hkTransform& planeEquationsOut, hkVector4& normalOut );

		//
		//	MASKS
		//

		/// returns the index of the bit set if passed in a compare mask or a index-8 of the only bit not set, works only on XYZ
		/// returns -1 for invalid input
		static const hkInt8 maskToIndex[];

		static const hkInt8 vertexToEdgeLut[];

		static inline int HK_CALL getNextModulo3( int i ) { return vertexToEdgeLut[i+2]; }
		static inline int HK_CALL getPrevModulo3( int i ) { return vertexToEdgeLut[i]; }


		// Evaluate (i+1) mod 3 with i>=0 && i<=2
		//static HK_FORCE_INLINE int getNextModulo3(int i) { return((0x09>>(i<<1))&3); }
		
		// Evaluate (i-1) mod 3 with i>=0 && i<=2
		//static HK_FORCE_INLINE int getPrevModulo3(int i) { return((0x12>>(i<<1))&3); }

	protected:

		hkpCollideTriangleUtil();

};

//#include <hkinternal/collide/util/hkpCollideTriangleUtil.inl>

#endif // HK_GEOMETRY2_TRIANGLEUTIL_H

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
