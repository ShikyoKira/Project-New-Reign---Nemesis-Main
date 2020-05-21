/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_GSK_CACHE
#define HK_COLLIDE2_GSK_CACHE

class hkpConvexShape;
class hkpTriangleShape;

	/// Details about a single contact point in a convex-convex collision manifold
class hkpGskCache
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkpGskCache );

			/// The vertex ids of object A immediately followed by vertex ids of object B.
			/// Check m_dimA and m_dimB for the number of vertices stored in this array.
			/// E.g. If object A collides with one of its vertices and hits object B at an edge,
			/// than m_vertices[0] is the vertex id of object A, m_vertices[1] and m_vertices[2]
			/// are the vertex ids of edge B and m_vertices[3] is unused.
		hkUint16  m_vertices[4];

			/// The number vertices of object A which support this contact.
			/// E.g.
			/// - If object A is a sphere: m_dimA == 1 (the center).
			/// - If object A collides with an edge: m_dimA == 2
			/// - If object A collides with a face: m_dimA == 3
		hkUint8	  m_dimA;

			/// The number vertices of object B which support this contact.
			/// See m_dimA for details.
		hkUint8	  m_dimB;

		enum MaxDim
		{
			MAX_DIM_MAX = 15
		};

			/// The total number of vertices in object A clipped to [1..3] or 15 if object A has more than 3 vertices.
		hkUint8	  m_maxDimA : 4;

			/// The total number of vertices in object B clipped to [1..3] or 15 if object B has more than 3 vertices.
		hkUint8	  m_maxDimB : 4;

		typedef hkUint8 GskFlags;

		enum GskFlagValues
		{
			// Cylinder flags
			GSK_FLAGS_SHAPE_A_IS_CYLINDER           = 0x01,
			GSK_FLAGS_SHAPE_B_IS_CYLINDER           = 0x02,
			GSK_FLAGS_REPLACE_SHAPE_A_WITH_CAPSULE  = 0x04,
			GSK_FLAGS_REPLACE_SHAPE_B_WITH_CAPSULE  = 0x08,

			GSK_FLAGS_CYLINDER_AGENT_FLAGS          = 0x0f,

			// Other flags
			GSK_FLAGS_DISABLE_CONTACT_TIMS     = 0x10,
			GSK_FLAGS_CONVEX_LIST_IN_GSK_MODE  = 0x20,
			GSK_FLAGS_PROCESS_FUNCTION_CALLED  = 0x40,

			// Be the first one to use the last bit below !!
			GSK_FLAGS_ALLOW_QUICKER_CONTACT_POINT_RECREATION    = 0x80,
		};

			/// Extra data used by the GSK-related agent3's
		hkUint8 m_gskFlags; 


		//
		//	Internal section
		//
	public:
		void init( const hkpConvexShape* shapeA, const hkpConvexShape* shapeB, const hkTransform& btoa );
		void initTriangle( const hkpConvexShape* shapeA, const hkpTriangleShape* shapeB, const hkTransform& btoa );
		inline void flip();
};

HK_CLASSALIGN16(class) hkGskCache16 : public hkpGskCache
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkGskCache16 );

		void set(const hkGskCache16& other)
		{
				*this = other;
		}
};

inline void hkpGskCache::flip()
{
	hkUint8 h;
	h = m_maxDimA; m_maxDimA = m_maxDimB; m_maxDimB = h;
	h = m_dimA; m_dimA = m_dimB; m_dimB = h;
	
	int last = m_dimA + m_dimB -1;
	hkUint16 k;
		// exchange first and last vertex, this should do it except for edge-edge cases
	k = m_vertices[0]; m_vertices[0] = m_vertices[last]; m_vertices[last] = k;
	if ( (m_dimA & m_dimB) ==2 )	// edge edge case
	{
		// 0 and 3 are swapped already, now swap 1 and 2
		k = m_vertices[1]; m_vertices[1] = m_vertices[2]; m_vertices[2] = k;
	}
}

#endif // HK_COLLIDE2_GSK_CACHE

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
