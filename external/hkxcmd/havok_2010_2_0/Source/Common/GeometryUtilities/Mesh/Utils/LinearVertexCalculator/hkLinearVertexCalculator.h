/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_LINEAR_VERTEX_CALCULATOR_H
#define HK_LINEAR_VERTEX_CALCULATOR_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>


class hkLinearVertexCalculator: public hkReferencedObject
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR( hkLinearVertexCalculator);

			/// Must be called before any of the other functions are called
		virtual void start();
			/// The vertex buffer passed in is the vertex linearity testing will be performed on
		virtual void setVertexBuffer(hkMeshVertexBuffer* buffer);
			/// Sets the triangle, which other vertices will have their linearity tested against
			/// MUST be called before any isLinear tests are performed
		virtual void setRootTriangle(const hkVector4 support[3], const int indices[3]);
			/// Returns true if the vertex is linear wrt to the root triangle
		virtual hkBool isLinear(hkVector4Parameter pos, int vertexIndex);
			/// Called when testing is done - releases access to the vertex buffer if currently locked
		virtual void end();

			/// Returns true if the element is to be linear tested. Can be overloaded, for more fine control of
			/// how a parameter is linear tested.
			/// epsilonOut holds the 'epsilon' which the component will be tested against.
		virtual hkBool checkElement(const hkVertexFormat::Element& ele, hkReal& epsilonOut);

			/// Enable
		void enableElement(hkVertexFormat::ComponentUsage usage, hkReal epsilon);
			/// Disable
		void disableElement(hkVertexFormat::ComponentUsage usage) { m_epsilons[usage] = -1.0f; }
			/// Disable all
		void disableAllElements();

			/// Ctor
		hkLinearVertexCalculator();
			/// Dtor
		~hkLinearVertexCalculator();

			/// calculate the barycentric coordinates of a point projected onto a triangle.
			/// Note: result 0 and 2 are always sign correct, result 1 is calculated as 1.0f - p0 - p2, this function is not epsilon safe.
			/// (NOTE Identical to hkpCollideTriangleUtil::calcBarycentricCoordinates - but needed currently as cannot access methods in 'physics' lib)
		static void HK_CALL calcBarycentricCoordinates(hkVector4Parameter pos, hkVector4Parameter t0, hkVector4Parameter t1, hkVector4Parameter t2, hkPadSpu<hkReal> result[3]);

	protected:

		hkBool m_isStarted;
		hkVector4 m_support[3];

		hkMeshVertexBuffer* m_vertexBuffer;							///< The current vertex buffer which will be used to work out if there is linearity (HK_NULL if nothing is locked)
		hkMeshVertexBuffer::LockedVertices m_lockedVertices;		///< Holds the current locked vertices checked for linearity
		hkReal m_lockedEpsilons[hkVertexFormat::MAX_ELEMENTS];		///< The epsilon for each buffer in linear locked vertices
		hkArray<hkVector4> m_rootTriangleValues;					///< The values

		hkReal m_epsilons[hkVertexFormat::USAGE_LAST];				///< The epsilon used to test each usage type < 0 if not tested

};

#endif // HK_LINEAR_VERTEX_CALCULATOR_H

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
