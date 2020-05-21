/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_DRAWUTIL_H
#define HK_VISUALIZE_DRAWUTIL_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Types/Color/hkColor.h>

class hkLocalFrame;

/// This class facilitates drawing cones, arrows, planes representing ragdoll limits.
class hkDrawUtil: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TOOLS);

		hkDrawUtil(hkReal new_scale);

			/// Displays an oriented point.
		void displayOrientedPoint(const hkVector4& position,const hkRotation& rot,hkReal size,unsigned int color);

			/// Displays a point.
		void displayPoint(const hkVector4& position,hkReal size,hkUint32 color);

			/// Displays a line segment.
		void displaySegment(const hkVector4& p1, const hkVector4& p2, hkUint32 color);

			/// Displays a cone.
		void displayCone (hkReal cone_angle, const hkVector4& startPos, const hkVector4& coneAaxis, const hkVector4& perpVector, int numSegments, unsigned color);

			/// Displays a plane.
		void displayPlane (const hkVector4& startPos, const hkVector4& planeNormal, const hkVector4& vectorOnPlane, unsigned color, hkBool showNormal = true);

			/// Displays an arrow.
		void displayArrow (const hkVector4& startPos, const hkVector4& arrowDirection, const hkVector4& perpDirection, unsigned color);
        
			/// Draws a semi circle.
			/// \param center The position of the center in world space
			/// \param normal The axis about which the circle is drawn
			/// \param startPerp An orthogonal axis to the normal which defines the start of the sweep
		void drawSemiCircle(const hkVector4& center, hkVector4& normal, 
						    hkVector4& startPerp, hkReal thetaMin, hkReal thetaMax,
							hkReal radius,int numSegments, 
							unsigned int color);

			/// Gets the scale by which the visual representation of cones, arrows and planes will be scaled.
		hkReal getScale();

			/// Sets the scale by which the visual representation of cones, arrows and planes will be scaled.
		void setScale(hkReal new_scale);
	
			/// Display two planes at the given heights above the base location.
			/// The normal dictates the plane the rectangles will be parallel to, with the given length and width.
			/// A line is drawn connecting the rectangle centroids, and a label is drawn if given.
		void displayMinMaxPlaneWidget( const hkTransform& locationOfBase, const hkVector4& normal, hkReal length, hkReal width, hkReal minHeight, hkReal maxHeight, const hkStringPtr& label, hkInt32 color, int tag);

			/// Displays a graphical representation of the given hinge axis and angle range.
			/// The parent vector defines 0 radians rotation, and along with the normal defines the plane of the semicircle.
			/// The child vector is used to ensure the semicircle is displayed on the inside of the hinge
		void displayAxisAngleWidget( const hkVector4& location, const hkVector4& normal, const hkVector4& parent, const hkVector4& child, hkReal minRadians, hkReal maxRadians, hkReal radius, hkInt32 numSegments, hkInt32 color, int tag);

			/// Display local frame. If drawChildren is true all children of the local frame will be drawn. Use
			/// annotation to add an additional string after the local frame name.
		static void HK_CALL displayLocalFrame( const hkLocalFrame& localFrame, const hkTransform& worldFromModel, hkReal size, bool drawChildren = true, hkInt32 color = hkColor::WHITE, const char* annotation = HK_NULL);

		/// Input for debugDisplayGeometry
		struct DebugDisplayGeometrySettings
		{
			DebugDisplayGeometrySettings()
			{
				m_vertexColor = hkColor::WHITE;
				m_edgeColor = hkColor::BLACK;
				m_faceColor = hkColor::RED;

				m_showVertices = true;
				m_showEdges = true;
				m_showFaces = true;

				m_forceColorFacesByMaterial = false;
				m_lightFaces = false;
				m_lightDirection.set(0.0f, 0.0f, 1.0f );

				m_skipFaces = 1;

				m_cullingAabb.setEmpty();
			}

			int m_faceColor;
			int m_edgeColor;
			int m_vertexColor;

			hkBool m_showVertices;
			hkBool m_showEdges;
			hkBool m_showFaces;

			// Ignore m_faceColor and use material index to access hkColor::s_colorTable
			hkBool m_forceColorFacesByMaterial;
			hkBool m_lightFaces;
			hkVector4 m_lightDirection;

			// For very large geometries, rendering may be slow. Set skipface to n > 1 to render every nth face
			int m_skipFaces;
			// Cull triangles displayed to only those whose aabb overlaps this aabb
			hkAabb m_cullingAabb;
		};

		/// Displays a hkGeometry as HK_DISPLAY_LINEs and HK_DISPLAY_TRIANGLEs
		static void HK_CALL debugDisplayGeometry(const hkGeometry& triMesh, const DebugDisplayGeometrySettings& settings);

	protected:
		hkReal m_scale;
};

#endif // HK_VISUALIZE_DRAWUTIL_H

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
