/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKMATH_MATH2D_H
#define HKMATH_MATH2D_H

/// Two dimensional vector.
class hkVector2
{
	public:
		HK_DECLARE_POD_TYPE();

			/// Return value indicates on which side of the infinite line this point lies.
			/// Note that this is not the Euclidean distance unless [p0,p1] is normalized.
			/// Greater than zero, zero, less than zero indicate left of, on the line
			/// and right of the line respectively. Mnemonic - if the line segment is the
			/// positive x-axis, then distance signedness matches the y-axis.
		inline hkReal signedDistanceToLineTimesLength( const hkVector2& p0, const hkVector2& p1 ) const;

			/// Return true if this is strictly right of the infinite line p0,p1
		inline hkBool32 rightOfLine( const hkVector2& p0, const hkVector2& p1 ) const;

			/// Return true if this is strictly left of the infinite line p0,p1
		inline hkBool32 leftOfLine( const hkVector2& p0, const hkVector2& p1 ) const;

			/// Returns true if the point is inside the circumcircle of the triangle p0, p1, p2 (wound CCW)
		inline hkBool32 inCircumcircle( const hkVector2& p0, const hkVector2& p1, const hkVector2& p2, hkReal tolerance = .01f ) const;

			/// Test for exact equality.
		inline hkBool32 equals( const hkVector2& p0 ) const;

			/// Return the dot product of this and p.
		inline hkReal dot( const hkVector2& p ) const;

			/// Elementwise maximum.
		inline void setMax( const hkVector2& a, const hkVector2& b );
		
			/// Elementwise minimum.
		inline void setMin( const hkVector2& a, const hkVector2& b );
		
			/// Set both elements.
		inline void setAll( hkReal a );

			/// Load from address.
		inline void load( const hkReal* p );

			/// Set both elements.
		inline void set( hkReal a, hkReal b );

			/// Set this to the perp of a (90 degree anticlockwise rotation)
		inline void setPerp( const hkVector2& a );
		
			/// Set this to the elementwise sum of a and b.
		inline void setAdd( const hkVector2& a, const hkVector2& b );
		
			/// Set this to the elementwise product of v and r.
		inline void setMul( const hkVector2& v, hkReal r );
		
			/// Set this to the elementwise product of a and b.
		inline void setMul( const hkVector2& a, const hkVector2& b );
			
			/// Multiply each element by r.
		inline void mul( hkReal r );
			
			/// Set this to a+b*r.
		inline void setAddMul( const hkVector2& a, const hkVector2& b, hkReal r );

			/// Set this to the elementwise subtraction of a and b.
		inline void setSub( const hkVector2& a, const hkVector2& b );
		
			/// Set this to be the vector (1-t)*a + t*b
		inline void setInterpolate( const hkVector2& a, const hkVector2& b, hkReal t);

			/// Return the distance between this and p.
		inline hkReal distanceTo( const hkVector2& p ) const;

			/// Set this to be the point on the segment end0,end1 which is closest to p.
		void setClosestPointOnSegmentToPoint( const hkVector2& end0, const hkVector2& end1, const hkVector2& p );
		
			/// Set this to { p.dot3(ax), p.dot3(ay) }
		inline void setProject( hkVector4Parameter p, hkVector4Parameter ax, hkVector4Parameter ay );
			
			/// Normalize this vector.
		inline void normalize();
			
			/// Normalize and return the original length.
		inline hkReal normalizeWithLength();

			/// Length.
		inline hkReal length() const;

			/// Set both elements to zero.
		inline void setZero();

			/// Return true if this is lexicographically less than v.
			/// The x components are compared and the y component used to break a tie if any.
		inline hkBool32 lexLess( const hkVector2& v );

	public:

			/// Public access
		float x;
			/// Public access
		float y;
};

#include <Common/Base/Math/Vector/hkVector2.inl>

#endif //HKMATH_MATH2D_H

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
