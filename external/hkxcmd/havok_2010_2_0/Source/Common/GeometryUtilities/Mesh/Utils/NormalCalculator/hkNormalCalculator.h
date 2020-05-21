/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_NORMAL_CALCULATOR_H
#define HK_NORMAL_CALCULATOR_H

/// A set of utilities for working out normals for triangles given by indexed vertices
class hkNormalCalculator
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkNormalCalculator );

			/// Calculates smoothing groups
			/// cosSmoothingAngle - is the cos of the maximum angle between triangle normals to allow smoothing
			///
			/// The calculateSmoothingGroups method takes a set of positions, a triangle list and an 'smoothing angle' which the angle across
			/// an edge between shared faces must be less than for there to be smoothing between the triangles. The actual smoothing
			/// factor is calculated as the dot product between the normals of two faces sharing an edge. It will therefore be 1 if the
			/// triangles are facing in the same direction, 0 if at 90 degree, -1 if at 180 degrees. Therefore the cos of the angle between
			/// the normals is the actual threshold (i.e., the value has to be greater than the specified value for there to be smoothing
			/// between the triangles).
			///
			/// The method returns the results in the two arrays, triangleIndicesOut and numTrianglesOut. TriangleIndidicesOut holds the indices
			/// of triangles (not the vertices) which are shared. The numTrianglesOut holds the number of triangles in each shared group. The
			/// groups indices are held contiguously in triangleIndicesOut. I.e., the first n indices in triangleIndicesOut belong to the first group (n being numTriangles[0])
			/// the next groups triangles indices will directly follow with the amount held in numTrianglesOut[1] and so forth.
		static void HK_CALL calculateSmoothingGroups(const hkArray<hkVector4>& positions, const hkUint16* triangleList, int numTriangles, hkReal cosSoothingAngle, hkArray<int>& triangleIndicesOut, hkArray<int>& numTrianglesOut);

			/// Calculates how to smooth the geometry
			/// cosSmoothingAngle - is the cos of the maximum angle between triangle normals to allow smoothing
			///
			/// calculatedSmoothedGeometry uses the calculateSmoothingGroups and calculateNormals methods to produce normals taking into account the smoothing angle.
			/// The method returns vertices and normals such that they are not shared where needed (because there isn't smoothing). The originalIndicesOut array holds
			/// the indices from the original positioned that the output normals and vertices out came from. This could be used for finding the texture coordinates to
			/// apply from the original geometry.
		static void HK_CALL calculateSmoothedGeometry(const hkArray<hkVector4>& positions, const hkUint16* triangleList, int numTriangles, hkReal cosSoothingAngle, hkArray<hkVector4>& verticesOut, hkArray<hkVector4>& normalsOut, hkArray<hkUint16>& triangleIndicesOut, hkArray<hkUint16>& originalIndicesOut);

			/// Normalizes all of the normals in the array normals
		static void HK_CALL normalize(hkArray<hkVector4>& normals);

			/// The calculateNormals method takes the positions of vertices, a triangle list and will return the normals at those positions
			/// (i.e., there will always be output the same amount of normals as positions). The normals produced will be calculated to
			/// produce lighting that is smooth across the surface. Vertex indices are used to identify if an edge is shared - not the
			/// position of the vertex.
		static void HK_CALL calculateNormals(const hkArray<hkVector4>& positions, const hkUint16* triangleList, int numTriangles, hkArray<hkVector4>& normals);

			/// Given triangles as in \a triangleIndices, the vertex \a positions and \a normals, compute the tangent frames in \a tangentsOut and \a binormalsOut such that the frames are aligned to the S-T directions of the \a texCoords.
		static void HK_CALL calculateTangentSpaces(const hkArray<hkVector4>& positions, const hkArray<hkVector4>& normals, const hkArray<hkVector4>& texCoords, const hkArray<hkUint16>& triangleIndices, hkArray<hkVector4>& tangentsOut, hkArray<hkVector4>& binormalsOut);
};


#endif // HK_NORMAL_CALCULATOR_H

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
