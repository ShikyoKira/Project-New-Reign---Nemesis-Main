/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_GEOMETRY_H
#define HK_MATH_GEOMETRY_H


extern const class hkClass hkGeometryTriangleClass;

extern const hkClass hkGeometryClass;

#include <Common/Base/hkBase.h>

///	Stores a triangle based mesh.
struct hkGeometry
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkGeometry);
	HK_DECLARE_REFLECTION();

		/// A hkTriangle contains indices into an array of vertices and a material index.
	struct Triangle
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkGeometry::Triangle );
		HK_DECLARE_REFLECTION();
		HK_DECLARE_POD_TYPE();

		int m_a;
		int m_b;
		int m_c;
		int m_material;

		void set( int a, int b, int c, int material = -1)
		{
			m_a = a;
			m_b = b;
			m_c = c;
			m_material = material;
		}
	};

		/// Array of vertices that the triangles can index into.
	hkArray<hkVector4> m_vertices;

		/// Array of triangles. The triangles are quadruples of ints with the first 3 being indices into the m_vertices array.
	hkArray<struct Triangle> m_triangles;

		/// Constructor
	hkGeometry ()
	{

	}
		/// Copy constructor. Required since hkArray's copy constructor is not public.
	hkGeometry (const hkGeometry& other)
	{
		m_vertices = other.m_vertices;
		m_triangles = other.m_triangles;
	}

		/// Finish constructor (for internal use).
	hkGeometry (hkFinishLoadedObjectFlag f)
		: m_vertices(f),
		  m_triangles(f)
	{

	}

		/// Clear content.
	void	clear()
	{
		m_triangles.clear();
		m_vertices.clear();
	}
};

#endif // HK_MATH_GEOMETRY_H

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
