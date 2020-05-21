/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_CONVEX_VERTICES_CONNECTIVITY_H
#define HKP_CONVEX_VERTICES_CONNECTIVITY_H


extern const class hkClass hkpConvexVerticesConnectivityClass;

/// Hold connectivity information for a hkpConvexVerticesShape
/// This information is optional for the hkpConvexVerticesShape - but having it set can speed up some algorithms,
/// and other algorithms actually require the information (for example the convex hull cutting functionality)
///
/// Connectivity information must be closed.
/// The vertex index order is expected to define the way the face is facing.
class hkpConvexVerticesConnectivity : public hkReferencedObject
{
	public:
        HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);

		hkpConvexVerticesConnectivity() {}

		hkpConvexVerticesConnectivity( hkFinishLoadedObjectFlag flag ) :
			hkReferencedObject(flag), m_vertexIndices(flag), m_numVerticesPerFace(flag) {}

			/// Returns true if the connectivity is 'closed'. It is a requirement of all valid connectivity
			/// that it is closed. Loosely being closed means that any surface has no 'holes'. Algorithmically
			/// if every edge exists once - in a forward direction and a backwards direction then the surface
			/// is closed.
			/// Note this definition allows multiple closed surfaces, or vertices which are not indexed.
		hkBool isClosed() const;

			/// Remove all of the connectivity information
		virtual void clear();

			/// Add a face
		void addFace(int* indices,int numIndices);
			/// Returns the total amount of faces
		HK_FORCE_INLINE int getNumFaces() const { return m_numVerticesPerFace.getSize(); }

		// An array holding the vertex indices for all the the faces
		// Each faces indices is stored in a clockwise is outwards order
		// Faces indices are stored one after another - in the order of the faces inside
		// of the hkpConvexVerticesShape
		hkArray<hkUint16> m_vertexIndices;

		// The number of indices used by each face. Maximum number of indices (and therefore edges) per
		// face is 256.
		hkArray<hkUint8> m_numVerticesPerFace;
};

#endif // HKP_CONVEX_VERTICES_CONNECTIVITY_H

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
