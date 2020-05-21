/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_GSK_MANIFOLD_H
#define HK_COLLIDE2_GSK_MANIFOLD_H

#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>



/// This is class to hold a feature based manifold.
/// Note: the class only occupies it's first getTotalSizeInBytes.
/// 
struct hkpGskManifold
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MANIFOLD, hkpGskManifold );

	//
	//	public structures 
	//
	struct ContactPoint
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_MANIFOLD, hkpGskManifold::ContactPoint );

		hkUchar m_dimA;
		hkUchar m_dimB;
		hkContactPointId m_id;
		union
		{
			hkUchar m_vert[4];	// these are the vertex indices pointing into the shared vertex array * sizeof(hkVector4)
			hkUint32 m_allVerts;
		};
		void zeroVerts(){ m_allVerts = 0; }

	};

	typedef hkUint16 VertexId;

	enum { MAX_POINTS = 4 };
	enum { MAX_VERTICES = MAX_POINTS * 4  };



	HK_FORCE_INLINE VertexId getVertexId( const ContactPoint& cp, int vertNr ) const;

	HK_FORCE_INLINE void init();

	HK_FORCE_INLINE hkpGskManifold();

	HK_FORCE_INLINE int getTotalSizeInBytes() const;

	HK_FORCE_INLINE VertexId* getVertexIds();

	HK_FORCE_INLINE const VertexId* getVertexIds() const;


		//
		//	Members
		//

		/// Total number of vertices used of object A for all contact points
	hkUchar  m_numVertsA;
	hkUchar  m_numVertsB;
	hkUchar  m_numContactPoints;

	ContactPoint m_contactPoints[ MAX_POINTS ];
	// followed the the array of vertexIds
		// add padding so at least we can fit MAX_POINTS and MAX_VERTICES in this structure
	hkUchar m_padding[ MAX_VERTICES * sizeof(VertexId) ];

};

void hkpGskManifold::init()	
{		
	*((hkUint32*)this) = 0; 	
}

hkpGskManifold::hkpGskManifold()
{ 
	init(); 
}

int hkpGskManifold::getTotalSizeInBytes() const
{
	return 4 //  m_numVertsA + m_numVertsB + m_numContactPoints		// header
		+ m_numContactPoints           * hkSizeOf(ContactPoint)	// contact points
		+ (m_numVertsA + m_numVertsB ) * hkSizeOf(VertexId);		// vertices

}

hkpGskManifold::VertexId* hkpGskManifold::getVertexIds()
{
	// return (VertexId*)( &m_contactPoints[m_numContactPoints] );
	return reinterpret_cast<VertexId*>( reinterpret_cast<char*>(&m_contactPoints) + m_numContactPoints*sizeof(ContactPoint) );
}

const hkpGskManifold::VertexId* hkpGskManifold::getVertexIds() const
{
	// return (const VertexId*)( &m_contactPoints[m_numContactPoints] );
	return reinterpret_cast<const VertexId*>( reinterpret_cast<const char*>(&m_contactPoints) + m_numContactPoints*sizeof(ContactPoint) );
}

hkUint16 hkpGskManifold::getVertexId( const ContactPoint& cp, int vertNr ) const
{
	return *hkAddByteOffsetConst(getVertexIds(), (hkUint8)(cp.m_vert[vertNr] >> 3) );	// sizeof(VertexId)/sizeof(hkVector4)
}




#endif //HK_COLLIDE2_GSK_MANIFOLD_H

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
