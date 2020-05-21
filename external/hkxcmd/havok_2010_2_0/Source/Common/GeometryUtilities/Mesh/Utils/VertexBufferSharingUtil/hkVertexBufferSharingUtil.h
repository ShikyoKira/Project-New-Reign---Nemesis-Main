/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VERTEX_BUFFER_SHARING_UTIL_H
#define HK_VERTEX_BUFFER_SHARING_UTIL_H

class hkMeshSystem;
class hkMeshShape;

/// Vertex buffer sharing utility
///
/// Given a array of mesh shapes, will reconstruct all of the mesh shapes, combining vertex buffers of the same format
/// into buffers of maxVertices or less. If a mesh section indexes more than maxVertices vertices, a vertex buffer will
/// be constructed of that size.
///
/// Note that the algorithm reconstructs all hkMeshShapes using the system, even if its possible it doesn't need to
///     - for example if the a section indexes all of the vertices in a vertex buffer which has more than maxVertices.
///
/// Also note that all mesh sections get translated into triangle lists - this is generally necessary as the sharing means
/// reindexing of the primitives.
///
/// The algorithm uses the actually contained vertex data as indexed by mesh sections. Vertex data doesn't have to be exactly the same, and there
/// is a threshold for what makes vertices equal. This means
///     -# If the input are already using shared vertex buffers, the output will throw out un indexed vertex indices
///     -# If there are more than one vertex buffer that contains a vertex which is 'approximately equal', they will all be potentially shared
///     -# Since there is a limit on the vertices in combined vertex buffer - a vertex which is 'approximately equal' may appear multiple times
///
/// Future updates could take into account how many vertices are shared when recombining - at the moment, the smallest are
/// recombined together, the thinking is that the small pieces are likely to be used together.
class hkVertexBufferSharingUtil
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkVertexBufferSharingUtil );

            /// Shares vertex buffers, up to max vertices and constructs new meshes that share the vertex buffers
        static void HK_CALL shareVertexBuffers(hkMeshSystem* system, const hkArray<const hkMeshShape*>& shapes, int maxVertices, hkArray<hkMeshShape*>& shapesOut);
};

#endif // HK_VERTEX_BUFFER_SHARING_UTIL_H

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
