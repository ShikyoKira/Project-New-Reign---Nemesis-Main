/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HAVOK_LINEAR_PARAMETRIC_CURVE_H
#define HAVOK_LINEAR_PARAMETRIC_CURVE_H

#include <Physics/Dynamics/Constraint/Bilateral/PointToPath/hkpParametricCurve.h>

extern const hkClass hkpLinearParametricCurveClass;

/// Piecewise linear curve class
class hkpLinearParametricCurve : public hkpParametricCurve
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor.
			///
			/// Default smoothing parameter is 0.01f.
			/// Loop is "non-closed" by default.
			/// To create a closed loop, the last segment should overlap the first segment exactly.
			/// This is easily accomplished by adding the first TWO points again at the end of the path.
			/// Or, if your last point is coincident to the first point, just add the second point of your
			/// path again as the last point. After this, setting the m_closedLoop flag to true will give
			/// you the desired behavior.
		hkpLinearParametricCurve();

			/// Given a parametric value, map to a point on the curve.
		virtual void getPoint( hkReal t, hkVector4& pointOnCurve ) const;

			/// Map a parametric value and a point to the nearest point on the curve. Returns the parametric value.
		virtual hkReal getNearestPoint( hkReal t, const hkVector4& nearPoint, hkVector4& pointOnPath ) const;

			/// Return the normalized tangent at the point on the curve specified by the parametric value.
		virtual void getTangent( hkReal t, hkVector4& tangent ) const;

			/// Return the smallest parametric value that is on the curve.
		virtual hkReal getStart() const;

			/// Return the largest parametric value that is on the curve.
		virtual hkReal getEnd() const;

			/// Return physical length along the curve from the start of the curve to this parametric point.
		virtual hkReal getLengthFromStart( hkReal t ) const;

			/// Return the normalized vector that defines "up" for a path.
			/// WARNING: For this curve, we calculate a binormal as being normal to the tangent
			/// but not necessarily continuous.
			/// A far better method would be to have a binormal specified at each point and interpolate between them.
		virtual void getBinormal( hkReal t, hkVector4& up ) const;

			/// Returns true if the path is closed. i.e., the beginning is the end and the end is the beginning.
		virtual hkBool isClosedLoop() const;

			// hkpLinearParametricCurve interface

			/// Add a point to the path.
		void addPoint( const hkVector4& p );

			/// Gets the smoothing factor.
			/// Smoothing factor determines how much to smooth out seams between line segments.
		hkReal getSmoothingFactor() const;

			/// Sets the smoothing factor.
			/// Smoothing factor determines how much to smooth out seams between line segments.
			/// Smoothing factor is the % from the end points of each line segment to begin smoothing.
			/// A value of 0.025 allows for fairly sharp angles. Set it to 0 if you want the curve to be
			/// discontinuous in the first derivative.
		void setSmoothingFactor( hkReal smooth );

            /// Get an array of points to draw.
		virtual void getPointsToDraw(hkArray<hkVector4>& pathPoints) const;

			/// Closes or uncloses the loop.
			/// A closed loop needs to have the last segment overlap the first segment exactly.
		void setClosedLoop( hkBool closeLoop );

		/// Transform all the points in the curve
		virtual void transformPoints( const hkTransform& transformation );

		///Create an exact copy of the curve
		virtual hkpParametricCurve* clone();

	public:

		hkReal m_smoothingFactor;
		hkBool m_closedLoop;
		hkVector4 m_dirNotParallelToTangentAlongWholePath;

		hkArray<hkVector4> m_points;
		hkArray<hkReal> m_distance;

	public:

		hkpLinearParametricCurve(hkFinishLoadedObjectFlag f) : hkpParametricCurve(f), m_points(f), m_distance(f) {}

};

#endif

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
