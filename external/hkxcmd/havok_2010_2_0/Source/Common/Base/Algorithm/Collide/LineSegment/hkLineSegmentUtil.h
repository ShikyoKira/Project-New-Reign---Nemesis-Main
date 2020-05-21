/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_INTERNAL_LINE_SEGMENT_UTIL_H
#define HK_INTERNAL_LINE_SEGMENT_UTIL_H

/// Line segment utilities - find the closest point on a line segment, of between a pair of segments
class hkLineSegmentUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkLineSegmentUtil);
		//
		//	EDGE EDGE
		//

		enum 
		{		
			CLSLS_POINTA_START = 1,		
			CLSLS_POINTA_END   = 2,		
			CLSLS_POINTB_START = 4,		
			CLSLS_POINTB_END   = 8		
		};

		struct ClosestLineSegLineSegResult
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MATH, hkLineSegmentUtil::ClosestLineSegLineSegResult );

			hkVector4 m_closestPointA;
			hkVector4 m_closestAminusClosestB;
			hkPadSpu<hkReal>  m_distanceSquared;
		};

		/// Computes the closest points between two segments.
		/// Note that the second parameter is not the endpoint of the segment, but the vector from the start point to the end point.
		/// The same is true of the fourth parameter.
		/// returns (see enum): if bit is set, point can be deleted
		static int HK_CALL closestLineSegLineSeg( hkVector4Parameter A, hkVector4Parameter dA, hkVector4Parameter B, hkVector4Parameter dB, ClosestLineSegLineSegResult& result );


		//
		//	POINT EDGE
		//

		struct ClosestPointLineSegResult
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MATH, hkLineSegmentUtil::ClosestPointLineSegResult );

			hkVector4 m_pointOnEdge;
		};

		static int HK_CALL closestPointLineSeg( hkVector4Parameter A, hkVector4Parameter B0, hkVector4Parameter B1, ClosestPointLineSegResult& result );

		//
		// Infinite line / infinite line
		//

		struct ClosestPointInfLineInfLineResult
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, ClosestPointInfLineInfLineResult);
			/// The minimum distance squared
			hkPadSpu<hkReal> m_distanceSquared;

				/// The fraction along dA
			hkPadSpu<hkReal> m_fractionA;

				/// The fraction along dB
			hkPadSpu<hkReal> m_fractionB;

				/// The closest point on line A ( equal to A + m_fractionA*dA )
			hkVector4 m_closestPointA;

				/// The closest point on line B ( equal to B + m_fractionB*dB )
			hkVector4 m_closestPointB;
		};

			/// Compute the closest point between two infinite lines (each specified by a point and direction)
		static hkResult HK_CALL closestPointInfLineInfLine( const hkVector4& A, const hkVector4& dA, const hkVector4& B, const hkVector4& dB, ClosestPointInfLineInfLineResult& result);

};

#endif // HK_INTERNAL_LINE_SEGMENT_UTIL_H

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
